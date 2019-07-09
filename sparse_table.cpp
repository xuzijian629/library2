#include <bits/stdc++.h>

using namespace std;

template<typename T>
struct SparseTable {
    vector<vector<T>> st;
    vector<int> lookup;

    SparseTable() {}

    SparseTable(const vector<T> &v) {
        int b = 0;
        while ((1 << b) <= v.size()) b++;
        st.assign(b, vector<T>(1 << b));
        for (int i = 0; i < v.size(); i++) {
            st[0][i] = v[i];
        }
        for (int i = 1; i < b; i++) {
            for (int j = 0; j + (1 << i) <= 1 << b; j++) {
                st[i][j] = min(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
            }
        }
        lookup.resize(v.size() + 1);
        for (int i = 2; i < lookup.size(); i++) {
            lookup[i] = lookup[i >> 1] + 1;
        }
    }

    T query(int l, int r) {
        int b = lookup[r - l];
        return min(st[b][l], st[b][r - (1 << b)]);
    }
};

struct LCA {
    int n, k = 0;
    vector<vector<int>> adj;
    vector<int> vs, depth, id, D;
    SparseTable<pair<int, int>> st;

    void dfs(int v, int p, int d) {
        id[v] = k;
        vs[k] = v;
        depth[k++] = d;
        D[v] = d;
        for (int s : adj[v]) {
            if (s != p) {
                dfs(s, v, d + 1);
                vs[k] = v;
                depth[k++] = d;
            }
        }
    }

    LCA() {}

    LCA(const vector<vector<int>> &adj) : n(adj.size()), adj(adj), vs(2 * adj.size() - 1), depth(2 * adj.size() - 1),
                                          id(adj.size()), D(adj.size()) {
        dfs(0, -1, 0);
        vector<pair<int, int>> ds;
        for (int i = 0; i < depth.size(); i++) {
            ds.emplace_back(depth[i], i);
        }
        st = SparseTable<pair<int, int>>(ds);
    }

    int lca(int u, int v) {
        return vs[st.query(min(id[u], id[v]), max(id[u], id[v]) + 1).second];
    }

    int dist(int u, int v) {
        return D[u] + D[v] - 2 * D[lca(u, v)];
    }
};

// vertex query
// edgeクエリは書いていないが、add, del関数を2引数にして、add(from, to), del(from, to)のときに、辺(from, to)の重みを操作すればよさそう
// edgeクエリのときはLCAがいらない
struct TreeMo {
    int n, block;
    vector<int> order, le, ri, lcas, in, vs;
    vector<bool> visited;
    function<void(int)> add, del;
    function<int()> get;
    int nl, nr;
    vector<vector<int>> adj;
    LCA lca;
    bool built = false;

    TreeMo() {}

    TreeMo(int n, function<void(int)> add, function<void(int)> del, function<int()> get) : n(n), adj(n),
                                                                                           block((int) sqrt(2 * n - 1)),
                                                                                           nl(0), nr(0), visited(n),
                                                                                           in(n), add(move(add)),
                                                                                           del(move(del)),
                                                                                           get(move(get)) {}

    void dfs(int v, int p, int d) {
        in[v] = vs.size();
        vs.push_back(v);
        for (int s : adj[v]) {
            if (s != p) {
                dfs(s, v, d + 1);
                vs.push_back(s);
            }
        }
    }

    void add_edge(int a, int b) {
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    void build_tree() {
        lca = LCA(adj);
        dfs(0, -1, 0);
        built = true;
    }

    void add_query(int u, int v) {
        assert(built && 0 <= u && u < n && 0 <= v && v < n);
        if (in[u] > in[v]) swap(u, v);
        le.push_back(in[u] + 1);
        ri.push_back(in[v] + 1);
        lcas.push_back(lca.lca(u, v));
    }

    void shift(int v) {
        visited[v].flip();
        if (visited[v]) add(v);
        else del(v);
    }

    vector<int> answer() {
        int qcnt = le.size();
        order.resize(qcnt);
        vector<int> ret(qcnt);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return le[a] / block != le[b] / block ? le[a] < le[b] : ri[a] < ri[b];
        });
        for (int i = 0; i < qcnt; i++) {
            if (i > 0) shift(lcas[order[i - 1]]);
            int idx = order[i];
            while (nl > le[idx]) shift(vs[--nl]);
            while (nr < ri[idx]) shift(vs[nr++]);
            while (nl < le[idx]) shift(vs[nl++]);
            while (nr > ri[idx]) shift(vs[--nr]);
            shift(lcas[idx]);
            ret[idx] = get();
        }
        return ret;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    vector<int> ws(n), ws_compress(n);
    // cnt[a]: 要素aのカウント
    vector<int> cnt;
    // 種類数
    int sum = 0;
    map<int, int> mp;
    for (int i = 0; i < n; i++) {
        cin >> ws[i];
        mp[ws[i]];
    }
    {
        int N = 0;
        for (auto &p : mp) p.second = N++;
        cnt.resize(N);
        for (int i = 0; i < n; i++) {
            ws_compress[i] = mp[ws[i]];
        }
    }
    auto add = [&](int v) {
        if (cnt[ws_compress[v]] == 0) sum++;
        cnt[ws_compress[v]]++;
    };
    auto del = [&](int v) {
        cnt[ws_compress[v]]--;
        if (cnt[ws_compress[v]] == 0) sum--;
    };
    auto get = [&]() {
        return sum;
    };
    TreeMo mo(n, add, del, get);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        mo.add_edge(a, b);
    }
    mo.build_tree();
    while (q--) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        mo.add_query(a, b);
    }
    for (int a : mo.answer()) {
        cout << a << '\n';
    }
}