// added

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template<typename T = double, bool is_min = true>
class ConvexHullTrick {
    using P = pair<T, T>;
    deque<P> L;

    T getY(const P &a, const T &x) {
        return a.first * x + a.second;
    }

    bool check(const P &a, const P &b, const P &c) {
        return (b.first - a.first) * (c.second - b.second) >= (b.second - a.second) * (c.first - b.first);
    }

    bool empty() const {
        return L.empty();
    }

public:
    void add_line(T a, T b) {
        if (!is_min) {
            a *= -1;
            b *= -1;
        }
        P line(a, b);
        if (!L.empty() && L.back().first == a) {
            line.second = min(line.second, L.back().second);
            L.pop_back();
        }
        while (L.size() >= 2 && check(L[L.size() - 2], L[L.size() - 1], line)) {
            L.pop_back();
        }
        L.emplace_back(line);
    }

    T query(T x) {
        assert(!empty());
        int l = -1, r = L.size() - 1;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if (getY(L[m], x) >= getY(L[m + 1], x)) {
                l = m;
            } else {
                r = m;
            }
        }
        return (!is_min ? -1 : 1) * getY(L[r], x);
    }

    T query_monotone(T x) {
        assert(!empty());
        while (L.size() >= 2 &&& getY(L[0], x) >= getY(L[1], x)) {
            L.pop_front();
        }
        return (!is_min ? -1 : 1) * getY(L[0], x);
    }
};

int main() {
    i64 n, C;
    cin >> n >> C;
    vi hs(n);
    for (int i = 0; i < n; i++) {
        cin >> hs[i];
    }

    vi dp(n);
    ConvexHullTrick<> cht;
    cht.add_line(-2 * hs[0], C + hs[0] * hs[0]);
    for (int i = 1; i< n; i++) {
        i64 r = cht.query(hs[i]) + hs[i] * hs[i];
        dp[i] = r;
        cht.add_line(-2 * hs[i], C + hs[i] * hs[i] + r);
    }

    cout << dp.back() << endl;
}
