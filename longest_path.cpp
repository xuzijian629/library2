// added
#include <bits/stdc++.h>

using namespace std;

int longest_path(const vector<vector<int>> &adj) {
    int n = adj.size();
    vector<vector<int>> inv(n);
    for (int a = 0; a < n; a++) {
        for (auto b : adj[a]) {
            inv[b].push_back(a);
        }
    }
    vector<int> len(n, -1);
    function<void(int)> set_len = [&](int v) {
        if (len[v] != -1) return;
        if (inv[v].empty()) {
            len[v] = 0;
            return;
        }
        int nax = 0;
        for (int u : inv[v]) {
            set_len(u);
            nax = max(nax, len[u]);
        }
        len[v] = nax + 1;
    };

    for (int i = 0; i < n; i++) {
        set_len(i);
    }

    return *max_element(len.begin(), len.end());
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
    }
    cout << longest_path(adj) << endl;
}