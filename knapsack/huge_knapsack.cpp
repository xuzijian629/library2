#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using ii = pair<i64, i64>;
using vii = vector<ii>;

int main() {
    i64 n, W;
    cin >> n >> W;

    vi vs(n), ws(n);
    for (int i = 0; i < n; i++) {
        cin >> vs[i] >> ws[i];
    }

    vii as, bs;
    for (int i = 0; i < (1 << n / 2); i++) {
        i64 value = 0, weight = 0;
        for (int j = 0; j < n / 2; j++) {
            if ((i >> j) & 1) {
                value += vs[j];
                weight += ws[j];
            }
        }
        as.push_back(ii(value, weight));
    }
    for (int i = 0; i < (1 << (1 + n) / 2); i++) {
        i64 value = 0, weight = 0;
        for (int j = 0; j < (1 + n) / 2; j++) {
            if ((i >> j) & 1) {
                value += vs[n / 2 + j];
                weight += ws[n / 2 + j];
            }
        }
        bs.push_back(ii(value, weight));
    }

    auto comp = [](ii p, ii q) {
        return (p.second == q.second && p.first > q.first) || p.second < q.second;
    };
    sort(as.begin(), as.end(), comp);

    vii cs = {as.front()};
    for (int i = 1; i < as.size(); i++) {
        if (as[i].first > cs.back().first) {
            cs.push_back(as[i]);
        }
    }

    i64 ans = 0;
    for (ii b: bs) {
        i64 value = b.first, weight = b.second;
        i64 rem = W - weight;

        auto it = lower_bound(cs.begin(), cs.end(), ii(-1, rem), comp);
        if (it == cs.begin()) continue;
        i64 V = (*(it - 1)).first + value;
        ans = max(ans, V);
    }
    cout << ans << endl;
}

