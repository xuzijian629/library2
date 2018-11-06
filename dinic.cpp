#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class Dinic {
    struct edge {
        int to, cap, rev;
    };

    int n;
    vector<vector<edge>> G;
    vi level;
    vi iter;

    void bfs(int s) {
        level.assign(n, -1);
        queue<int> que;
        que.push(s);
        level[s] = 0;
        while (que.size()) {
            int v = que.front();
            que.pop();
            for (int i = 0; i < G[v].size(); i++) {
                edge &e = G[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int i = iter[v]; i < G[v].size(); i++) {
            edge &e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
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
    Dinic(int n) : n(n), G(n), level(n), iter(n) {}

    void addEdge(int from, int to, int cap) {
        G[from].push_back({to, cap, int(G[to].size())});
        G[to].push_back({from, 0, int(G[from].size()) - 1});
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (1) {
            bfs(s);
            if (level[t] < 0) return flow;
            iter.assign(n, 0);
            int f;
            while ((f = dfs(s, t, 1e9)) > 0) {
                flow += f;
            }
        }
    }
};

int main() {
    int v, e;
    cin >> v >> e;
    Dinic d(v);
    for (int i = 0; i < e; i++) {
        int u, v, c;
        cin >> u >> v >> c;
        d.addEdge(u, v, c);
    }

    cout << d.max_flow(0, v - 1) << endl;
}