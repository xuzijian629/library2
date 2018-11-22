#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

struct SlideMin {
    // data[i]: mininum over [max(0, i + 1 - width), i + 1)
    int n;
    vi data;
    SlideMin(const vi &as, int width, bool minimum = true) : n(as.size()), data(as.size()) {
        using ii = pair<i64, int>;
        deque<ii> deq;
        auto comp = [&](ii &e, i64 v) {
            return minimum ? e.first >= v : e.first <= v;
        };
        for (int i = 0; i < n; i++) {
            if (deq.size() && deq.front().second <= i - width) {
                deq.pop_front();
            }
            while (deq.size()) {
                if (comp(deq.back(), as[i])) deq.pop_back();
                else break;
            }
            deq.push_back(ii(as[i], i));
            data[i] = deq.front().first;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n, m, k;
    cin >> n >> m >> k;
    vi as(n);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }

    vi dp(as);
    for (int i = 0; i < k - 1; i++) {
        SlideMin smax(dp, m, false);
        for (int j = 0; j < n; j++) {
            if (j <= i) {
                dp[j] = 0;
            } else {
                dp[j] = smax.data[j - 1] + as[j] * (i + 2);
            }
        }
    }
    i64 nax = -1;
    for (int i = 0; i < n; i++) {
        nax = max(nax, dp[i]);
    }
    cout << nax << endl;
}
