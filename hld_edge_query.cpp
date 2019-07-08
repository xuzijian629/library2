#include <bits/stdc++.h>
using namespace std;
#pragma GCC Optimize("Ofast")

class HLD_NodeQuery {
    int n;
    vector<vector<int>> adj;
    vector<int> sz, in, out, head, rev, par;
    void dfs_sz(int v, int p) {
        par[v] = p;
        sz[v] = 1;
        if (!adj[v].empty() && adj[v][0] == p) swap(adj[v][0], adj[v].back());
        for (int &s : adj[v]) {
            if (s == p) continue;
            dfs_sz(s, v);
            sz[v] += sz[s];
            if (sz[adj[v][0]] < sz[s]) swap(adj[v][0], s);
        }
    }
    void dfs_hld(int v, int p, int &times) {
        in[v] = times++;
        rev[in[v]] = v;
        for (int &s : adj[v]) {
            if (s == p) continue;
            head[s] = adj[v][0] == s ? head[v] : s;
            dfs_hld(s, v, times);
        }
        out[v] = times;
    }
public:
    HLD_NodeQuery() {}
    HLD_NodeQuery(int n) : n(n), adj(n), sz(n), in(n), out(n), head(n), rev(n), par(n) {}
    void add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    void build() {
        dfs_sz(0, -1);
        int t = 0;
        dfs_hld(0, -1, t);
    }
    int la(int v, int k) {
        while (1) {
            int u = head[v];
            if (in[v] - k >= in[u]) return rev[in[v] - k];
            k -= in[v] - in[u] + 1;
            v = par[u];
        }
    }
    int lca(int u, int v) {
        for (;; v = par[head[v]]) {
            if (in[u] > in[v]) swap(u, v);
            if (head[u] == head[v]) return u;
        }
    }

    template <typename T>
    void set_hld_array(int idx, T value, vector<T> &array) {
        array[in[idx]] = value;
    }
    template <typename T>
    T get_hld_array(int idx, vector<T> &array) {
        return array[in[idx]];
    }

    template <typename Query>
    // Queryはhoge(u, v)の形の関数。(u, v)のペアが何度か実行される形になる。別に配列やaccumulate用の変数を用意して使う
    // [u, v]
    void node_query(int u, int v, Query query) {
        for (;; v = par[head[v]]) {
            if (in[u] > in[v]) swap(u, v);
            if (head[u] == head[v]) break;
            query(in[head[v]], in[v]);
        }
        query(in[u], in[v]);
    }
};

// 配列がreadonlyか1点更新の場合しか使えない
class HLD_EdgeQuery {
    int n;
    int edge_cnt = 0;
    HLD_NodeQuery hld_nodequery;
    map<pair<int, int>, int> midmap;
public:
    HLD_EdgeQuery(int n) : n(n), hld_nodequery(2 * n - 1) {}
    void add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        int mid = n + edge_cnt++;
        hld_nodequery.add_edge(a, mid);
        hld_nodequery.add_edge(mid, b);
        midmap[{min(a, b), max(a, b)}] = mid;
    }
    void build() {
        hld_nodequery.build();
    }

    int la(int v, int k) {
        return hld_nodequery.la(v, k);
    }
    int lca(int u, int v) {
        return hld_nodequery.lca(u, v);
    }

    template <typename T>
    // (u, v) must be an edge
    void set_hld_array(int u, int v, T value, vector<T> &array) {
        int mid = midmap[{min(u, v), max(u, v)}];
        hld_nodequery.set_hld_array(mid, value, array);
    }
    template <typename T>
    // (u, v) must be an edge
    T get_hld_array(int u, int v, vector<int> &array) {
        int mid = midmap[{min(u, v), max(u, v)}];
        return hld_nodequery.get_hld_array(mid, array);
    }

    template <typename Query>
    // Queryはhoge(u, v)の形の関数。(u, v)のペアが何度か実行される形になる。別に配列やaccumulate用の変数を用意して使う。
    // もともとの木の頂点に対応するarrayの値は演算の単位元にしておく
    void edge_query(int u, int v, Query query) {
        return hld_nodequery.node_query(u, v, query);
    }
};

// (type, value)の配列に対し、[l, r)内になるtype kのvalueについての累積。とりあえず累積和
struct PairQuery {
    vector<vector<pair<int, int>>> acc;
    PairQuery(const vector<pair<int, int>> &as, int MAX_TYPE) : acc(MAX_TYPE, vector<pair<int, int>>(1, {-1, 0})) {
        int n = as.size();
        for (int i = 0; i < n; i++) {
            int t = as[i].first, v = as[i].second;
            if (t == -1) continue;
            acc[t].emplace_back(i, acc[t].back().second + v);
        }
    }
    // [l, r)
    int query(int l, int r, int k) {
        auto R = lower_bound(acc[k].begin(), acc[k].end(), make_pair(r, 0));
        auto L = lower_bound(acc[k].begin(), acc[k].end(), make_pair(l, 0));
        --R;
        --L;
        return R->second - L->second;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    HLD_EdgeQuery hld(n);
    vector<vector<int>> adj(n);
    map<pair<int, int>, int> cs, ds;
    for (int i = 0; i < n - 1; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        a--, b--;
        cs[{min(a, b), max(a, b)}] = c;
        ds[{min(a, b), max(a, b)}] = d;
        hld.add_edge(a, b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    hld.build();
    vector<int> depth(n);
    function<void(int, int, int)> dfs = [&](int v, int p, int d) {
        depth[v] = d;
        for (int s : adj[v]) {
            if (s != p) dfs(s, v, d + ds[{min(s, v), max(s, v)}]);
        }
    };
    dfs(0, -1, 0);
    auto dist = [&](int u, int v) {
        int w = hld.lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    };
    vector<pair<int, int>> D(2 * n - 1, {-1, 0}), E(2 * n - 1, {-1, 0});
    for (auto &e : cs) {
        int a = e.first.first, b = e.first.second, c = e.second;
        hld.set_hld_array(a, b, {c, ds[{min(a, b), max(a, b)}]}, D);
        hld.set_hld_array(a, b, {c, 1}, E);
    }
    PairQuery qD(D, n), qE(E, n);

    int X, reg;
    auto color_sum = [&](int i, int j) -> void {
        reg += qD.query(i, j, X);
    };
    auto color_num = [&](int i, int j) -> void {
        reg += qE.query(i, j, X);
    };

    while (q--) {
        int x, y, u, v;
        cin >> x >> y >> u >> v;
        u--, v--;
        int ret = dist(u, v);
        X = x;
        reg = 0;
        hld.edge_query(u, v, color_sum);
        ret -= reg;
        reg = 0;
        hld.edge_query(u, v, color_num);
        ret += reg * y;
        cout << ret << '\n';
    }
}