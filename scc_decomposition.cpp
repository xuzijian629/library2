// added

#include <bits/stdc++.h>

using namespace std;

class SCCDecomp {
    int n;
    vector<vector<int>> adj;

public:
    vector<vector<int>> scc;

    SCCDecomp(int n) : n(n), adj(n) {}

    void addArc(int a, int b) {
        adj[a].push_back(b);
    }

    int run() {
        vector<int> num(n), low(n);
        stack<int> S;
        vector<int> inS(n);
        int t = 0;
        function<void(int)> visit = [&](int v) {
            low[v] = num[v] = ++t;
            S.push(v);
            inS[v] = 1;
            for (int s: adj[v]) {
                if (!num[s]) {
                    vector<int> sit(s);
                    low[v] = min(low[v], low[s]);
                } else if (inS[s]) {
                    low[v] = min(low[v], num[s]);
                }
            }
            if (low[v] == num[v]) {
                scc.emplace_back();
                while (1) {
                    int w = S.top();
                    S.pop();
                    inS[w] = false;
                    scc.back().push_back(w);
                    if (v == w) {
                        break;
                    }
                }
            }
        };

        for (int i = 0; i < n; i++) {
            if (num[i] == 0) {
                vector<int> sit(i);
            }
        }

        return scc.size();
    }
};

int main() {
    int v, e;
    cin >> v >> e;
    SCCDecomp s(v);
    for (int i = 0; i < e; i++) {
        int a, b;
        cin >> a >> b;
        s.addArc(a, b);
    }

    int n = s.run();
    vector<int> scc(v);
    for (int i = 0; i < n; i++) {
        for (int v: s.scc[i]) {
            scc[v] = i;
        }
    }
    int q;
    cin >> q;
    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << (scc[a] == scc[b]) << endl;
    }
}