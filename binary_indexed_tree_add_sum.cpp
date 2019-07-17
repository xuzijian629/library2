// iran

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class BIT {
    int n, N;
    vi data;

    i64 sum(int i) {
        i64 s = 0;
        while (i > 0) {
            s += data[i];
            i -= i & -i;
        }
        return s;
    }

    // assuming elements are all positive
    int lower_bound(i64 w) {
        if (w <= 0) return -1;
        int x = 0;
        for (int k = N; k > 0; k >>= 1) {
            if (x + k <= n && data[x + k] < w) {
                w -= data[x + k];
                x += k;
            }
        }
        return x;
    }

    int upper_bound(i64 w) {
        if (w <= 0) return -1;
        int x = 0;
        for (int k = N; k > 0; k >>= 1) {
            if (x + k <= n && data[x + k] <= w) {
                w -= data[x + k];
                x += k;
            }
        }
        return x;
    }

public:
    BIT(int n) : n(n) {
        data = vi(n + 1);
        N = 1;
        while (N <= n) N <<= 1;
        N >>= 1;
    }

    void add(int i, i64 x) {
        i++;
        while (i <= n) {
            data[i] += x;
            i += i & -i;
        }
    }

    // [l, r)
    i64 sum(int l, int r) {
        return sum(r) - sum(l);
    }

    // sum([a, k]) >= xとなる最小のk
    int lower_bound(int a, i64 x) {
        return max(a, lower_bound(x + sum(0, a)));
    }

    // sum([a, k]) > xとなる最小のk
    int upper_bound(int a, i64 x) {
        return max(a, upper_bound(x + sum(0, a)));
    }
};

int main() {
    int n, q;
    cin >> n >> q;
    BIT bit(n + 1);
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        bit.add(i, a);
    }
    bit.add(n, 1e17);
    while (q--) {
        i64 x;
        cin >> x;
        i64 ret = 0;
        for (int i = 0; i < n; i++) {
            ret += bit.upper_bound(i, x) - i;
        }
        cout << ret << endl;
    }
}