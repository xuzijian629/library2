#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;

// 一点更新、区間畳み込み
class PersistentSegTree {
    struct Node {
        i64 data;
        Node *l, *r;

        Node (const i64& data) : data(data) {}
    };

    int sz;
    function<i64(i64, i64)> f;
    i64 unit;

    PersistentSegTree(function<i64(i64, i64)> f, i64 unit) : f(f), unit(unit) {}

    Node* build(vi& v) {
        sz = int(v.size());
        return build(0, int(v.size()), v);
    }

    Node* merge(Node* l, Node* r) {
        auto t = new Node(f(l->data, r->data));
        t->l = l;
        t->r = r;
        return t;
    }

    Node* build(int l, int r, vi& v) {
        if (l + 1 >= r) return new Node(v[l]);
        return merge(build(l, (l + r) >> 1, v), build((l + r) >> 1, r, v));
    }

    Node* update(int a, i64& x, Node* k, int l, int r) {
        if (r <= a || a + 1 <= l) {
            return k;
        } else if (a <= l && r <= a + 1) {
            return new Node(x);
        } else {
            return merge(update(a, x, k->l, l, (l + r) >> 1), update(a, x, k->r, (l + r) >> 1, r));
        }
    }

    Node* update(Node* t, int k, i64& x) {
        return update(k, x, t, 0, sz);
    }

    i64 query(int a, int b, Node* k, int l, int r) {
        if (r <= a || b <= l || !k) {
            return unit;
        } else if (a <=l && r <= b) {
            return k->data;
        } else {
            return f(query(a, b, k->l, l, (l + r) >> 1), query(a, b, k->r, (l + r) >> 1, r));
        }
    }

    i64 query(Node* t, int a, int b) {
        return query(a, b, t, 0, sz);
    }
};

int main() {

}