#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

vi des, asc;
using ii = pair<i64, int>;
vector<vector<ii>> adj;

i64 ans = 0;

void dfs1(int v, int p) {
    i64 m = 0;
    for (ii s: adj[v]) {
        if (s.second != p) {
            dfs1(s.second, v);
            m = max(m, des[s.second] + s.first);
        }
    }
    des[v] = m;
}

void dfs2(int v, int p) {
    vector<ii> ds = {ii(0, 0)};
    for (ii s: adj[v]) {
        if (s.second == p) {
            ds.push_back(ii(asc[p] + s.first, s.second));
        } else {
            ds.push_back(ii(des[s.second] + s.first, s.second));
        }
    }
    sort(ds.begin(), ds.end(), greater<>());
    ans = max(ans, ds[0].first + ds[1].first);
    for (ii s: adj[v]) {
        if (s.second != p) {
            asc[s.second] = ds[ds[0].second == s.second].first;
            dfs2(s.second, v);
        }
    }
}

int main() {
    int n;
    cin >> n;
    des = vi(n);
    asc = vi(n);
    adj = vector<vector<ii>>(n);
    for (int i = 0; i < n - 1; i++) {
        int s, t, w;
        cin >> s >> t >> w;
        adj[s].push_back(ii(w, t));
        adj[t].push_back(ii(w, s));
    }
    dfs1(0, -1);
    dfs2(0, -1);

    cout << ans << endl;
}