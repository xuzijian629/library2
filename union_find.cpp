// MOVED
// added

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

class Solution {
    int n;
    int id(int i, int j, int k) {
        return 4 * (i * n + j) + k;
    }
public:
    int regionsBySlashes(vector<string>& grid) {
        n = grid.size();
        UnionFind uf(n * n * 4);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 1; j++) {
                uf.unite(id(i, j, 3), id(i, j + 1, 1));
            }
        }
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n; j++) {
                uf.unite(id(i, j, 2), id(i + 1, j, 0));
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '/') {
                    uf.unite(id(i, j, 0), id(i, j, 1));
                    uf.unite(id(i, j, 2), id(i, j, 3));
                } else if (grid[i][j] == '\\') {
                    uf.unite(id(i, j, 0), id(i, j, 3));
                    uf.unite(id(i, j, 1), id(i, j, 2));
                } else {
                    uf.unite(id(i, j, 0), id(i, j, 1));
                    uf.unite(id(i, j, 1), id(i, j, 2));
                    uf.unite(id(i, j, 2), id(i, j, 3));
                }
            }
        }
        return uf.cnt;
    }
};

int main() {
    Solution hoge;
    vector<string> as = {" /","/ "};
    cout << hoge.regionsBySlashes(as) << endl;
}