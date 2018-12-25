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

using T = i64;

struct MinMonoid {
    static constexpr T id() {
        return 2e18;
    }
    static T op(T a, T b) {
        return min(a, b);
    }
};

struct UpdateMonoid {
    static constexpr T id() {
        return 2e18;
    }
    static T op(T a, T b) {
        return b;
    }
};

struct Modifier {
    // lazyの結果によってaccがどう変わるか。szは部分木のサイズ
    static T op(T a, T b, int sz) {
        return (b == UpdateMonoid::id()) ? a : b;
    }
};

template<class Monoid, class OperatorMonoid>
class ImplicitTreap {
    xorshift rnd;
    struct Node {
        T value, acc, lazy;
        int priority, cnt;
        bool rev;
        Node *l, *r;
        Node(T value, int priority) : value(value), acc(Monoid::id()), lazy(OperatorMonoid::id()), priority(priority), cnt(1), rev(false), l(nullptr), r(nullptr) {}
    } *root = nullptr;
    using Tree = Node *;

    int cnt(Tree t) {
        return t ? t->cnt : 0;
    }

    T acc(Tree t) {
        return t ? t->acc : Monoid::id();
    }

    void update_cnt(Tree t) {
        if (t) {
            t->cnt = 1 + cnt(t->l) + cnt(t->r);
        }
    }

    void update_acc(Tree t) {
        if (t) {
            t->acc = Monoid::op(acc(t->l), Monoid::op(t->value, acc(t->r)));
        }
    }

    void pushup(Tree t) {
        update_cnt(t), update_acc(t);
    }

