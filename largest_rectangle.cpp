#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

i64 largest_rectangle(vi& hist) {
    hist.push_back(0);
    using ii = pair<i64, int>;
    stack<ii> st;
    i64 ret = 0;
    for (int i = 0; i < hist.size(); i++) {
        if (st.empty() || st.top().first < hist[i]) {
            st.push(ii(hist[i], i));
        } else {
            int s;
            while (st.size() && st.top().first >= hist[i]) {
                ret = max(ret, st.top().first * (i - st.top().second));
                s = st.top().second;
                st.pop();
            }
            st.push(ii(hist[i], s));
        }
    }
    
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int h, w;
    cin >> h >> w;
    vvi b(h, vi(w));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int c;
            cin >> c;
            if (c == 0) {
                if (i == 0) {
                    b[i][j] = 1;
                } else {
                    b[i][j] = b[i - 1][j] + 1;
                }
            } else {
                b[i][j] = 0;
            }
        }
    }

    i64 ret = 0;
    for (int i = 0; i < h; i++) {
        ret = max(ret, largest_rectangle(b[i]));
    }
    cout << ret << endl;
}