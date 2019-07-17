// added

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Dinic {
    struct edge {
        int to, cap, rev;
    };

    int n;
    vector<vector<edge>> G;
    vector<int> level;
    vector<int> iter;

    void bfs(int s) {
        level.assign(n, -1);
        queue<int> que;
        que.push(s);
        level[s] = 0;
        while (que.size()) {
            int v = que.front();
            que.pop();
            for (int i = 0; i < G[v].size(); i++) {
                edge &e = G[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int i = iter[v]; i < G[v].size(); i++) {
            edge &e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    Dinic(int n) : n(n), G(n), level(n), iter(n) {}

    void add_edge(int from, int to, int cap) {
        G[from].push_back({to, cap, (int) G[to].size()});
        G[to].push_back({from, 0, (int) G[from].size() - 1});
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (1) {
            bfs(s);
            if (level[t] < 0) return flow;
            iter.assign(n, 0);
            int f;
            while ((f = dfs(s, t, 1e9)) > 0) {
                flow += f;
            }
        }
    }
};

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int h, w, W;
    cin >> h >> w >> W;
    vector<vector<int>> be(h, vector<int>(w));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cin >> be[i][j];
        }
    }

    vector<int> as, bs, cs;
    auto push = [&](int a, int b, int c) {
        as.push_back(a);
        bs.push_back(b);
        cs.push_back(c);
    };

    int idx = 0;
    map<pair<int, int>, int> mp;
    constexpr int T = 100000, S = 200000;
    int dy[] = {-1, 1};
    auto get_mid = [&](int i, int j, int d) {
        return be[i][j + d];
    };
    int sum = 0;
    for (int i = 0; i < h - 1; i++) {
        for (int j = 0; j < w - 1; j++) {
            if ((i + j) & 1) continue;
            mp[{i, j}] = idx;
            int B = be[i][j] + be[i + 1][j] + be[i][j + 1] + be[i + 1][j + 1];
            sum += B;
            if (i & 1) {
                push(S, idx, W);
                push(idx, T, B);
            } else {
                push(S, idx, B);
                push(idx, T, W);
            }
            for (int k = 0; k < 2; k++) {
                int x = i - 1;
                int y = j + dy[k];
                if (0 <= x && 0 <= y && y < w - 1) {
//                    cerr << "mp" << endl;
//                    for (auto &p : mp) {
//                        cerr << "(" << p.first.first << "," << p.first.second << ")" << endl;
//                    }
//                    cerr << "cur" << endl;
//                    cerr << "(" << i << "," << j << "), (" << x << "," << y << ")" << endl;
                    assert(mp.count({x, y}));
                    int mid = get_mid(i, j, k);
                    if (x & 1) {
                        push(idx, mp[{x, y}], mid);
                    } else {
                        push(mp[{x, y}], idx, mid);
                    }
                }
            }
            idx++;
        }
    }

    Dinic dinic(idx + 2);
    for (int i = 0; i < as.size(); i++) {
        int a = as[i], b = bs[i], c = cs[i];
        if (a == S) a = idx;
        if (b == T) b = idx + 1;
//        cout << "edge: " << a << ' ' << b << ' ' << c << endl;
        dinic.add_edge(a, b, c);
    }

    cout << sum - dinic.max_flow(idx, idx + 1) << endl;
}