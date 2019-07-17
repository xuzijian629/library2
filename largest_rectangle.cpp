// added
#include <bits/stdc++.h>

using namespace std;

int largest_rectangle(vector<int> &hist) {
    hist.push_back(0);
    stack<pair<int, int>> st;
    int ret = 0;
    for (int i = 0; i < hist.size(); i++) {
        if (st.empty() || st.top().first < hist[i]) {
            st.emplace(hist[i], i);
        } else {
            int s = 0;
            while (!st.empty() && st.top().first >= hist[i]) {
                ret = max(ret, st.top().first * (i - st.top().second));
                s = st.top().second;
                st.pop();
            }
            st.emplace(hist[i], s);
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
    vector<vector<int>> b(h, vector<int>(w));
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

    int ret = 0;
    for (int i = 0; i < h; i++) {
        ret = max(ret, largest_rectangle(b[i]));
    }
    cout << ret << endl;
}