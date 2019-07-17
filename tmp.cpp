// iran

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
#define endl '\n'

class xorshift {
    uint64_t x;
public:
    xorshift() {
        mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
        x = rnd();
        for (int i = 0; i < 100; i++) {
            random();
        }
    }
    uint64_t random() {
        x = x ^ (x << 7);
        return x = x ^ (x >> 9);
    }
};

// for RMQ & RAQ
constexpr int INF = __INT_MAX__;
class ImplicitTreap {
    xorshift rnd;
    struct Node {
        int value, min, lazy;
        int priority, cnt;
        bool rev;
        Node *l, *r;
        Node(int value, int priority) : value(value), min(INF), lazy(0), priority(priority), cnt(1), rev(false), l(nullptr), r(nullptr) {}
    } *root = nullptr;
    using Tree = Node *;

    int cnt(Tree t) {
        return t ? t->cnt : 0;
    }

    int get_min(Tree t) {
        return t ? t->min : INF;
    }

    void update_cnt(Tree t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
    }

    void update_min(Tree t) {
        if (t) {
            t->min = min(t->value, min(get_min(t->l), get_min(t->r)));
        }
    }

    void pushup(Tree t) {
        update_cnt(t), update_min(t);
    }

    void pushdown(Tree t) {
        if (t && t->rev) {
            t->rev = false;
            swap(t->l, t->r);
            if (t->l) t->l->rev ^= 1;
            if (t->r) t->r->rev ^= 1;
        }
        if (t && t->lazy) {
            if (t->l) {
                t->l->lazy += t->lazy;
                t->l->min += t->lazy;
            }
            if (t->r) {
                t->r->lazy += t->lazy;
                t->r->min += t->lazy;
            }
            t->value += t->lazy;
            t->lazy = 0;
        }
        pushup(t);
    }
    
    void split(Tree t, int key, Tree& l, Tree& r) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        pushdown(t);
        int implicit_key = cnt(t->l) + 1;
        if (key < implicit_key) {
            split(t->l, key, l, t->l), r = t;
        } else {
            split(t->r, key - implicit_key, t->r, r), l = t;
        }
        pushup(t);
    }
    
    void insert(Tree& t, int key, Tree item) {
        Tree t1, t2;
        split(t, key, t1, t2);
        merge(t1, t1, item);
        merge(t, t1, t2);
    }

    void merge(Tree& t, Tree l, Tree r) {
        pushdown(l);
        pushdown(r);
        if (!l || !r) {
            t = l ? l : r;
        } else if (l->priority > r->priority) {
            merge(l->r, l->r, r), t = l;
        } else {
            merge(r->l, l, r->l), t = r;
        }
        pushup(t);
    }
    
    void erase(Tree& t, int key) {
        Tree t1, t2, t3;
        split(t, key + 1, t1, t2);
        split(t1, key, t1, t3);
        merge(t, t1, t2);
    }

    void add(Tree t, int l, int r, int x) {
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2 , t3);
        t2->lazy += x;
        t2->min += x;
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    int findmin(Tree t, int l, int r) {
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        int ret = t2->min;
        merge(t2, t2, t3);
        merge(t, t1, t2);
        return ret;
    }

    void reverse(Tree t, int l, int r) {
        if (l > r) return;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        t2->rev ^= 1;
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    // [l, r)の先頭がmになるように左シフトさせる。std::rotateと同じ仕様
    void rotate(Tree t, int l, int m, int r) {
        reverse(t, l, r);
        reverse(t, l, l + r - m);
        reverse(t, l + r - m, r);
    }

    void dump(Tree t) {
        if (!t) return;
        pushdown(t);
        dump(t->l);
        cout << t->value << " ";
        dump(t->r);
    }
    
public:
    void insert(int pos, int x) {
        insert(root, pos, new Node(x, rnd.random()));
    }

    void add(int l, int r, int x) {
        add(root, l, r, x);
    }

    int findmin(int l, int r) {
        return findmin(root, l, r);
    }

    void erase(int pos) {
        erase(root, pos);
    }

    void reverse(int l, int r) {
        reverse(root, l, r);
    }

    void rotate(int l, int m, int r) {
        rotate(root, l, m, r);
    }

    void dump() {
        dump(root);
        cout << endl;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n, q;
    cin >> n >> q;
    ImplicitTreap tr;
    for (int i = 0; i < n; i++) {
        tr.insert(0, 0);
    }

    while (q--) {
        // tr.dump();
        int type;
        cin >> type;
        if (type) {
            int s, t;
            cin >> s >> t;
            cout << tr.findmin(s, t + 1) << endl;
        } else {
            int s, t, x;
            cin >> s >> t >> x;
            tr.add(s, t + 1, x);
        }
    }
}

