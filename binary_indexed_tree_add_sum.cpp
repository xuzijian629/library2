#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class BIT {
    int n;
    vi data;

    i64 sum(int i) {
        i64 s = 0;
        while (i > 0) {
            s += data[i];
            i -= i & -i;
        }
        return s;
    }

public:
    BIT(int n) : n(n) {
        data = vi(n + 1);
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
            int x, y;
            cin >> x >> y;
            x--;
            y--;
            cout << bit.sum(x, y + 1) << endl;
        } else {
            int i, x;
            cin >> i >> x;
            i--;
            bit.add(i, x);
        }
    }
}