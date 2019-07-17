// hoge

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> calc_dist(const vector<vector<int>>& adj) {
    // return two vectors v[0], v[1]
    // v[0][i] is the length to the furthest descendant
    // v[1][i] is the length to the furthest node passing parent 
    int n = adj.size();
    vector<int> dp(n), dp2(n);
    function<void(int, int)> dfs = [&](int v, int p) {
        int nax = 0;
        bool leaf = true;
        for (int s : adj[v]) {
            if (s != p) {
                leaf = false;
                dfs(s, v);
                nax = max(nax, dp[s]);
            }
        }
        if (!leaf) {
            dp[v] = nax + 1;
        }
    };
    dfs(0, -1);

    function<void(int, int)> dfs2 = [&](int v, int p) {
        vector<int> ds;
        for (int s : adj[v]) {
            if (s != p) {
                ds.push_back(dp[s]);
            }
        }
        sort(ds.rbegin(), ds.rend());
        for (int s : adj[v]) {
            if (s != p) {
                if (ds.size() == 1) {
                    dp2[s] = dp2[v] + 1;
                } else {
                    dp2[s] = max(dp2[v] + 1, (ds[0] == dp[s] ? ds[1] : ds[0]) + 2);
                }
                dfs2(s, v);
            }
        }
    };
    dfs2(0, -1);
    vector<vector<int>> ret;
    ret.emplace_back(dp);
    ret.emplace_back(dp2);
    return ret;
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> adja(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adja[a].push_back(b);
        adja[b].push_back(a);
    }
    int m;
    cin >> m;
    vector<vector<int>> adjb(m);
    for (int i = 0; i < m - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adjb[a].push_back(b);
        adjb[b].push_back(a);
    }

    auto va = calc_dist(adja);
    auto vb = calc_dist(adjb);

    int A = 0, B = 0, C;
    vector<int> as, bs;
    for (int i = 0; i < n; i++) {
        A = max(A, va[0][i] + va[1][i]);
        as.push_back(max(va[0][i], va[1][i]));
    }
    for (int i = 0; i < m; i++) {
        B = max(B, vb[0][i] + vb[1][i]);
        bs.push_back(max(vb[0][i], vb[1][i]));
    }
    C = max(A, B);
    sort(bs.begin(), bs.end());
    vector<long long> bsum(m + 1);
    for (int i = 0; i < m; i++) {
        bsum[i + 1] = bsum[i] + bs[i];
    }

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        int d = C - as[i] - 1;
        // sum up all bs[j] where bs[j] >= d;
        int lt = lower_bound(bs.begin(), bs.end(), d) - bs.begin();
        ans += 1ll * C * lt;
        ans += 1ll * (m - lt) * (as[i] + 1) + bsum[m] - bsum[lt];
    }

    cout << ans << endl;
}
