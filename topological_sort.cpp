#include <bits/stdc++.h>
using namespace std;

class DAG {
    int n;
    vector<vector<int>> adj;
    vector<int> visited;

    void dfs(int v) {
        if (visited[v] == 2) {
            is_acyclic = false;
            return;
        } else if (!visited[v]) {
            visited[v] = 2;
            for (int s : adj[v]) {
                dfs(s);
            }
            visited[v] = 1;
            sorted.push_back(v);
        }
    }

public:
    vector<int> sorted;
    DAG(int n) : n(n), adj(n), visited(n) {}
    bool is_acyclic = true;

    void add_arc(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(b);
    }

    void tsort() {
        for (int i = 0; i < n; i++) {
            if (!visited[i]) dfs(i);
        }
        reverse(sorted.begin(), sorted.end());
    }
};
