// iran

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

i64 solve_knapsack_small_weight(const vi &ws, const vi &vs, int MAX_WEIGHT) {
    int n = ws.size();
    vi value(MAX_WEIGHT + 1);
    for (int i = 0; i < n; i++) {
        for (int j = MAX_WEIGHT; j >= 0; j--) {
            if (j - ws[i] >= 0) {
                value[j] = max(value[j], value[j - ws[i]] + vs[i]);
            }
        }
    }
    i64 nax = 0;
    for (int i = 0; i <= MAX_WEIGHT; i++) {
        nax = max(nax, value[i]);
    }
    return nax;
}

int main() {
    int n, w;
    cin >> n >> w;
    vi ws(n), vs(n);
    for (int i = 0; i < n; i++) {
        cin >> ws[i] >> vs[i];
    }
    cout << solve_knapsack_small_weight(ws, vs, w) << endl;
}

