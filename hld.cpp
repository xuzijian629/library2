// Added

#include <bits/stdc++.h>
#define int long long

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

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    HLD hld(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        hld.add_edge(a, b);
    }
    hld.build();
    BIT bit(n);


    auto sum = [&](int u, int v) {
        return bit.sum(u, v + 1);
    };

    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int u, v;
            cin >> u >> v;
            cout << hld.query(u, v, sum, [](int a, int b) { return a + b; }, 0ll, true) << '\n';
        } else {
            int v, x;
            cin >> v >> x;
            // こっちは半開区間
            // 辺クエリなので1足している
            bit.add(hld.in[v] + 1, hld.out[v], x);
        }
    }
}