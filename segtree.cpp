// added
#include <bits/stdc++.h>

using namespace std;

struct HLD {
    int n;
    vector<vector<int>> adj;
    vector<int> sz, in, out, head, rev, par, depth;

    HLD(int n) : n(n), adj(n), sz(n), in(n), out(n), head(n), rev(n), par(n), depth(n) {}

    void add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    void dfs_sz(int v, int p, int d) {
        par[v] = p;
        sz[v] = 1;
        depth[v] = d;
        if (!adj[v].empty() && adj[v][0] == p) swap(adj[v][0], adj[v].back());
        for (int &s : adj[v]) {
            if (s == p) continue;
            dfs_sz(s, v, d + 1);
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

    void build() {
        dfs_sz(0, -1, 0);
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

    template<typename Q, typename F, typename T>
    // qは閉区間に対応させること!!!!!!!!
    T query(int u, int v, const Q &q, const F &f, const T &e, bool edge = false) {
        T l = e, r = e;
        for (;; v = par[head[v]]) {
            if (in[u] > in[v]) swap(u, v), swap(l, r);
            if (head[u] == head[v]) break;
            l = f(q(in[head[v]], in[v]), l);
        }
        return f(f(q(in[u] + edge, in[v]), l), r);
    }

    template<typename Q>
    // qは閉区間に対応させること!!!!!!!!
    void update(int u, int v, const Q &q, bool edge = false) {
        for (;; v = par[head[v]]) {
            if (in[u] > in[v]) swap(u, v);
            if (head[u] == head[v]) break;
            q(in[head[v]], in[v]);
        }
        q(in[u] + edge, in[v]);
    }
};

class BIT {
    int n;
    vector<int> data0, data1;

    void add(vector<int> &data, int i, int x) {
        while (i <= n) {
            data[i] += x;
            i += i & -i;
        }
    }

    int sum(vector<int> &data, int i) {
        int s = 0;
        while (i > 0) {
            s += data[i];
            i -= i & -i;
        }
        return s;
    }

public:
    BIT(int n) : n(n), data0(n + 1), data1(n + 1) {}

    // [l, r)
    void add(int l, int r, int x) {
        l++;
        r++;
        add(data0, l, -x * (l - 1));
        add(data1, l, x);
        add(data0, r, x * (r - 1));
        add(data1, r, -x);
    }

    // [l, r)
    int sum(int l, int r) {
        int s = 0;
        s += sum(data0, r) + sum(data1, r) * r;
        s -= sum(data0, l) + sum(data1, l) * l;
        return s;
    }
};

// T0: 元の配列のモノイド
// T1: T0に対する作用素モノイド
template<class T0, class T1>
class SegmentTree {
    // k番目のノードにのlazyを伝搬
    void eval(int k, int len) {
        // 定数倍高速化
        if (lazy[k] == u1) return;
        // len個分のlazy[k]を評価
        node[k] = g(node[k], p(lazy[k], len));
        if (k < N - 1) {
            // 最下段でなければ下のlazyに伝搬
            lazy[2 * k + 1] = f1(lazy[2 * k + 1], lazy[k]);
            lazy[2 * k + 2] = f1(lazy[2 * k + 2], lazy[k]);
        }
        lazy[k] = u1;
    }

    // k番目のノード[l, r)について、[a, b)の範囲内にxを作用
    void update(int a, int b, T1 x, int k, int l, int r) {
        eval(k, r - l);
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            lazy[k] = f1(lazy[k], x);
            eval(k, r - l);
        } else {
            update(a, b, x, 2 * k + 1, l, (l + r) / 2);
            update(a, b, x, 2 * k + 2, (l + r) / 2, r);
            node[k] = f0(node[2 * k + 1], node[2 * k + 2]);
        }
    }

    // k番目のノード[l, r)について、[a, b)のクエリを求める
    T0 query(int a, int b, int k, int l, int r) {
        if (r <= a || b <= l) return u0;
        eval(k, r - l);
        if (a <= l && r <= b) return node[k];
        T0 vl = query(a, b, 2 * k + 1, l, (l + r) / 2);
        T0 vr = query(a, b, 2 * k + 2, (l + r) / 2, r);
        return f0(vl, vr);
    }

public:
    int sz; // 元の配列のサイズ
    int N;
    vector<T0> node;
    vector<T1> lazy;
    // T0上の演算、単位元
    using F0 = function<T0(T0, T0)>;
    F0 f0;
    T0 u0;
    // T1上の演算、単位元
    using F1 = function<T1(T1, T1)>;
    F1 f1;
    T1 u1;
    // 作用
    using G = function<T0(T0, T1)>;
    G g;
    // 多数のt1(T1)に対するf1の合成
    using P = function<T1(T1, int)>;
    P p;

    SegmentTree(const vector<T0> &a, F0 f0, T0 u0, F1 f1, T1 u1, G g, P p)
            : sz(a.size()), f0(f0), u0(u0), f1(f1), u1(u1), g(g), p(p) {
        for (N = 1; N < sz; N *= 2);
        node.resize(2 * N - 1);
        lazy.resize(2 * N - 1, u1);
        for (int i = 0; i < sz; i++) node[N - 1 + i] = a[i];
        for (int i = N - 2; i >= 0; i--) node[i] = f0(node[2 * i + 1], node[2 * i + 2]);
    }

    // [a, b)にxを作用
    void update(int a, int b, T1 x) {
        assert(0 <= a && a < b && b <= sz);
        update(a, b, x, 0, 0, N);
    }

    void update(int a, T1 x) {
        update(a, a + 1, x);
    }

    // [a, b)
    T0 query(int a, int b) {
        return query(a, b, 0, 0, N);
    }

    T0 query(int a) {
        return query(a, a + 1);
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    HLD hld(n);
    for (int i = 1; i < n; i++) {
        int p;
        cin >> p;
        p--;
        hld.add_edge(p, i);
    }
    hld.build();
//    // Min & Update
//    SegmentTree<int, int> seg(
//            vector<int>(n, (1LL << 31) - 1),
//            [](int x, int y) { return min(x, y); }, 1e18,
//            [](int x, int y) { return y == 1e18 ? x : y; }, 1e18,
//            [](int x, int y) { return y == 1e18 ? x : y; },
//            [](int y, int len) { return y; }
//    );
//
//    // Sum & Add
//    SegmentTree<int, int> seg(
//            vector<int>(n, 0),
//            plus<int>(), 0,
//            plus<int>(), 0,
//            plus<int>(),
//            multiplies<int>()
//    );
//
//    // Min & Add
//    SegmentTree<int, int> seg(
//            vector<int>(n, 0),
//            [](int x, int y) { return min(x, y); }, 1e18,
//            plus<int>(), 0,
//            plus<int>(),
//            [](int y, int len) { return y; }
//    );
//
//    // Sum & Update
//    constexpr int u1 = 1e18;
//    SegmentTree<int, int> seg(
//            vector<int>(n),
//            plus<int>(), 0,
//            [](int x, int y) { return y == u1 ? x : y; }, u1,
//            [](int x, int y) { return y == u1 ? x : y; },
//            [](int y, int len) { return y == u1 ? u1 : y * len; }
//    );

    // Sum & Affine
    using T = int;
    using Affine = pair<T, T>;
    SegmentTree<Affine, Affine> seg(
            vector<Affine>(n),
            [](Affine x, Affine y) { return Affine(x.first + y.first, x.second + y.second); }, Affine(0, 0),
            [](Affine x, Affine y) { return Affine(x.first * y.first, x.second * y.first + y.second); }, Affine(1, 0),
            [](Affine x, Affine y) { return Affine(x.first * y.first, x.second * y.first + y.second); },
            [](Affine y, int len) { return Affine(y.first, y.second * len); }
    );
//    seg.update(i, j, {a, b}); // [i, j)にax + bを作用
//    seg.update(i, j, {0, a}); // update
//    seg.update(i, j, {1, a}); // 加算
//    seg.update(i, j, {a, 0}); // 倍
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        seg.update(hld.in[i], hld.in[i] + 1, {0, a});
    }

    int q;
    cin >> q;
    while (q--) {
        string s;
        cin >> s;
        if (s == "get") {
            int v;
            cin >> v;
            v--;
            cout << seg.query(hld.in[v], hld.out[v]).second << '\n';
        } else {
            int v;
            cin >> v;
            v--;
            seg.update(hld.in[v], hld.out[v], {-1, 1});
        }
    }
}
