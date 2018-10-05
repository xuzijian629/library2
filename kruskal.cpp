#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;

struct edge {
    int a, b;
    i64 w;
};

i64 kruskal(vector<edge>& edges, int n) {
    vector<int> par(n);
    for (int i = 0; i < n; i++) {
        par[i] = i;
    }
    function<int(int)> find = [&](int x) {
        if (x == par[x]) return x;
        return par[x] = find(par[x]);
    };
    auto unite = [&](int x, int y) {
        x = find(x);
        y = find(y);
        par[x] = y;
    };
    auto same = [&](int x, int y) {
        return find(x) == find(y);
    };
    sort(edges.begin(), edges.end(), [](edge a, edge b) {
        return a.w < b.w;
    });
    i64 ret = 0;
    for (edge e: edges) {
        if (!same(e.a, e.b)) {
            unite(e.a, e.b);
            ret += e.w;
        }
    }
    return ret;
}

int main() {
    int v, e;
    cin >> v >> e;
    vector<edge> edges;
    for (int i = 0; i < e; i++) {
        int a, b;
        i64 w;
        cin >> a >> b >> w;
        edges.push_back({a, b, w});
    }
    cout << kruskal(edges, v) << endl;
}
