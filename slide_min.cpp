template <class T>
class SegTree {
    using F = function<T(T, T)>;
    int n;
    vector<T> data;
    F f;
    T e;

public:
    SegTree<T>() {}
    SegTree<T>& operator=(const SegTree<T>& t) {
        n = t.n;
        data = t.data;
        f = t.f;
        e = t.e;
        return *this;
    }

    SegTree<T>(const vector<T>& as, const F f, const T& e) : f(f), e(e) {
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