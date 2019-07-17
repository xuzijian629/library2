// added

#include <bits/stdc++.h>

using namespace std;

// 一点更新、区間畳み込み
class PersistentSegTree {
    struct Node {
        int data;
        Node *l, *r;

        Node(const int &data) : data(data) {}
    };

    int sz;
    function<int(int, int)> f;
    int unit;

    PersistentSegTree(function<int(int, int)> f, int unit) : f(move(f)), unit(unit) {}

    Node *build(vector<int> &v) {
        sz = int(v.size());
        return build(0, int(v.size()), v);
    }

    Node *merge(Node *l, Node *r) {
        auto t = new Node(f(l->data, r->data));
        t->l = l;
        t->r = r;
        return t;
    }

    Node *build(int l, int r, vector<int> &v) {
        if (l + 1 >= r) return new Node(v[l]);
        return merge(build(l, (l + r) >> 1, v), build((l + r) >> 1, r, v));
    }

    Node *update(int a, int &x, Node *k, int l, int r) {
        if (r <= a || a + 1 <= l) {
            return k;
        } else if (a <= l && r <= a + 1) {
            return new Node(x);
        } else {
            return merge(update(a, x, k->l, l, (l + r) >> 1), update(a, x, k->r, (l + r) >> 1, r));
        }
    }

    Node *update(Node *t, int k, int &x) {
        return update(k, x, t, 0, sz);
    }

    int query(int a, int b, Node *k, int l, int r) {
        if (r <= a || b <= l || !k) {
            return unit;
        } else if (a <= l && r <= b) {
            return k->data;
        } else {
            return f(query(a, b, k->l, l, (l + r) >> 1), query(a, b, k->r, (l + r) >> 1, r));
        }
    }

    int query(Node *t, int a, int b) {
        return query(a, b, t, 0, sz);
    }
};

int main() {

}