// verified: https://www.spoj.com/submit/QTREE2/

#include <bits/stdc++.h>
using namespace std;

class LCA {
    int n;
    const int LOG = 20;
    vector<int> ds;
    vector<vector<int>> adj;
    vector<vector<int>> table;

    void dfs(int v, int p, int d) {
        ds[v] = d;
        table[v][0] = p;
        for (int s : adj[v]) {
            if (s != p) {
                dfs(s, v, d + 1);
            }
        }
    }

public:
    LCA(int n) : n(n), ds(n), adj(n), table(n, vector<int>(LOG, -1)) {}

    void add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    void build() {
        dfs(0, -1, 0);
        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i < n; i++) {
                if (table[i][k - 1] == -1) table[i][k] = -1;
                else table[i][k] = table[table[i][k - 1]][k - 1];
            }
        }
    }

    int lca(int u, int v) {
        if (ds[u] > ds[v]) swap(u, v);
        for (int i = LOG - 1; i >= 0; i--) {
            if (((ds[v] - ds[u]) >> i) & 1) v = table[v][i];
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if ((table[u][i] != table[v][i])) {
                u = table[u][i];
                v = table[v][i];
            }
        }
        return table[u][0];
    }

    int dist(int u, int v) {
        return ds[u] + ds[v] - 2 * ds[lca(u, v)];
    }

    // u->vパスでuから距離kにある頂点の番号
    int kth_node(int u, int v, int k) {
        if (k == 0) return u;
        if (k > dist(u, lca(u, v))) return kth_node(v, u, dist(u, v) - k);
        int w = u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (1 << i <= k) {
                assert(table[w][i] != -1);
                w = table[w][i];
                k -= 1 << i;
                if (k == 0) return w;
            }
        }
        assert(false);
    }
};

void solve() {
    int n;
    cin >> n;
    vector<vector<pair<int, int>>> adj(n);
    LCA lca(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        a--, b--;
        adj[a].emplace_back(b, w);
        adj[b].emplace_back(a, w);
        lca.add_edge(a, b);
    }
    lca.build();
    vector<long long> ds(n);
    function<void(int, int, long long)> dfs = [&](int v, int p, long long d) {
        ds[v] = d;
        for (auto &s : adj[v]) {
            if (s.first != p) {
                dfs(s.first, v, d + s.second);
            }
        }
    };
    dfs(0, -1, 0);
    while (1) {
        string s;
        cin >> s;
        if (s == "DIST") {
            int a, b;
            cin >> a >> b;
            a--, b--;
            cout << ds[a] + ds[b] - 2 * ds[lca.lca(a, b)] << '\n';
        } else if (s == "KTH") {
            int a, b, k;
            cin >> a >> b >> k;
            a--, b--, k--;
            cout << lca.kth_node(a, b, k) + 1 << '\n';
        } else {
            break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) solve();
}