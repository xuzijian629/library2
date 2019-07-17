#include <bits/stdc++.h>
using namespace std;
constexpr int MOD = 1000000000;

struct Combination {
    int n;
    vector<vector<int>> dp;
    Combination(int n) : n(n) {}

    void build() {
        dp = vector<vector<int>>(n + 1, vector<int>(n + 1));
        for (int i = 0; i <= n; i++) {
            dp[i][0] = 1;
            dp[i][i] = 1;
        }
        for (int i = 2; i <= n; i++) {
            for (int j = 1; j < i; j++) {
                dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
                dp[i][j] %= MOD;
            }
        }
    }

    int built_ncr(int n, int r) {
        return dp[n][r];
    }

    // avoid MLE
    int ncr(int n, int r) {
        if (n < 2) return 1;
        vector<int> cur(2, 1);
        for (int i = 2; i <= n; i++) {
            vector<int> nex(n + 1, 1);
            for (int j = 1; j < i; j++) {
                nex[j] = cur[j - 1] + cur[j];
                nex[j] %= MOD;
            }
            cur = move(nex);
        }
        return cur[r];
    }
};

int main() {
    int n;
    cin >> n;
    n /= 1000;

    int m;
    cin >> m;

    n %= m;

    Combination comb(m);
    cout << comb.ncr(m, n) << endl;
}