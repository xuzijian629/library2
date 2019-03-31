#include <bits/stdc++.h>
using namespace std;

class Graph {
    int n, max_color;
    vector<int> color;
    bool is_bip = true;
    vector<vector<int>> blacks, whites;

    void dfs(int v, int c) {
        if (color[v] != -1) {
            if (color[v] != c) {
                is_bip = false;
            }
            return;
        }
        color[v] = c;
        for (int i: adj[v]) {
            dfs(i, c ^ 1);
        }
    }

public:
    vector<vector<int>> adj;
    vector<vector<int>> adj_matrix;

    Graph(int n) : n(n), max_color(0), adj(n) {}

    void use_adj_matrix() {
        adj_matrix = vector<vector<int>>(n, vector<int>(n));
    }

    void add_edge(int a, int b) {
        assert(0 <= a && a < n);
        assert(0 <= b && b < n);
        if (adj_matrix.size()) {
            adj_matrix[a][b] = 1;
            adj_matrix[b][a] = 1;
        }
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    bool is_bipartite() {
        color = vector<int>(n, -1);
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                dfs(i, max_color);
                max_color += 2;
            }
        }
        return is_bip;
    }

    bool is_connected() {
        color = vector<int>(n, -1);
        dfs(0, 0);
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                return false;
            }
        }
        return true;
    }

    vector<vector<int>> get_blacks() {
        blacks = vector<vector<int>>(max_color / 2);
        for (int i = 0; i < n; i++) {
            if (color[i] % 2 == 0) {
                blacks[color[i] / 2].push_back(i);
            }
        }
        return blacks;
    }

    vector<vector<int>> get_whites() {
        whites = vector<vector<int>>(max_color / 2);
        for (int i = 0; i < n; i++) {
            if (color[i] % 2 == 1) {
                whites[color[i] / 2].push_back(i);
            }
        }
        return whites;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    Graph g(n);
    vector<pair<int, int>> es;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        g.add_edge(a, b);
        es.emplace_back(a, b);
    }
    if (!g.is_bipartite()) {
        cout << "NO" << endl;
        return 0;
    }
    cout << "YES" << endl;
    set<int> blacks, whites;
    for (auto& v : g.get_blacks()) {
        for (int a : v) {
            blacks.insert(a);
        }
    }
    for (auto& v : g.get_whites()) {
        for (int a : v) {
            whites.insert(a);
        }
    }
    for (int i = 0; i < m; i++) {
        if (blacks.count(es[i].first)) {
            cout << 1;
        } else {
            cout << 0;
        }
    }
}