    void pushdown(Tree t) {
        if (t && t->rev) {
            t->rev = false;
            swap(t->l, t->r);
            if (t->l) t->l->rev ^= 1;
            if (t->r) t->r->rev ^= 1;
        }
        if (t && t->lazy != OperatorMonoid::id()) {
            if (t->l) {
                t->l->lazy = OperatorMonoid::op(t->l->lazy, t->lazy);
                t->l->acc = Modifier::op(t->l->acc, t->lazy, cnt(t->l));
            }
            if (t->r) {
                t->r->lazy = OperatorMonoid::op(t->r->lazy, t->lazy);
                t->r->acc = Modifier::op(t->r->acc, t->lazy, cnt(t->r));
            }
            t->value = Modifier::op(t->value, t->lazy, 1);
            t->lazy = OperatorMonoid::id();
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

    void update(Tree t, int l, int r, T x) {
        if (l >= r) return;
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2 , t3);
        t2->lazy = OperatorMonoid::op(t2->lazy, x);
        t2->acc = Modifier::op(t2->acc, x, cnt(t2));
        merge(t2, t2, t3);
        merge(t, t1, t2);
    }

    T query(Tree t, int l, int r) {
        Tree t1, t2, t3;
        split(t, l, t1, t2);
        split(t2, r - l, t2, t3);
        T ret = t2->acc;
        merge(t2, t2, t3);
        merge(t, t1, t2);
        return ret;
    }

    // [l, r)の中で左から何番目か
    int find(Tree t, T x, int offset, bool left = true) {
        if (Monoid::op(t->acc, x) == x) {
            return -1;
        } else {
            if (left) {
                if (t->l && Monoid::op(t->l->acc, x) != x) {
                    return find(t->l, x, offset, left);
                } else {
                    return (Monoid::op(t->value, x) != x) ? offset + cnt(t->l) : find(t->r, x, offset + cnt(t->l) + 1, left);
                }
            } else {
                if (t->r && Monoid::op(t->r->acc, x) != x) {
                    return find(t->r, x, offset + cnt(t->l) + 1, left);
                } else {
                    return (Monoid::op(t->value, x) != x) ? offset + cnt(t->l) : find(t->l, x, offset, left);
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
    ImplicitTreap() {}
    ImplicitTreap(vector<T> as) {
        ::reverse(as.begin(), as.end());
        for (T a : as) {
            insert(0, a);
        }
    }

    int size() {
        return cnt(root);
    }

    void insert(int pos, T x) {
        insert(root, pos, new Node(x, rnd.random()));
    }

    void update(int l, int r, T x) {
        update(root, l, r, x);
    }

    T query(int l, int r) {
        return query(root, l, r);
    }

    // 二分探索。[l, r)内のkでMonoid::op(tr[k], x) != xとなる最左/最右のもの。存在しない場合は-1
    // たとえばMinMonoidの場合、x未満の最左/最右の要素の位置を返す
    int binary_search(int l, int r, T x, bool left = true) {
        Tree t1, t2, t3;
        split(root, l, t1, t2);
        split(t2, r - l, t2, t3);
        int ret = find(t2, x, l, left);
        merge(t2, t2, t3);
        merge(root, t1, t2);
        return ret;
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

    T operator[](int pos) {
        return query(pos, pos + 1);
    }
};

struct Q {
    int type, t, a, b, ans;
};

int main() {
    // assert(freopen("/Users/xuzijian/atcoder/paken/input", "r", stdin));
    // assert(freopen("/Users/xuzijian/atcoder/paken/output", "w", stdout));
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n;
    cin >> n;
    string s;
    cin >> s;
    ImplicitTreap<MinMonoid, UpdateMonoid> tr, tr2;
    for (int i = 0; i < n; i++) {
        tr.insert(i, s[i] == 'W');
        tr2.insert(i, -int(s[i] == 'W'));
    }

    vector<Q> qs;
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        char a, b;
        cin >> a >> b;
        qs.push_back({0, i, int(a == 'R'), int(b == 'W'), -1});
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        qs.push_back({1, a, b, i, -1});
    }

    sort(qs.begin(), qs.end(), [](Q a, Q b) {
        return a.t == b.t ? a.type < b.type : a.t < b.t;
    });

    auto leftmost_black = [&tr]() {
        return tr.binary_search(0, tr.size(), 1);
    };
    auto rightmost_black = [&tr]() {
        return tr.binary_search(0, tr.size(), 1, false);
    };
    auto leftmost_white = [&tr2]() {
        return tr2.binary_search(0, tr2.size(), 0);
    };
    auto rightmost_white = [&tr2]() {
        return tr2.binary_search(0, tr2.size(), 0, false);
    };

    for (int i = 0; i < qs.size(); i++) {
        if (qs[i].type == 0) {
            // put
            if (qs[i].b == 0) {
                // put black
                if (qs[i].a == 0) {
                    // left
                    int k = leftmost_black();
                    if (k != -1) {
                        tr.update(0, k, 0);
                        tr2.update(0, k, 0);
                    }
                    tr.insert(0, 0);
                    tr2.insert(0, 0);
                } else {
                    int k = rightmost_black();
                    if (k != -1) {
                        tr.update(k, tr.size(), 0);
                        tr2.update(k, tr2.size(), 0);
                    }
                    tr.insert(tr.size(), 0);
                    tr2.insert(tr2.size(), 0);
                }
            } else {
                // put white
                if (qs[i].a == 0) {
                    // left
                    int k = leftmost_white();
                    if (k != -1) {
                        tr.update(0, k, 1);
                        tr2.update(0, k, -1);
                    }
                    tr.insert(0, 1);
                    tr2.insert(0, -1);
                } else {
                    int k = rightmost_white();
                    if (k != -1) {
                        tr.update(k, tr.size(), 1);
                        tr2.update(k, tr2.size(), -1);
                    }
                    tr.insert(tr.size(), 1);
                    tr2.insert(tr2.size(), -1);
                }
            }
            // cout << "turn " << i << endl;
            // tr.dump();
        } else {
            // query
            qs[i].ans = tr[qs[i].a];
        }
    }
    sort(qs.begin(), qs.end(), [](Q a, Q b) {
        return a.type == b.type ? a.b < b.b : a.type > b.type;
    });

    for (int i = 0; i < q; i++) {
        cout << (qs[i].ans ? 'W' : 'B') << endl;
    }
}