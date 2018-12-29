#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

i64 inv_number(vi& as) {
    i64 cnt = 0;
    int n = as.size();
    if (n > 1) {
        vi bs(as.begin(), as.begin() + n / 2);
        vi cs(as.begin() + n / 2, as.end());
        cnt += inv_number(bs);
        cnt += inv_number(cs);
        for (int i = 0, j = 0, k = 0; i < n; i++) {
            if (k == cs.size()) {
                as[i] = bs[j++];
            } else if (j == bs.size()) {
                as[i] = cs[k++];
            } else if (bs[j] <= cs[k]) {
                as[i] = bs[j++];
            } else {
                as[i] = cs[k++];
                cnt += n / 2 - j;
            }
        }
    }
    return cnt;
}
