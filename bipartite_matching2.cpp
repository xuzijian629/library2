#include <bits/stdc++.h>
using namespace std;

class BipartiteMatching {
    int n, n0;
    int mode;
    vector<vector<int>> adj;
    vector<int> mc;
    vector<int> used;

    bool dfs(int v) {
        used[v] = true;
        for (int i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            int w = mc[u];
            if (w < 0 || (!used[w] && dfs(w))) {
                mc[v] = u;
                mc[u] = v;
                return true;
            }
        }
        return false;
    }
public:
    BipartiteMatching(int n) : n(n), adj(n), mc(n, -1) {
        mode = 0;
    }
    BipartiteMatching(int n, int m) : n(n + m), n0(n), adj(n + m), mc(n + m, -1) {
        mode = 1;
    }

    void add_edge(int u, int v) {
        if (mode == 0) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        } else {
            adj[u].push_back(n0 + v);
            adj[n0 + v].push_back(u);
        }
    }

    int match() {
        int ret = 0;
        for (int v = 0; v < n; v++) {
            if (mc[v] < 0) {
                used = vector<int>(n);
                if (dfs(v)) {
                    ret++;
                }
            }
        }
        return ret;
    }

    int operator[](int k) {
        return mc[k];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> adj(n);
        for (int i = 1; i < n; i++) {
            int p;
            cin >> p;
            p--;
            adj[i].push_back(p);
            adj[p].push_back(i);
        }
        vector<vector<int>> ances(n);
        function<void(int, int)> dfs = [&](int v, int p) {
            if (p != -1) {
                for (int a : ances[p]) ances[v].push_back(a);
            }
            ances[v].push_back(v);
            for (int s : adj[v]) {
                if (s != p) {
                    dfs(s, v);
                }
            }
        };
        dfs(0, -1);
        for (int i = 0; i < n; i++) {
            sort(ances[i].begin(), ances[i].end());
        }
        vector<vector<int>> lst(m);
        for (int i = 0; i < m; i++) {
            int k;
            cin >> k;
            for (int j = 0; j < k; j++) {
                int a;
                cin >> a;
                a--;
                lst[i].push_back(a);
            }
        }
        for (int i = 0; i < m; i++) {
            sort(lst[i].begin(), lst[i].end());
        }

        auto ok = [&](int a, int b) {
            int i = 0, j = 0;
            while (i < ances[a].size() && j < lst[b].size()) {
                if (ances[a][i] == lst[b][j]) return true;
                if (ances[a][i] < lst[b][j]) i++;
                else if (ances[a][i] > lst[b][j]) j++;
            }
            return false;
        };

//        for (int i = 0; i < n; i++) {
//            cout << "ances[" << i << "]: ";
//            for (int a : ances[i]) {
//                cout << a << ' ';
//            }
//            cout << endl;
//        }

        BipartiteMatching match(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (ok(i, j)) {
//                    cout << "edge: " << i << ' ' << j << endl;
                    match.add_edge(i, j);
                }
            }
        }
        cout << match.match() << endl;
    }
}