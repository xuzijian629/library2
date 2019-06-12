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
};

int main() {
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
    int q;
    cin >> q;
    while (q--) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        cout << ds[a] + ds[b] - 2 * ds[lca.lca(a, b)] << '\n';
    }
}