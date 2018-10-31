#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class BipartiteMatching {
    int n;
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
    BipartiteMatching(int n) : n(n), adj(n), mc(n, -1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
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
    int n;
    cin >> n;
    BipartiteMatching match(n + 50);
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        for (int j = 0; j < n; j++) {
            if (a == j) continue;
            match.addEdge(i, n + j);
        }
    }
    if (match.match() != n) {
        cout << -1 << endl;
        return 0;
    }
    for (int i = 0; i < n; i++) {
        cout << match[i] - n << endl;
    }
}