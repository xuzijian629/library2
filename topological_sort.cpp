#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class DAG {
    int n;
    vvi adj;
    vi used;

public:
    vi sorted;
    DAG(int n) : n(n), adj(n), used(n) {}

    void addArc(int a, int b) {
        adj[a].push_back(b);
    }
    
    void tsort() {
        function<void(int)> dfs = [&](int v) {
            if (used[v]) return;
            used[v] = 1;
            for (int s: adj[v]) {
                dfs(s);
            }
            sorted.push_back(v);
        };
        for (int i = 0; i < n; i++) {
            dfs(i);
        }
        reverse(sorted.begin(), sorted.end());
    }
};

int main() {
    int v, e;
    cin >> v >> e;
    DAG g(v);
    for (int i = 0; i < e; i++) {
        int a, b;
        cin >> a >> b;
        g.addArc(a, b);
    }
    g.tsort();
    for (int v: g.sorted) {
        cout << v << endl;
    }
}