#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class RangeFind {
    int n;
    vector<i64> data;
    
    int find(int a, int b, const i64 x, int k, int l, int r) {
        if (data[k] > x || r <= a || b <= l) return -1;
        if (k >= n) {
            return k - n;
        }
        int t = find(a, b, x, 2 * k + 1, (l + r) >> 1, r);
        if (t != -1) return t;
        return find(a, b, x, 2 * k, l, (l + r) >> 1);
    }
    
public:
    RangeFind(const vector<i64>& as) {
        n = 1;
        while (n < as.size()) n <<= 1;
        data.assign(2 * n, 1e18);
        for (int i = 0; i < as.size(); i++) {
            data[n + i] = as[i];
        }
        for (int i = n - 1; i > 0; i--) {
            data[i] = min(data[2 * i], data[2 * i + 1]);
        }
    }
    
    void update(int k, const i64& x) {
        k += n;
        data[k] = x;
        while (k >>= 1) {
            data[k] = min(data[2 * k], data[2 * k + 1]);
        }
    }
    
    i64 query(int a, int b) {
        i64 L = 1e18, R = 1e18;
        for (a += n, b += n; a < b; a >>= 1, b >>= 1) {
            if (a & 1) {
                L = min(L, data[a++]);
            }
            if (b & 1) {
                R = min(data[--b], R);
            }
        }
        return min(L, R);
    }
    
    // [a, b)でx以下の値をもつ最右の要素の位置
    int find(int a, int b, const i64& x) {
        return find(a, b, x, 1, 0, n);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout.setf(ios::fixed);
    cout.precision(10);
    int n, q;
    cin >> n >> q;
    vi sum(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> sum[i + 1];
        sum[i + 1] += sum[i];
    }
    RangeFind rf(sum);
    while (q--) {
        i64 x;
        cin >> x;
        i64 ret = 0;
        for (int i = 1; i <= n; i++) {
            int k = rf.find(i, n + 1, x + sum[i - 1]);
            if (k != -1) {
                ret += k - i + 1;
            }
        }
        cout << ret << endl;
    }
}
