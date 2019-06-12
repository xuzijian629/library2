// MOVED

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;

class Dijkstra {
    int n;
    vector<vector<pair<int, double>>> adj;
    vector<double> d;

public:
    Dijkstra(int n) : n(n) {
        adj = vector<vector<pair<int, double>>>(n);
        d = vector<double>(n, numeric_limits<double>::infinity());
    }

    void addEdge(int a, int b, double w) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(make_pair(b, w));
        adj[b].push_back(make_pair(a, w));
    }
    void addArc(int a, int b, double w) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(make_pair(b, w));
    }
    void run(int a) {
        d[a] = 0;
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> q;
        q.push(make_pair(0, a));

        while (!q.empty()) {
            auto top = q.top();
            int u = top.second;
            q.pop();

            if (d[u] < top.first) continue;
            for (auto i: adj[u]) {
                if (d[i.first] > d[u] + i.second) {
                    d[i.first] = d[u] + i.second;
                    q.push(make_pair(d[i.first], i.first));
                }
            }
        }
    }

    double dist(int a) {
        return d[a];
    }
};

int main() {
    int v, e, r;
    cin >> v >> e >> r;

    Dijkstra g(v);

    for (int i = 0; i < e; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        g.addArc(a, b, w);
    }

    g.run(r);
    for (int i = 0; i < v; i++) {
        double d = g.dist(i);
        if (d == numeric_limits<double>::infinity()) {
            cout << "INF" << endl;
        } else {
            cout << i64(d) << endl;
        }
    }
}
