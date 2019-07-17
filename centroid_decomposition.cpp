// added

#include <bits/stdc++.h>

using namespace std;

class CentroidDecomposition {
    int n;
    vector<vector<int>> adj;
    vector<int> sub;
    vector<bool> visited;
    int build_dfs(int v, int p) {
        sub[v] = 1;
        for (int s : adj[v]) {
            if (s != p && !visited[s]) {
                sub[v] += build_dfs(s, v);
            }
        }
        return sub[v];
    }
    int search_centroid(int v, int p, int mid) {
        for (int s : adj[v]) {
            if (s != p && !visited[s]) {
                if (sub[s] > mid) return search_centroid(s, v, mid);
            }
        }
        return v;
    }
    void path_dfs(int v, int p, int centroid) {
        path[v].push_back(centroid);
        for (int s : adj[v]) {
            if (s != p && !visited[s]) {
                path_dfs(s, v, centroid);
            }
        }
    }
    int build(vector<vector<int>> &t, int v) {
        int centroid = search_centroid(v, -1, build_dfs(v, -1) / 2);
        visited[centroid] = true;
        path_dfs(centroid, -1, centroid);
        for (int s : adj[centroid]) {
            if (!visited[s]) {
                t[centroid].emplace_back(build(t, s));
            }
        }
        visited[centroid] = false;
        return centroid;
    }

public:
    // 分解した木において各頂点から重心までのpath（それぞれO(logN)の長さになる）
    vector<vector<int>> path;
    vector<vector<int>> decomp;

    CentroidDecomposition(int n) : n(n), adj(n), sub(n), visited(n), path(n), decomp(n) {}
    void add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    int build() {
        int root = build(decomp, 0);
        for (int i = 0; i < n; i++) {
            reverse(path[i].begin(), path[i].end());
        }
        return root;
    }
};

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, q;
    cin >> n >> q;
    LCA lca(n);
    CentroidDecomposition decomp(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        lca.add_edge(a, b);
        decomp.add_edge(a, b);
    }
    lca.build();
    decomp.build();
    vector<int> dist(n, 1e9);

    function<void(int)> update = [&](int v) {
        for (int s : decomp.path[v]) {
            dist[s] = min(dist[s], lca.dist(v, s));
        }
    };
    update(0);
    while (q--) {
        int t, v;
        cin >> t >> v;
        v--;
        if (t == 1) {
            update(v);
        } else {
            int ans = 1e9;
            for (int s : decomp.path[v]) {
                ans = min(ans, dist[s] + lca.dist(s, v));
            }
            cout << ans << '\n';
        }

    }
}
