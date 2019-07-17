// hoge
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template <class T, class T2>
class LazySegTree {
    using F = function<T(T, T)>;
    using G = function<T(T, T2)>;
    using H = function<T2(T2, T2)>;
    using P = function<T2(T2, int)>;
    int n;
    vector<T> data;
    vector<T2> lazy;
    const F f;
    const G g;
    const H h;
    const P p;
    const T e;
    const T2 e2;

public:
    LazySegTree(const vector<T>& as, const F f, const G g, const H h, const P p, const T& e, const T2 e2) : f(f), g(g), h(h), p(p), e(e), e2(e2) {
        n = 1;
        while (n < as.size()) n <<= 1;
        data.assign(2 * n, e);
        lazy.assign(2 * n, e2);
        for (int i = 0; i < as.size(); i++) {
            data[n + i] = as[i];
        }
        for (int i = n - 1; i > 0; i--) {
            data[i] = f(data[2 * i], data[2 * i + 1]);
        }
    }

    void propagate(int k, int len) {
        if (lazy[k] != e2) {
            if (k < n) {
                lazy[2 * k] = h(lazy[2 * k], lazy[k]);
                lazy[2 * k + 1] = h(lazy[2 * k + 1], lazy[k]);
            }
            data[k] = g(data[k], p(lazy[k], len));
            lazy[k] = e2;
        }
    }

    T update(int a, int b, const T2& x, int k, int l, int r) {
        propagate(k, r - l);
        if (r <= a || b <= l) {
            return data[k];
        } else if (a <= l && r <= b) {
            lazy[k] = h(lazy[k], x);
            propagate(k, r - l);
            return data[k];
        } else {
            return data[k] = f(update(a, b, x, 2 * k, l, (l + r) >> 1), update(a, b, x, 2 * k + 1, (l + r) >> 1, r));
        }
    }

    T update(int a, int b, const T2& x) {
        return update(a, b, x, 1, 0, n);
    }

    T query(int a, int b, int k, int l, int r) {
        propagate(k, r - l);
        if (r <= a || b <= l) {
            return e;
        } else if (a <= l && r <= b) {
            return data[k];
        } else {
            return f(query(a, b, 2 * k, l, (l + r) >> 1), query(a, b, 2 * k + 1, (l + r) >> 1, r));
        }
    }

    T query(int a, int b) {
        return query(a, b, 1, 0, n);
    }
};

int main() {
    int n;
    cin >> n;
    vi as(n);

    // RUQ, RMQ
    // auto f = [](i64 a, i64 b) {return min(a, b);};
    // auto g = [](i64 a, i64 b) {return b < 0 ? a : b;};
    // auto h = [](i64 a, i64 b) {return b < 0 ? a : b;};
    // auto p = [](i64 a, int b) {return a;};
    // LazySegTree<i64, i64> tree(as, f, g, h, p, 1e18, -1);

    // RAQ, RMQ
    // auto f = [](i64 a, i64 b) {return min(a, b);};
    // auto g = [](i64 a, i64 b) {return a + b;};
    // auto h = [](i64 a, i64 b) {return a + b;};
    // auto p = [](i64 a, i64 b) {return a;};
    // LazySegTree<i64, i64> tree(as, f, g, h, p, 1e18, 0);

    // RUQ, RSQ
    const i64 INF = 1e18;
    auto f = [](i64 a, i64 b) {return a + b;};
    auto g = [](i64 a, i64 b) {return b == INF ? a : b;};
    auto h = [](i64 a, i64 b) {return b == INF ? a : b;};
    auto p = [](i64 a, i64 b) {return a == INF ? INF : a * b;};
    LazySegTree<i64, i64> tree(as, f, g, h, p, 0, INF);

    int q;
    cin >> q;
    while (q--) {
        int c;
        cin >> c;
        if (c == 0) {
            int s, t, x;
            cin >> s >> t >> x;
            tree.update(s, t + 1, x);
        } else {
            int s, t;
            cin >> s >> t; 
            cout << tree.query(s, t + 1) << endl;
        }
    }
}