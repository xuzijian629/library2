// added
#include <bits/stdc++.h>

using namespace std;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

struct MillerRabin {
    int modpow(int a, int n, int mod) {
        if (n == 0) return 1;
        if (n % 2 == 0) {
            int t = modpow(a, n / 2, mod);
            return t * t % mod;
        }
        return a * modpow(a, n - 1, mod) % mod;
    }

    int modinv(int n, int mod) {
        return modpow(n, mod - 2, mod);
    }

    bool is_prime(int n, int k = 50) {
        if (n == 2) return true;
        if (n < 2 || n % 2 == 0) return false;
        int d = n - 1;
        while (d % 2 == 0) {
            d /= 2;
        }
        for (int i = 0; i < k; i++) {
            int a = rnd() % (n - 2) + 1;
            int t = d;
            int y = modpow(a, t, n);
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
    vector<int> as(n);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }

    vector<int> dp(m + 1, -1);
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
    int ans = 0;
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