// iran

#include <bits/stdc++.h>
using namespace std;

// [a, b]まで最短いくつの区間を通って行けるか
//　https://twitter.com/ei1333/status/1112072250885181441
struct TapuRing {
    const int MAX_M = 505050;
    vector<pair<int, int>> es;
    // dp[i][j]: j番目の点から2^i個以内の区間を通って到達可能な最右の点
    vector<vector<int>> dp;

    // [a, b]を結ぶ区間を追加
    void add_segment(int a, int b) {
        assert(b < MAX_M);
        es.emplace_back(a, b);
    }

    void build() {
        int n = es.size();
        dp.assign(20, vector<int>(MAX_M));
        for (auto &e : es) {
            dp[0][e.first] = max(dp[0][e.first], e.second);
        }
        for (int i = 1; i < MAX_M; i++) {
            dp[0][i] = max(dp[0][i - 1], dp[0][i]);
        }
        for (int i = 1; i < 20; i++) {
            for (int j = 0; j < MAX_M; j++) {
                dp[i][j] = dp[i - 1][dp[i - 1][j]];
            }
        }
    }

    // [a, b]まで最短いくつの区間を通るか
    int query(int a, int b) {
        int ret = 1;
        for (int i = 19; i >= 0; i--) {
            if (dp[i][a] < b) {
                ret += 1 << i;
                a = dp[i][a];
            }
        }
        if (ret > 505050) return -1;
        return ret;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    TapuRing tapu;
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        tapu.add_segment(a, b);
    }
    tapu.build();
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        cout << tapu.query(a, b) << '\n';
    }
}
