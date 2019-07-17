// iran

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
i64 MOD = 1e9 + 7;

int main() {
    int n, m;
    cin >> n >> m;
    
    int N;
    for (N = 1; N * N < n; N++);
    
    vi a(N * N), b(N * N);
    vi asum(N), aeach(N), bsum(N), beach(N), xsum(N);

    auto add_a = [&](int s, int t, i64 x) {
        for (int k = 0; k < N; k++) {
            int l = k * N, r = (k + 1) * N;
            if (r <= s || t <= l) {
                continue;
            }
            if (s <= l && r <= t) {
                asum[k] += x * N;
                asum[k] %= MOD;
                aeach[k] += x;
                aeach[k] %= MOD;
                xsum[k] += bsum[k] * x;
                xsum[k] %= MOD;
            } else {
                for (int i = max(s, l); i < min(t, r); i++) {
                    a[i] += x;
                    a[i] %= MOD;
                    assert(i / N == k);
                    asum[k] += x;
                    asum[k] %= MOD;
                    xsum[k] += (b[i] + beach[k]) * x;
                    xsum[k] %= MOD;
                }
            }
        }
    };

    auto add_b = [&](int s, int t, i64 x) {
        for (int k = 0; k < N; k++) {
            int l = k * N, r = (k + 1) * N;
            if (r <= s || t <= l) {
                continue;
            }
            if (s <= l && r <= t) {
                bsum[k] += x * N;
                bsum[k] %= MOD;
                beach[k] += x;
                beach[k] %= MOD;
                xsum[k] += asum[k] * x;
                xsum[k] %= MOD;
            } else {
                for (int i = max(s, l); i < min(t, r); i++) {
                    b[i] += x;
                    b[i] %= MOD;
                    bsum[k] += x;
                    bsum[k] %= MOD;
                    xsum[k] += (a[i] + aeach[k]) * x;
                    xsum[k] %= MOD;
                }
            }
        }
    };

    auto query = [&](int s, int t) {
        i64 sum = 0;
        for (int k = 0; k < N; k++) {
            int l = k * N, r = (k + 1) * N;
            if (r <= s || t <= l) {
                continue;
            }
            if (s <= l && r <= t) {
                sum += xsum[k];
                sum %= MOD;
            } else {
                for (int i = max(s, l); i < min(t, r); i++) {
                    sum += (a[i] + aeach[k]) * (b[i] + beach[k]);
                    sum %= MOD;
                }
            }
        }
        cout << sum << endl;
    };
    
    for (int i = 0; i < m; i++) {
        char c;
        cin >> c;
        if (c == '*') {
            int l, r, x;
            cin >> l >> r >> x;
            l--;
            add_a(l, r, x);
        } else if (c == '.') {
            int l, r, x;
            cin >> l >> r >> x;
            l--;
            add_b(l, r, x);
        } else {
            int l, r;
            cin >> l >> r;
            l--;
            query(l, r);
        }
    }
}
