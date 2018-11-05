#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
constexpr i64 MOD = 1e9 + 7;

i64 partition_count(int n, int m, i64 mod) {
    // divide n (undistinguished) items into m (undistinguished) groups, groups can have 0 items
    vvi dp(m + 1, vi(n + 1));
    dp[0][0] = 1;
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (j - i >= 0) {
                dp[i][j] = (dp[i - 1][j] + dp[i][j - i]) % mod;
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    return dp[m][n];
}

int main() {
    int n, k;
    cin >> n >> k;
    cout << partition_count(n, k, MOD) << endl;
}