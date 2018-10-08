#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class RangeCount {
    const int ST_SIZE = (1 << 20) - 1;
    int n;
    vi data;
    vvi segtree;

    void init(int k, int l, int r) {
        if (r - l == 1) {
            segtree[k].push_back(data[l]);
        } else {
            int lch = k * 2 + 1;
            int rch = k * 2 + 2;
            init(lch, l, (l + r) / 2);
            init(rch, (l + r) / 2, r);
            segtree[k].resize(r - l);

            merge(segtree[lch].begin(), segtree[lch].end(), segtree[rch].begin(), segtree[rch].end(), segtree[k].begin());
        }
    }

    // number of x in [i, j)
    int query(int i, int j, i64 x, int k, int l, int r) {
        if (j <= l || r <= i) {
            return 0;
        }

        if (i <= l && r <= j) {
            return upper_bound(segtree[k].begin(), segtree[k].end(), x) - segtree[k].begin();
        }

        int lc = query(i, j, x, k * 2 + 1, l, (l + r) / 2);
        int rc = query(i, j, x, k * 2 + 2, (l + r) / 2, r);
        return lc + rc;
    }

public:
    RangeCount(vi v) {
        n = v.size();
        data = vi(v);
        segtree = vvi(ST_SIZE);
        init(0, 0, n);
    }

    int exact(int i, int j, i64 x) {
        return query(i, j, x, 0, 0, n) - query(i, j, x - 1, 0, 0, n);
    }

    int le(int i, int j, i64 x) {
        return query(i, j, x, 0, 0, n);
    }

    int lt(int i, int j, i64 x) {
        return query(i, j, x - 1, 0, 0, n);
    }

    int ge(int i, int j, i64 x) {
        return query(i, j, 1e18, 0, 0, n) - query(i, j, x - 1, 0, 0, n);
    }

    int gt(int i, int j, i64 x) {
        return query(i, j, 1e18, 0, 0, n) - query(i, j, x, 0, 0, n);
    }
};

int main() {
    int n;
    cin >> n;
    vi vs(n);
    i64 ans = 0;
    for (int i = 0; i < n; i++) {
        cin >> vs[i];
        vs[i]--;
    }
    RangeCount cnt(vs);

    for (int i = 0; i < n; i++) {
        if (vs[i] >= i) {
            ans += cnt.ge(0, i, vs[i]);
            ans += cnt.le(i, vs[i] + 1, vs[i]) - 1;
            ans += cnt.le(vs[i] + 1, n + 1, vs[i]);
        } else {
            ans += cnt.ge(0, vs[i], vs[i]);
            ans += cnt.ge(vs[i], i + 1, vs[i]) - 1;
            ans += cnt.le(i + 1, n + 1, vs[i]);
        }
    }
    cout << ans / 2 << endl;
}

