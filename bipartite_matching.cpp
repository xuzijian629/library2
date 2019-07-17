// added

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class BipartiteMatching {
    int n, n0;
    int mode;
    vvi adj;
    vi mc;
    vi used;

    bool dfs(int v) {
        used[v] = true;
        for (int i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            int w = mc[u];
            if (w < 0 || (!used[w] && dfs(w))) {
                mc[v] = u;
                mc[u] = v;
                return true;
            }
        }
        return false;
    }
public:
    BipartiteMatching(int n) : n(n), adj(n), mc(n, -1) {
        mode = 0;
    }
    BipartiteMatching(int n, int m) : n(n + m), n0(n), adj(n + m), mc(n + m, -1) {
        mode = 1;
    }

    void addEdge(int u, int v) {
        if (mode == 0) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        } else {
            adj[u].push_back(n0 + v);
            adj[n0 + v].push_back(u);
        }
    }

    int match() {
        int ret = 0;
        for (int v = 0; v < n; v++) {
            if (mc[v] < 0) {
                used = vi(n);
                if (dfs(v)) {
                    ret++;
                }
            }
        }
        return ret;
    }

    int operator[](int k) {
        return mc[k];
    }
};

int main() {
    int x, y, e;
    cin >> x >> y >> e;
    BipartiteMatching match(x, y);
    for (int i = 0; i < e; i++) {
        int a, b;
        cin >> a >> b;
        match.addEdge(a, b);
    }

    cout << match.match() << endl;
}