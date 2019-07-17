// iran

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

i64 solve_knapsack_small_value(const vi &ws, const vi &vs, int MAX_VALUE, i64 W) {
    int n = ws.size();
    int V = n * MAX_VALUE;
    vi weight(V + 1, 1e18);
    weight[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = V; j >= 0; j--) {
            if (j - vs[i] >= 0) {
                weight[j] = min(weight[j], weight[j - vs[i]] + ws[i]);
            }
        }
    }
    i64 nax = 0;
    for (int i = 0; i <= V; i++) {
        if (weight[i] <= W) {
            nax = max(nax, i64(i));
        }
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
    cout << solve_knapsack_small_value(ws, vs, 1000, w) << endl;
}

