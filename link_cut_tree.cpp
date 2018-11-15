#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template <class T, class T2 = T>
struct LinkCutTree {
    using F = function<T(T, T)>;
    using G = function<T(T, T2, int)>;
    using H = function<T2(T2, T2)>;

    struct Node {
        Node *l, *r, *p;
        int idx;
        T key, sum;
        T2 lazy;

        bool rev;
        int sz;

        bool is_root() {
            return !p || (p->l != this && p->r != this);
        }

        Node(int idx, const T &key, const T2 &v) : idx(idx), key(key), sum(key), lazy(v), sz(1), l(nullptr), r(nullptr), p(nullptr), rev(false) {}
    };

    const T e;
    const T2 e2;
    const F f;
    const G g;
    const H h;

    LinkCutTree(const F &f, const T &e) : LinkCutTree(f, G(), H(), e, T2()) {}
    LinkCutTree(const F &f, const G &g, const H &h, const T &e, const T2 &e2) : f(f), g(g), h(h), e(e), e2(e2) {}

    Node *make_node(int idx, const T &v = T()) {
        return new Node(idx, v, e2);
    }

    void propagate(Node *t, const T2 &x) {
        t->lazy = h(t->lazy, x);
        t->key = g(t->key, x, 1);
        t->sum = g(t->sum, x, t->sz);
    }

    void toggle(Node *t) {
        swap(t->l, t->r);    
        t->rev ^= true;
    }

    void push(Node *t) {
        if (t->lazy != e2) {
            if (t->l) propagate(t->l, t->lazy);
            if (t->r) propagate(t->r, t->lazy);
            t->lazy = e2;
        }
        if (t->rev) {
            if (t->l) toggle(t->l);
            if (t->r) toggle(t->r);
            t->rev = false;
        }
    }

    void update(Node *t) {
        t->sz = 1;
        t->sum = t->key;
        if (t->l) t->sz += t->l->sz, t->sum = f(t->l->sum, t->sum);
        if (t->r) t->sz += t->r->sz, t->sum = f(t->sum, t->r->sum);
    }

    void rotr(Node *t) {
        auto *x = t->p, *y = x->p;
        if (x->l = t->r) t->r->p = x;
        t->r = x, x->p = t;
        update(x), update(t);
        if (t->p = y) {
            if (y->l == x) y->l = t;
            if (y->r == x) y->r = t;
            update(y);
        }
    }

    void rotl(Node *t) {
        auto *x = t->p, *y = x->p;
        if (x->r = t->l) t->l->p = x;
        t->l = x, x->p = t;
        update(x), update(t);
        if (t->p = y) {
            if (y->l == x) y->l = t;
            if (y->r == x) y->r = t;
            update(y);
        }
    }

    void splay(Node *t) {
        push(t);
        while (!t->is_root()) {
            auto *q = t->p;
            if (q->is_root()) {
                push(q), push(t);
                if (q->l == t) rotr(t);
                else rotl(t);
            } else {
                auto *r = q->p;
                push(r), push(q), push(t);
                if (r->l == q) {
                    if (q->l == t) rotr(q), rotr(t);
                    else rotl(t), rotr(t);
                } else {
                    if (q->r == t) rotl(q), rotl(t);
                    else rotr(t), rotl(t);
                }
            }
        }
    }

    Node *expose(Node *t) {
        Node *rp = nullptr;
        for (Node *cur = t; cur; cur = cur->p) {
            splay(cur);
            cur->r = rp;
            update(cur);
            rp = cur;
        }
        splay(t);
        return rp;
    }

    void link(Node *child, Node *parent) {
        expose(child);
        expose(parent);
        child->p = parent;
        parent->r = child;
    }

    void cut(Node *child) {
        expose(child);
        auto *parent = child->l;
        child->l = nullptr;
        parent->p = nullptr;
    }

    void evert(Node *t) {
        expose(t);
        toggle(t);
        push(t);
    }

    Node *lca(Node *u, Node *v) {
        expose(u);
        return expose(v);
    }

    vi getpath(Node *x) {
        vi vs;
        function<void(Node *)> dfs = [&](Node *cur) {
            if (!cur) return;
            push(cur);
            dfs(cur->r);
            vs.push_back(cur->idx);
            dfs(cur->l);
        };
        expose(x);
        dfs(x);
        return vs;
    }

    void set_propagate(Node *t, const T2 &x) {
        expose(t);
        propagate(t, x);
        push(t);
    }
};

int main() {
    int n;
    cin >> n;
    LinkCutTree<i64> lct([](i64 a, i64 b) {return a + b;}, 0);
    vector<LinkCutTree<i64>::Node*> nodes(n);
    for (int i = 0; i < n; i++) {
        nodes[i] = lct.make_node(i);
    }
    for (int i = 0; i < n; i++) {
        int d;
        cin >> d;
        for (int j = 0; j < d; j++) {
            int k;
            cin >> k;
            lct.link(nodes[k], nodes[i]);
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int v, w;
            cin >> v >> w;
            lct.expose(nodes[v]);
            nodes[v]->key += w;
        } else {
            int u;
            cin >> u;
            lct.expose(nodes[u]);
            cout << nodes[u]->sum << endl;
        }
    }
}