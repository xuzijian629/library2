// added

#include <bits/stdc++.h>

using namespace std;

struct edge {
    int a, b;
    double w;
};

class BellmanFord {
    int n;
    vector<edge> edges;
    vector<double> d;

public:
    BellmanFord(int n) : n(n), edges(n), d(n, numeric_limits<double>::infinity()) {}

    void add_edge(int a, int b, double w) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        edges.push_back({a, b, w});
        edges.push_back({b, a, w});
    }

    void add_arc(int a, int b, double w) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        edges.push_back({a, b, w});
    }

    // return true if has negative loop
    bool run(int a) {
        d[a] = 0;
        for (int i = 0; i < n; i++) {
            bool change = false;
            for (edge e: edges) {
                if (d[e.a] != numeric_limits<double>::infinity()) {
                    if (d[e.b] > d[e.a] + e.w) {
                        d[e.b] = d[e.a] + e.w;
                        change = true;
                    }
                }
            }
            if (!change) {
                return false;
            }
        }
        return true;
    }

    double dist(int a) {
        return d[a];
    }
};

int main() {
    int v, e, r;
    cin >> v >> e >> r;
    BellmanFord g(v);
    for (int i = 0; i < e; i++) {
        int a, b, w;
        cin >> a >> b >> w;
        g.add_arc(a, b, w);
    }
    if (g.run(r)) {
        cout << "NEGATIVE CYCLE" << endl;
        return 0;
    }
    for (int i = 0; i < v; i++) {
        if (g.dist(i) == numeric_limits<double>::infinity()) {
            cout << "INF" << endl;
        } else {
            cout << (long long) g.dist(i) << endl;
        }
    }
}
