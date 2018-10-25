// NOT VERIFIED
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
constexpr i64 MOD = 1000000000;

i64 partition_count(int n, int m, i64 mod) {
    // divide n items by m people
    vvi dp(m + 1, vi(n + 1));
    dp[0][0] = 1;
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (j - i >= 0) {
                dp[i][j] = (dp[i - 1][j] + dp[i][j - 1]) % mod;
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    return dp[m][n];
}

int main() {
    i64 n;
    cin >> n;
    n /= 1000;
    
    i64 m;
    cin >> m;
    
    n %= m;
    cout << partition_count(n, m, MOD) << endl;
}