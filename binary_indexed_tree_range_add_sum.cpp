#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class BIT {
    int n;
    vi data0, data1;

    void add(vi& data, int i, i64 x) {
        while (i <= n) {
            data[i] += x;
            i += i & -i;
        }
    }

    i64 sum(vi& data, int i) {
        i64 s = 0;
        while (i > 0) {
            s += data[i];
            i -= i & -i;
        }
        return s;
    }

public:
    BIT(int n) : n(n) {
        data0 = vi(n + 1);
        data1 = vi(n + 1);
    }

    // [l, r)
    void add(int l, int r, i64 x) {
        l++;
        r++;
        add(data0, l, -x * (l - 1));
        add(data1, l, x);
        add(data0, r, x * (r - 1));
        add(data1, r, -x);
    }

    // [l, r)
    i64 sum(int l, int r) {
        i64 s = 0;
        s += sum(data0, r) + sum(data1, r) * r;
        s -= sum(data0, l) + sum(data1, l) * l;
        return s;
    }
};

int main() {
    int n;
    cin >> n;
    BIT bit(n);
    int q;
    cin >> q;
    while (q--) {
        int t;
        cin >> t;
        if (t) {
            int i;
            cin >> i;
            i--;
            cout << bit.sum(i, i + 1) << endl;
        } else {
            int s, t, x;
            cin >> s >> t >> x;
            s--;
            t--;
            bit.add(s, t + 1, x);
        }
    }
}