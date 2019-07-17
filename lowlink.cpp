// ushiadded

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

struct Lowlink {
    int n;
    vvi adj;
    vi ord, low, visited;
    Lowlink(const vvi &adj) : n(adj.size()), adj(adj), ord(adj.size()), low(adj.size()), visited(adj.size()) {
        int k = 0;
        dfs(0, -1, k);
    }
    vi articulations;

    void dfs(int v, int p, int &k) {
        ord[v] = k++;
        visited[v] = 1;
        low[v] = ord[v];
        for (int s : adj[v]) {
            if (s == p) continue;
            if (!visited[s]) {
                dfs(s, v, k);
            }
            low[v] = min(low[v], ord[s]);
        }
    }

    bool is_bridge(int u, int v) {
        if (ord[u] > ord[v]) return is_bridge(v, u);
        return ord[u] < low[v];
    }

    // return articulation vertex
    int is_articulation(int u, int v) {
        if (ord[u] > ord[v]) return is_bridge(v, u);
        return ord[u] <= low[v] ? u : -1;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vvi adj(n);
    vector<pair<int, int>> es;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
        es.emplace_back(a, b);
    }
    Lowlink ll(adj);
    set<int> as;
    for (int i = 0; i < m; i++) {
        int k = ll.is_articulation(es[i].first, es[i].second);
        if (k != -1) as.insert(k);
    }
    for (int a : as) {
        cout << a << '\n';
    }
}