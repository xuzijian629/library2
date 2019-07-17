// added

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template<typename flow_t = int, typename cost_t = int>
struct PrimalDual {
    const cost_t INF;
    struct edge {
        int to;
        flow_t cap;
        cost_t cost;
        int rev;
        bool isrev;
    };
    vector<vector<edge>> graph;
    vector<cost_t> potential, min_cost;
    vector<int> prevv, preve;

    PrimalDual(int v) : graph(v), INF(numeric_limits<cost_t>::max()) {}
    void add_edge(int from, int to, flow_t cap, cost_t cost) {
        graph[from].emplace_back((edge) {to, cap, cost, (int) graph[to].size(), false});
        graph[to].emplace_back((edge) {from, 0, -cost, (int) graph[from].size() - 1, true});
    }

    cost_t min_cost_flow(int s, int t, flow_t f) {
        int V = (int) graph.size();
        cost_t ret = 0;
        priority_queue<pair<cost_t, int>, vector<pair<cost_t, int>>, greater<pair<cost_t, int>>> que;
        potential.assign(V, 0);
        prevv.assign(V, -1);
        preve.assign(V, -1);

        while (f > 0) {
            min_cost.assign(V, INF);
            que.emplace(0, s);
            min_cost[s] = 0;
            while (!que.empty()) {
                pair<cost_t, int> p = que.top();
                que.pop();
                if (min_cost[p.second] < p.first) continue;
                for (int i = 0; i < graph[p.second].size(); i++) {
                    edge& e = graph[p.second][i];
                    cost_t next_cost = min_cost[p.second] + e.cost + potential[p.second] - potential[e.to];
                    if (e.cap > 0 && min_cost[e.to] > next_cost) {
                        min_cost[e.to] = next_cost;
                        prevv[e.to] = p.second, preve[e.to] = i;
                        que.emplace(min_cost[e.to], e.to);
                    }
                }
            }
            if (min_cost[t] == INF) return -1;
            for (int v = 0; v < V; v++) {
                potential[v] += min_cost[v];
            }
            flow_t addflow = f;
            for (int v = t; v != s; v = prevv[v]) {
                addflow = min(addflow, graph[prevv[v]][preve[v]].cap);
            }
            f -= addflow;
            ret += addflow * potential[t];
            for (int v = t; v != s; v = prevv[v]) {
                edge& e = graph[prevv[v]][preve[v]];
                e.cap -= addflow;
                graph[v][e.rev].cap += addflow;
            }
        }
        return ret;
    }
};

int main() {
    int n;
    cin >> n;
    vector<vector<int>> board(n, vector<int>(n));
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char c;
            cin >> c;
            board[i][j] = c == '.';
            if (c == '.') cnt++;
        }
    }

    PrimalDual<> mcf(2 * n * n + 2);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mcf.add_edge(0, 1 + i * n + j, n + 1, 1);
            if (board[i][j]) {
                mcf.add_edge(1 + n * n + i * n + j, 2 * n * n + 1, 1, 0);
            }
        }
    }

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            int S = 1 + r * n + c;
            for (int j = 0; j <= c; j++) {
                int T = 1 + n * n + r * n + j;
                mcf.add_edge(S, T, 1, 0);
            }
            for (int j = c; j < n; j++) {
                if (r + 1 < n) {
                    int T = 1 + n * n + (r + 1) * n + j;
                    mcf.add_edge(S, T, 1, 0);
                }
            }
        }
    }

    cout << mcf.min_cost_flow(0, 2 * n * n + 1, cnt) << endl;
}
