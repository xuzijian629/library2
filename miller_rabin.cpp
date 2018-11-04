#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct MillerRabin {    
    i64 modpow(i64 a, i64 n, i64 mod) {
        if (n == 0) return 1;
        if (n % 2 == 0) {
            i64 t = modpow(a, n / 2, mod);
            return t * t % mod;
        }
        return a * modpow(a, n - 1, mod) % mod;
    }

    i64 modinv(i64 n, i64 mod) {
        return modpow(n, mod - 2, mod);
    }

    bool is_prime(i64 n, int k = 50) {
        if (n == 2) return true;
        if (n < 2 || n % 2 == 0) return false;
        i64 d = n - 1;
        while (d % 2 == 0) {
            d /= 2;
        }
        for (int i = 0; i < k; i++) {
            i64 a = rnd() % (n - 2) + 1;
            i64 t = d;
            i64 y = modpow(a, t, n);
            while (t != n - 1 && y != 1 && y != n - 1) {
                y = modpow(y, 2, n);
                t *= 2;
            }
            if (y != n - 1 && t % 2 == 0) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    int m, n;
    cin >> m >> n;
    vi as(n);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }

    vi dp(m + 1, -1);
    // dp[i]: ちょうどi円で買えるカップ麺の個数の最大値
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m + 1; j++) {
            if (j - as[i] >= 0 && dp[j - as[i]] != -1) {
                dp[j] = max(dp[j], dp[j - as[i]] + 1);
            }
        }
    }

    MillerRabin p;
    i64 ans = 0;
    for (int i = 0; i <= m; i++) {
        ans = max(ans, dp[i]);
    }
    for (int i = 0; i <= m; i++) {
        if (p.is_prime(m - i) && dp[i] != -1) {
            ans += dp[i];
        }
    }
    cout << ans << endl;
}   