#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

vi dd, ad, dc, ac;

struct edge {
    int to, cost, ok;
};

vector<vector<edge>> edges;

void dfs1(int v, int p) {
    i64 sum = 0;
    i64 m = 0;
    for (edge& e: edges[v]) {
        if (e.to != p) {
            dfs1(e.to, v);
            sum += dc[e.to] + e.ok;
            m = max(m, dd[e.to] + e.cost);
        }
    }
    dc[v] = sum;
    dd[v] = m;
}

void dfs2(int v, int p) {
    using ii = pair<i64, int>;
    vector<ii> ds = {ii(0, 0)};
    i64 cs = 0;
    for (edge& e: edges[v]) {
        if (e.to == p) {
            ds.push_back(ii(ad[v], p));
            cs += ac[v];
        } else {
            ds.push_back(ii(dd[e.to] + e.cost, e.to));
            cs += dc[e.to] + e.ok;
        }
    }
    sort(ds.begin(), ds.end(), greater<>());
    for (edge& e: edges[v]) {
        if (e.to != p) {
            ad[e.to] = ds[ds[0].second == e.to].first + e.cost;
            ac[e.to] = cs - (dc[e.to] + e.ok) + (!e.ok);
            dfs2(e.to, v);
        }
    }
}

int main() {
    int n, d;
    cin >> n >> d;
    dd = vi(n);
    ad = vi(n);
    dc = vi(n);
    ac = vi(n);
    edges = vector<vector<edge>>(n);

    for (int i = 0; i < n - 1; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        edges[a].push_back({b, c, 1});
        edges[b].push_back({a, c, 0});
    }

    dfs1(0, -1);
    dfs2(0, -1);

    i64 ans = 1e9;
    for (int i = 0; i < n; i++) {
        if (max(dd[i], ad[i]) <= d) {
            ans = min(ans, dc[i] + ac[i]);
        }
    }

    // for (int i = 0; i < n; i++) {
    //     cout << "i: " << i << ", dd: " << dd[i] << ", ad: " << ad[i] << ", dc: " << dc[i] << ", ac: " << ac[i] << endl;   
    // }
    cout << (ans == 1e9 ? -1 : ans) << endl;
}