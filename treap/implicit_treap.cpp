// T0: 元の配列のモノイド
// T1: T0に対する作用素モノイド
template <class T0, class T1>
class BaseImplicitTreap {
    // T0上の演算、単位元
    virtual T0 f0(T0, T0) = 0;
    const T0 u0;
    // T1上の演算、単位元
    virtual T1 f1(T1, T1) = 0;
    const T1 u1;
    // T0に対するT1の作用
    virtual T0 g(T0, T1) = 0;
    // 多数のt1(T1)に対するf1の合成
    virtual T1 p(T1, int) = 0;

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
    } rnd;

    struct Node {
        T0 value, acc;
        T1 lazy;
        int priority, cnt;
        bool rev;
        Node *l, *r;

        Node(T0 value_, int priority_, T0 u0_, T1 u1_)
            : value(value_), acc(u0_), lazy(u1_), priority(priority_), cnt(1), rev(false), l(nullptr), r(nullptr) {}
    } *root = nullptr;

    using Tree = Node *;

    int cnt(Tree t) { return t ? t->cnt : 0; }

    T0 acc(Tree t) { return t ? t->acc : u0; }

    void update_cnt(Tree t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
    }

    void update_acc(Tree t) {
        if (t) {
            t->acc = f0(acc(t->l), f0(t->value, acc(t->r)));
        }
    }

    void pushup(Tree t) { update_cnt(t), update_acc(t); }

    void pushdown(Tree t) {
        if (t && t->rev) {
            t->rev = false;
            swap(t->l, t->r);
            if (t->l) t->l->rev ^= 1;
            if (t->r) t->r->rev ^= 1;
        }
        if (t && t->lazy != u1) {
            if (t->l) {
                t->l->lazy = f1(t->l->lazy, t->lazy);
                t->l->acc = g(t->l->acc, p(t->lazy, cnt(t->l)));
            }
            if (t->r) {
                t->r->lazy = f1(t->r->lazy, t->lazy);
                t->r->acc = g(t->r->acc, p(t->lazy, cnt(t->r)));
            }
            t->value = g(t->value, p(t->lazy, 1));
            t->lazy = u1;
        }
        pushup(t);
    }

    void split(Tree t, int key, Tree &l, Tree &r) {
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

    void insert(Tree &t, int key, Tree item) {
        Tree t1, t2;
        split(t, key, t1, t2);
        merge(t1, t1, item);
        merge(t, t1, t2);
    }

    void merge(Tree &t, Tree l, Tree r) {
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

    void erase(Tree &t, int key) {
        Tree t1, t2, t3;
        split(t, key + 1, t1, t2);
        split(t1, key, t1, t3);
        merge(t, t1, t2);
    }

    void update(Tree t, int l, int r, T1 x) {
        if (l >= r) return;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        t2->lazy = f1(t2->lazy, x);
        t2->acc = g(t2->acc, p(x, cnt(t2)));
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    T0 query(Tree t, int l, int r) {
        if (l == r) return u0;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        T0 ret = t2->acc;
        merge(t2, t2, t3);
        merge(t, t1, t2);
        return ret;
    }

    // [l, r)の中で左から何番目か
    int find(Tree t, T0 x, int offset, bool left = true) {
        if (f0(t->acc, x) == x) {
            return -1;
        } else {
            if (left) {
                if (t->l && f0(t->l->acc, x) != x) {
                    return find(t->l, x, offset, left);
                } else {
                    return (f0(t->value, x) != x) ? offset + cnt(t->l) : find(t->r, x, offset + cnt(t->l) + 1, left);
                }
            } else {
                if (t->r && f0(t->r->acc, x) != x) {
                    return find(t->r, x, offset + cnt(t->l) + 1, left);
                } else {
                    return (f0(t->value, x) != x) ? offset + cnt(t->l) : find(t->l, x, offset, left);
                }
            }
        }
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

    // [l, r)の先頭がmになるようにシフトさせる。std::rotateと同じ仕様
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
    BaseImplicitTreap(T0 u0_, T1 u1_) : u0(u0_), u1(u1_) {}

    void set_by_vector(const vector<T0> &a) {
        for (int i = 0; i < a.size(); i++) {
            insert(i, a[i]);
        }
    }

    int size() { return cnt(root); }

    void insert(int pos, T0 x) { insert(root, pos, new Node(x, rnd.random(), u0, u1)); }

    void update(int l, int r, T1 x) { update(root, l, r, x); }

    T0 query(int l, int r) { return query(root, l, r); }

    // 二分探索。[l, r)内のkでf0(tr[k], x) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    int binary_search(int l, int r, T0 x, bool left = true) {
        if (l >= r) return -1;
        Tree t1, t2, t3;
        split(root, l, t1, t2);
        split(t2, r - l, t2, t3);
        int ret = find(t2, x, l, left);
        merge(t2, t2, t3);
        merge(root, t1, t2);
        return ret;
    }

    void erase(int pos) { erase(root, pos); }

    void reverse(int l, int r) { reverse(root, l, r); }

    void rotate(int l, int m, int r) { rotate(root, l, m, r); }

    void dump() {
        dump(root);
        cout << endl;
    }

    T0 operator[](int pos) { return query(pos, pos + 1); }
};

template <class T0, class T1>
struct MinUpdateQuery : public BaseImplicitTreap<T0, T1> {
    using BaseImplicitTreap<T0, T1>::BaseImplicitTreap;
    MinUpdateQuery() : MinUpdateQuery(numeric_limits<T0>::max(), numeric_limits<T1>::min()) {}
    T0 f0(T0 x, T0 y) override { return min(x, y); }
    T1 f1(T1 x, T1 y) override { return y == numeric_limits<T1>::min() ? x : y; }
    T0 g(T0 x, T1 y) override { return y == numeric_limits<T1>::min() ? x : y; }
    T1 p(T1 x, int len) override { return x; }
};

template <class T0, class T1>
struct SumAddQuery : public BaseImplicitTreap<T0, T1> {
    using BaseImplicitTreap<T0, T1>::BaseImplicitTreap;
    SumAddQuery() : SumAddQuery(0, 0) {}
    T0 f0(T0 x, T0 y) override { return x + y; }
    T1 f1(T1 x, T1 y) override { return x + y; }
    T0 g(T0 x, T1 y) override { return x + y; }
    T1 p(T1 x, int len) override { return x * len; }
};

template <class T0, class T1>
struct MinAddQuery : public BaseImplicitTreap<T0, T1> {
    using BaseImplicitTreap<T0, T1>::BaseImplicitTreap;
    MinAddQuery() : MinAddQuery(numeric_limits<T0>::max(), 0) {}
    T0 f0(T0 x, T0 y) override { return min(x, y); }
    T1 f1(T1 x, T1 y) override { return x + y; }
    T0 g(T0 x, T1 y) override { return x + y; }
    T1 p(T1 x, int len) override { return x; }
};

template <class T0, class T1>
struct SumUpdateQuery : public BaseImplicitTreap<T0, T1> {
    using BaseImplicitTreap<T0, T1>::BaseImplicitTreap;
    SumUpdateQuery() : SumUpdateQuery(0, numeric_limits<T1>::min()) {}
    T0 f0(T0 x, T0 y) override { return x + y; }
    T1 f1(T1 x, T1 y) override { return y == numeric_limits<T1>::min() ? x : y; }
    T0 g(T0 x, T1 y) override { return y == numeric_limits<T1>::min() ? x : y; }
    T1 p(T1 x, int len) override { return x == numeric_limits<T1>::min() ? numeric_limits<T1>::min() : x * len; }
};

template <class T0>
struct SumAffineQuery : public BaseImplicitTreap<T0, pair<T0, T0>> {
    using T1 = pair<T0, T0>;  // first * x + second
    using BaseImplicitTreap<T0, T1>::BaseImplicitTreap;
    SumAffineQuery() : SumAffineQuery(0, {1, 0}) {}
    T0 f0(T0 x, T0 y) override { return x + y; }
    T1 f1(T1 x, T1 y) override { return {x.first * y.first, x.second * y.first + y.second}; }
    T0 g(T0 x, T1 y) override { return y.first * x + y.second; }
    T1 p(T1 x, int len) override { return {x.first, x.second * len}; }
    // update(i, j, {a, b}); // [i, j)にax + bを作用
    // update(i, j, {0, a}); // update
    // update(i, j, {1, a}); // 加算
    // update(i, j, {a, 0}); // 倍
};

template <class T>
struct MinmaxAffineQuery : public BaseImplicitTreap<pair<T, T>, pair<T, T>> {
    using T0 = pair<T, T>;  // {min, max}
    using T1 = pair<T, T>;  // first * x + second
    using BaseImplicitTreap<T0, T1>::BaseImplicitTreap;
    MinmaxAffineQuery()
        : MinmaxAffineQuery({numeric_limits<T>::max(), -numeric_limits<T>::max()}, {1, 0}) {
    }  // TODO: _u1を使うとコンパイル通らない原因不明
    T0 f0(T0 x, T0 y) override { return {min(x.first, y.first), max(x.second, y.second)}; }
    T1 f1(T1 x, T1 y) override { return {x.first * y.first, x.second * y.first + y.second}; }
    T0 g(T0 x, T1 y) override {
        T0 ret = {x.first * y.first + y.second, x.second * y.first + y.second};
        if (y.first < 0) swap(ret.first, ret.second);
        return ret;
    }
    T1 p(T1 x, int len) override { return x; }
    // update(i, j, {a, b}); // [i, j)にax + bを作用
    // update(i, j, {0, a}); // update
    // update(i, j, {1, a}); // 加算
    // update(i, j, {a, 0}); // 倍
};