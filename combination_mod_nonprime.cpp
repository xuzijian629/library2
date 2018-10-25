#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
constexpr i64 MOD = 1000000000;

struct Combination {
    int n;
    vvi dp;
    Combination(int n) : n(n) {}
    
    void build() {
        dp = vvi(n + 1, vi(n + 1));
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
    
    i64 built_ncr(int n, int r) {
        return dp[n][r];
    }
    
    // avoid MLE
    i64 ncr(int n, int r) {
        if (n < 2) return 1;
        vi cur(2, 1);
        for (int i = 2; i <= n; i++) {
            vi nex(n + 1, 1);
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
    i64 n;
    cin >> n;
    n /= 1000;
    
    i64 m;
    cin >> m;
    
    n %= m;
    
    Combination comb(m);
    cout << comb.ncr(m, n) << endl;
}