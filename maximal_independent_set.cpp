#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;

struct UnionFind {
    int n, cnt;
    vi par, rank, sz;

    UnionFind(int n) : n(n), cnt(n), par(n), rank(n), sz(n, 1) {
        iota(par.begin(), par.end(), 0);
    }

    int find(int x) {
        if (x == par[x]) return x;
        return par[x] = find(par[x]);
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int size(int x) {
        return sz[find(x)];
    }

    void unite(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return;
        if (rank[x] < rank[y]) {
            par[x] = y;
            sz[y] += sz[x];
        } else {
            par[y] = x;
            sz[x] += sz[y];
            if (rank[x] == rank[y]) {
                rank[x]++;
            }
        }
        cnt--;
    }
};

struct MaximalIndependentSetForest {
    struct Edge { int to, type; }; // type 1: real edge, type 0: dummy edge to make connected
    vector<vector<Edge>> G;
    MaximalIndependentSetForest(const vector<pair<int, int>>& edges) {
        map<int, int> mp;
        for (auto& e : edges) {
            mp[e.first], mp[e.second];
        }
        int N = 0;
        for (auto& p : mp) {
            p.second = N++;
        }
        UnionFind uf(N);
        G = vector<vector<Edge>>(N);
        for (auto& e : edges) {
            int a = mp[e.first], b = mp[e.second];
            uf.unite(a, b);
            G[a].push_back({b, 1});
            G[b].push_back({a, 1});
        }
        for (int i = 1; i < N; i++) {
            if (!uf.same(0, i)) {
                G[0].push_back({i, 0});
                G[i].push_back({0, 0});
                uf.unite(0, i);
            }
        }
    }

    pair<int, int> dfs(int v, int p) {
        pair<int, int> ret = {1, 0};
        for (Edge& e : G[v]) {
            if (e.to != p) {
                pair<int, int> tmp = dfs(e.to, v);
                ret.second += max(tmp.first, tmp.second);
                if (e.type) {
                    ret.first += tmp.second;
                } else {
                    ret.first += max(tmp.first, tmp.second);
                }
            }
        }
        return ret;
    }
    int solve() {
        if (G.empty()) return 0;
        pair<int, int> ans = dfs(0, -1);
        return max(ans.first, ans.second);
    }
};

struct MaxCutFree {
    int solve(int n, vector<int> as, vector<int> bs) {
        int m = as.size();
        vvi adj(n);
        for (int i = 0; i < m; i++) {
            int a = as[i], b = bs[i];
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        set<int> ss;
        vector<pair<int, int>> bridges;
        for (int i = 0; i < m; i++) {
            // ignore i-th edge
            UnionFind uf(n);
            for (int j = 0; j < m; j++) {
                if (j == i) continue;
                uf.unite(as[j], bs[j]);
            }
            if (!uf.same(as[i], bs[i])) {
                ss.insert(as[i]);
                ss.insert(bs[i]);
                bridges.emplace_back(as[i], bs[i]);
            }
        }
        MaximalIndependentSetForest mis(bridges);
        return n - ss.size() + mis.solve();
    }
};

int main() {
    MaxCutFree solution;
    int n = 1000;
    vector<int> as, bs;
    cout << solution.solve(n, as, bs) << endl;
}
