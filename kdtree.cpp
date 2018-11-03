#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
#pragma GCC optimize("O3")

using ii = pair<i64, i64>;
// Assuming all points are random. Otherwise, shuffle them!
class KDTree {
    struct node {
        ii p;
        node *l, *r;
        node(const ii& p) : p(p), l(nullptr), r(nullptr) {}
    } *root;
    
    function<bool(int, ii, ii)> compare = [](int d, ii p, ii q) {
        return d ? p.first < q.first : p.second < q.second;
    };

    node *insert(node *t, int d, const ii &p) {
        if (t == nullptr) return new node(p);
        if (compare(d, p, t->p)) {
            t->l = insert(t->l, !d, p);
        } else {
            t->r = insert(t->r, !d, p);
        }
        return t;
    }

    void search(node *t, int d, const ii &ld, const ii &ru, vector<ii> &out) {
        if (t == nullptr) return;
        const ii &p = t->p;
        if (ld.first <= p.first && p.first <= ru.first && ld.second <= p.second && p.second <= ru.second) {
            out.push_back(p);
        }
        if (!compare(d, p, ld)) {
            search(t->l, !d, ld, ru, out);
        }
        if (!compare(d, ru, p)) {
            search(t->r, !d, ld, ru, out);
        }
    }

public:
    KDTree() : root(nullptr) {};

    void insert(const ii &p) {
        root = insert(root, 0, p);
    }

    // including border
    vector<ii> search(const ii &ld, const ii &ru) {
        vector<ii> ret;
        search(root, 0, ld, ru, ret);
        return ret;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout.setf(ios::fixed);
    cout.precision(10);
    int n;
    cin >> n;
    KDTree kdtree;
    unordered_map<i64, int> id;
    vector<ii> ps;
    for (int i = 0; i < n; i++) {
        i64 x, y;
        cin >> x >> y;
        ps.push_back(ii(x, y));
        id[x * i64(1e9) + y] = i;
    }
    random_shuffle(ps.begin(), ps.end());
    for (ii& p: ps) {
        kdtree.insert(p);
    }
    int q;
    cin >> q;
    while (q--) {
        i64 sx, tx, sy, ty;
        cin >> sx >> tx >> sy >> ty;
        vector<ii> ret = kdtree.search(ii(sx, sy), ii(tx, ty));
        vi tmp;
        for (ii& p: ret) {
            tmp.push_back(id[p.first * i64(1e9) + p.second]);
        }
        sort(tmp.begin(), tmp.end());
        for (int t: tmp) {
            cout << t << '\n';
        }
        cout << '\n';
    }
}