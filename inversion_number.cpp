// added

#include <bits/stdc++.h>

using namespace std;

long long inv_number(vector<int> &as) {
    int cnt = 0;
    int n = as.size();
    if (n > 1) {
        vector<int> bs(as.begin(), as.begin() + n / 2);
        vector<int> cs(as.begin() + n / 2, as.end());
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
