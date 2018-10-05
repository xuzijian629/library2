#include <bits/stdc++.h>
using namespace std;

long long dp[101][10101];
// i番目までから選んで、価値jのときの重さの最小値

int main() {
    int n, W;
    cin >> n >> W;
    for (int i = 0; i < 101; i++) {
        for (int j = 0; j < 10101; j++) {
            dp[i][j] = 1e12;
        }
    }
    dp[0][0] = 0;

    for (int i = 0; i < n; i++) {
        int v, w;
        cin >> v >> w;
        for (int j = 0; j < 10101; j++) {
            if (j - v >= 0) {
                dp[i + 1][j] = min(dp[i][j], dp[i][j - v] + w);   
            } else {
                dp[i + 1][j] = dp[i][j];
            }
        }
    }

    int ans = 0;
    for (int j = 0; j < 10101; j++) {
        if (dp[n][j] <= W) {
            ans = max(ans, j);
        }
    }
    cout << ans << endl;
}
