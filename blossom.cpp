#include <bits/stdc++.h>
using namespace std;
#define int long long

class Blossom {
    // 1-based vertex index
    vector<int> vis, par, orig, match, aux;
    int t = 0, N;
    vector<vector<int>> conn;
    queue<int> Q;

    void augment(int u, int v) {
        int pv = v, nv;
        do {
            pv = par[v], nv = match[pv];
            match[v] = pv, match[pv] = v;
            v = nv;
        } while (u != pv);
    }
    int lca(int v, int w) {
        ++t;
        while (1) {
            if (v) {
                if (aux[v] == t) {
                    return v;
                }
                aux[v] = t;
                v = orig[par[match[v]]];
            }
            swap(v, w);
        }
    }

    void blossom(int v, int w, int a) {
        while (orig[v] != a) {
            par[v] = w;
            w = match[v];
            if (vis[w] == 1) {
                Q.push(w), vis[w] = 0;
            }
            orig[v] = orig[w] = a;
            v = par[w];
        }
    }

    bool bfs(int u) {
        vis.assign(N + 1, -1);
        iota(orig.begin(), orig.end(), 0);
        Q = queue<int>();
        Q.push(u);
        vis[u] = 0;
        while (Q.size()) {
            int v = Q.front();
            Q.pop();
            for (int x : conn[v]) {
                if (vis[x] == -1) {
                    par[x] = v;
                    vis[x] = 1;
                    if (!match[x]) {
                        return augment(u, x), true;
                    }
                    Q.push(match[x]);
                    vis[match[x]] = 0;
                } else if (vis[x] == 0 && orig[v] != orig[x]) {
                    int a = lca(orig[v], orig[x]);
                    blossom(x, v, a);
                    blossom(v, x, a);
                }
            }
        }
        return false;
    }

public:
    Blossom(int n) : vis(n + 1), par(n + 1), orig(n + 1), match(n + 1), aux(n + 1), N(n), conn(n + 1) {}

    void addEdge(int u, int v) {
        assert(1 <= u && u <= N && 1 <= v && v <= N);
        conn[u].push_back(v);
        conn[v].push_back(u);
    }

    int Match() {
        int ans = 0;
        vector<int> V(N - 1);
        iota(V.begin(), V.end(), 1);
        shuffle(V.begin(), V.end(), mt19937(chrono::steady_clock::now().time_since_epoch().count()));
        for (int x : V) {
            if (!match[x]) {
                for (int y : conn[x]) {
                    if (!match[y]) {
                        match[x] = y;
                        match[y] = x;
                        ++ans;
                        break;
                    }
                }
            }
        }
        for (int i = 1; i <= N; i++) {
            if (!match[i] && bfs(i)) ++ans;
        }
        return ans;
    }
};

struct MaximizingGCD {
    int maximumGCDPairing(vector<int> as) {
        int n = as.size();
        int sum = 0;
        for (int a : as) {
            sum += a;
        }
        set<int> divs;
        for (int i = 1; i * i <= sum; i++) {
            if (sum % i == 0) {
                divs.insert(i);
                divs.insert(sum / i);
            }
        }

        vector<int> ds;
        for (int d : divs) {
            ds.push_back(d);
        }

        reverse(ds.begin(), ds.end());

        auto ok = [&](int d) {
            Blossom b(n);
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (i == j) continue;
                    if ((as[i] + as[j]) % d == 0) {
                        b.addEdge(i + 1, j + 1);
                    }
                }
            }
            if (b.Match() == n / 2) {
                return true;
            }
            return false;
        };

        for (int d : ds) {
            if (ok(d)) {
                return d;
            }
        }
        assert(0);
    }
};

int32_t main() {
    MaximizingGCD solve;
    vector<int> as = {505, 796395095, 891890495, 554953895, 808647335, 581980895, 808647335, 808647335, 655854695, 953512055, 459458495, 943421975, 514593575, 714713495, 814773455, 892370975, 843241895, 796395095, 349188335, 349188335, 554953895, 937295855, 972971495, 673872695, 810809495, 892370975, 843241895, 972971495, 972971495, 937295855};
    cout << solve.maximumGCDPairing(as) << endl;
}