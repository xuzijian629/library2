#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template <class T>
class SegTree {
    using F = function<T(T, T)>;
    int n;
    vector<T> data;
    const F f;
    const T e;

public:
    SegTree(const vector<T>& as, const F f, const T& e) : f(f), e(e) {
        n = 1;
        while (n < as.size()) n <<= 1;
        data.assign(2 * n, e);
        for (int i = 0; i < as.size(); i++) {
            data[n + i] = as[i];
        }
        for (int i = n - 1; i > 0; i--) {
            data[i] = f(data[2 * i], data[2 * i + 1]);
        }
    }

    void update(int k, const T& x) {
        k += n;
        data[k] = x;
        while (k >>= 1) {
            data[k] = f(data[2 * k], data[2 * k + 1]);
        }
    }

    T query(int a, int b) {
        T L = e, R = e;
        for (a += n, b += n; a < b; a >>= 1, b >>= 1) {
            if (a & 1) {
                L = f(L, data[a++]);
            }
            if (b & 1) {
                R = f(data[--b], R);
            }
        }
        return f(L, R);
    }
};

int main() {
    int n, d, k;
    cin >> n >> d >> k;
    vi as(n);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }

    i64 nax = 0, j;
    SegTree<i64> segtree(as, [](i64 a, i64 b) {return max(a, b);}, -1e18);
    for (int i = 0; i < n; i++) {
        i64 x = as[i];
        i64 y = segtree.query(i + 1, min(n, i + d + 1));
        if (y - x > nax) {
            j = i;
            nax = y - x;
        }
    }
    if (nax == 0) {
        cout << nax << endl;
        return 0;
    }
    cout << nax * k << endl;
    cout << j << " ";
    for (int i = j + 1; i < n; i++) {
        if (as[i] == as[j] + nax) {
            cout << i << endl;
            return 0;
        }
    }
}
