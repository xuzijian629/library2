// added

#include <bits/stdc++.h>

using namespace std;

class FordFulkerson {
    struct edge {
        int to, cap, rev;
    };

    int n;
    vector<vector<edge>> G;
    vector<bool> used;

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        used[v] = true;

        for (int i = 0; i < G[v].size(); i++) {
            edge &e = G[v][i];
            if (!used[e.to] && e.cap > 0) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    FordFulkerson(int n) : n(n), G(n), used(n) {}

    void addEdge(int from, int to, int cap) {
        G[from].push_back({to, cap, int(G[to].size())});
        G[to].push_back({from, 0, int(G[from].size()) - 1});
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (1) {
            used.assign(n, 0);
            int f = dfs(s, t, 1e9);
            if (f == 0) return flow;
            flow += f;
        }
    }
};

int main() {
    int v, e;
    cin >> v >> e;
    FordFulkerson f(v);
    for (int i = 0; i < e; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        f.addEdge(u, v, c);
    }

    cout << f.max_flow(0, v - 1) << endl;
}