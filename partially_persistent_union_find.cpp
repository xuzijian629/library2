// added
#include <bits/stdc++.h>

using namespace std;

class PartiallyParsistentUnionFind {
    vector<int> rank, par, time;
    const int INF = 1e9;
public:
    PartiallyParsistentUnionFind(int n) {
        rank.resize(n);
        par.resize(n, -1);
        time.resize(n, INF);
    }

    int find(int t, int x) {
        if (time[x] > t) return x;
        return find(t, par[x]);
    }

    bool unite(int t, int x, int y) {
        x = find(t, x);
        y = find(t, y);
        if (x == y) return false;
        if (rank[x] > rank[y]) {
            par[y] = x;
            time[y] = t;
        } else {
            par[x] = y;
            time[x] = t;
            if (rank[x] == rank[y]) {
                rank[y]++;
            }
        }
        return true;
    }

    bool same(int t, int x, int y) {
        return find(t, x) == find(t, y);
    }
};

int main() {
    int n, m, q;
    cin >> n >> m >> q;
    using ii = pair<int, int>;
    map<ii, int> ord;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        ord[ii(a, b)] = 1e9;
    }
    vector<ii> qs;
    for (int i = 0; i < q; i++) {
        int c, d;
        cin >> c >> d;
        c--, d--;
        qs.emplace_back(c, d);
    }
    reverse(qs.begin(), qs.end());
    for (int i = 0; i < q; i++) {
        ord[qs[i]] = i + 1;
    }

    PartiallyParsistentUnionFind uf(n);
    vector<pair<int, ii>> vs;
    for (auto &p: ord) {
        if (p.second == 1e9) {
            uf.unite(0, p.first.first, p.first.second);
        } else {
            vs.emplace_back(p.second, p.first);
        }
    }

    sort(vs.begin(), vs.end());
    for (auto &v: vs) {
        uf.unite(v.first, v.second.first, v.second.second);
    }

    for (int i = 1; i < n; i++) {
        int l = -1, r = q + 1;
        while (l < r - 1) {
            int mid = (l + r) / 2;
            if (uf.same(mid, 0, i)) {
                r = mid;
            } else {
                l = mid;
            }
        }

        cout << (l == -1 ? -1 : (l == q ? 0 : q - l)) << endl;
    }
}

