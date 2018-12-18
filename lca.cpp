#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template <class T>
class SegTree {
    using F = function<T(T, T)>;
    int n;
    vector<T> data;
    F f;
    T e;

public:
    SegTree<T>() {}
    SegTree<T>& operator=(const SegTree<T>& t) {
        n = t.n;
        data = t.data;
        f = t.f;
        e = t.e;
        return *this;
    }

    SegTree<T>(const vector<T>& as, const F f, const T& e) : f(f), e(e) {
        n = 1;
        while (n < as.size()) n <<= 1;
        data.assign(2 * n, e);
        for (int i = 0; i < as.size(); i++) {
            data[n + i] = as[i];
        }
        for (int i = n - 1; i > 0; i--) {
            data[i] = f(data[2 * i], data[2 * i + 1]);
        }
    }

    void update(int k, const T& x) {
        k += n;
        data[k] = x;
        while (k >>= 1) {
            data[k] = f(data[2 * k], data[2 * k + 1]);
        }
    }

    T query(int a, int b) {
        T L = e, R = e;
        for (a += n, b += n; a < b; a >>= 1, b >>= 1) {
            if (a & 1) {
                L = f(L, data[a++]);
            }
            if (b & 1) {
                R = f(data[--b], R);
            }
        }
        return f(L, R);
    }
};

class LCA {
    vvi adj;
    vi vs, depth, id, D;
    using ii = pair<i64, int>;
    SegTree<ii> segtree;

    void dfs(int v, int p, int d, int &k) {
        id[v] = k;
        vs[k] = v;
        depth[k++] = d;
        D[v] = d;
        for (int s : adj[v]) {
            if (s != p) {
                dfs(s, v, d + 1, k);
                vs[k] = v;
                depth[k++] = d;
            }
        }
    }

public:
    LCA(const vvi& adj) : adj(adj) {
        int n = adj.size();
        vs.assign(2 * n - 1, 0);
        depth.assign(2 * n - 1, 0);
        id.assign(n, 0);
        D.assign(n, 0);
        int k = 0;
        dfs(0, -1, 0, k);
        vector<ii> ds;
        for (int i = 0; i < depth.size(); i++) {
            ds.push_back(ii(depth[i], i));
        }
        segtree = SegTree<ii>(ds, [](ii a, ii b){return min(a, b);}, ii(1e18, 0));
    }

    int lca(int u, int v) {
        return vs[segtree.query(min(id[u], id[v]), max(id[u], id[v]) + 1).second];
    }

    int dist(int u, int v) {
        return D[u] + D[v] - 2 * D[lca(u, v)];
    }
};

int main() {
    int n;
    cin >> n;
    vvi adj(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    LCA lca(adj);
    int q;
    cin >> q;
    while (q--) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        cout << lca.dist(a, b) + 1 << endl;
    }
}