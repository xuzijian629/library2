#include <bits/stdc++.h>
using namespace std;

int dp[101][10101];

int main() {
    int n, W;
    cin >> n >> W;

    for (int i = 0; i < n; i++) {
        int v, w;
        cin >> v >> w;
        for (int j = 0; j < 10101; j++) {
            if (j - w >= 0) {
                dp[i + 1][j] = max(dp[i][j], dp[i][j - w] + v);
            } else {
                dp[i + 1][j] = dp[i][j];
            }
        }
    }
    int ans = 0;
    for (int j = 0; j <= W; j++) {
        ans = max(ans, dp[n][j]);
    }
    cout << ans << endl;
}
