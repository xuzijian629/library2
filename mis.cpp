#include <bits/stdc++.h>
using namespace std;

vector<int> maximalIndependentSet(const vector<vector<int>>& adj) {
    mt19937 rnd(chrono::system_clock::now().time_since_epoch().count());
    int n = adj.size();
    vector<int> pm(n);
    iota(pm.begin(), pm.end(), 0);

    auto start = chrono::system_clock::now();

    vector<int> ret;
    int sz = 0;

    int cnt = 0;

    while (1) {
        cnt++;
        if (cnt >> 10 & 1) {
            auto now = chrono::system_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(now - start).count() > 1800) break;
        }

        vector<int> tmp(n);
        int s = 0;

        for (int a : pm) {
            bool ok = true;
            for (int b : adj[a]) {
                if (tmp[b]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                tmp[a] = 1;
                s++;
            }
        }

        if (s > sz) {
            sz = s;
            ret = tmp;
        }
        
        shuffle(pm.begin(), pm.end(), rnd);
    }

    cerr << "tried " << cnt << " times" << endl;
    return ret;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> mis = maximalIndependentSet(adj);
    cout << accumulate(mis.begin(), mis.end(), 0) << endl;
}
