// added

#include <bits/stdc++.h>
using namespace std;

class StrongHash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

public:
    int operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM) & INT_MAX;
    }
};

class LCA {
    int n;
    const int LOG = 20;
    vector<int> ds;
    vector<vector<int>> adj;
    vector<vector<int>> table;

    void dfs(int v, int p, int d) {
        ds[v] = d;
        table[v][0] = p;
        for (int s : adj[v]) {
            if (s != p) {
                dfs(s, v, d + 1);
            }
        }
    }

public:
    LCA(int n) : n(n), ds(n), adj(n), table(n, vector<int>(LOG, -1)) {}

    void add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    void build() {
        dfs(0, -1, 0);
        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i < n; i++) {
                if (table[i][k - 1] == -1) table[i][k] = -1;
                else table[i][k] = table[table[i][k - 1]][k - 1];
            }
        }
    }

    int lca(int u, int v) {
        if (ds[u] > ds[v]) swap(u, v);
        for (int i = LOG - 1; i >= 0; i--) {
            if (((ds[v] - ds[u]) >> i) & 1) v = table[v][i];
        }
        if (u == v) return u;
        for (int i = LOG - 1; i >= 0; i--) {
            if ((table[u][i] != table[v][i])) {
                u = table[u][i];
                v = table[v][i];
            }
        }
        return table[u][0];
    }

    int dist(int u, int v) {
        return ds[u] + ds[v] - 2 * ds[lca(u, v)];
    }

    // u->vパスでuから距離kにある頂点の番号
    int kth_node(int u, int v, int k) {
        if (k == 0) return u;
        if (k > dist(u, lca(u, v))) return kth_node(v, u, dist(u, v) - k);
        int w = u;
        for (int i = LOG - 1; i >= 0; i--) {
            if (1 << i <= k) {
                w = table[w][i];
                k -= 1 << i;
                if (k == 0) return w;
            }
        }
        assert(false);
    }
};

class PrimeUtil {
    mt19937 rnd;

    int modpow(int a, long long n, int mod) {
        if (n == 0) return 1;
        if (n & 1) return 1ll * a * modpow(a, n - 1, mod) % mod;
        int t = modpow(a, n / 2, mod);
        return 1ll * t * t % mod;
    }

public:
    PrimeUtil() : rnd(chrono::steady_clock::now().time_since_epoch().count()) {}

    bool is_prime(int n, int k = 50) {
        if (n == 2) return true;
        if (!(n & 1) || n < 2) return false;
        int d = n - 1;
        while (!(d & 1)) d >>= 1;
        for (int i = 0; i < k; i++) {
            int a = rnd() % (n - 2) + 1;
            int t = d;
            int y = modpow(a, t, n);
            while (t != n - 1 && y != 1 && y != n - 1) {
                y = modpow(y, 2, n);
                t <<= 1;
            }
            if (y != n - 1 && !(t & 1)) return false;
        }
        return true;
    }

    int gen_prime(int keta = 30) {
        assert(keta <= 30);
        while (1) {
            int d = rnd() & ((1 << keta) - 1);
            d |= 1;
            if (is_prime(d)) return d;
        }
    }
};

// https://snuke.hatenablog.com/entry/2017/02/03/054210
class TreeHash {
    int n, mod;
    vector<vector<pair<int, int>>> adj;
    vector<int> label;
    vector<int> Hash, Hashp;
    map<pair<int, int>, int> es;
    LCA lca;
    StrongHash hash;

    int modpow(int a, long long n, int mod) {
        if (n == 0) return 1;
        if (n & 1) return 1ll * a * modpow(a, n - 1, mod) % mod;
        int t = modpow(a, n / 2, mod);
        return 1ll * t * t % mod;
    }

    void dfs(int v, int p) {
        long long ret = hash(label[v]);
        for (auto &s : adj[v]) {
            if (s.first != p) {
                dfs(s.first, v);
                // h * hのところは適当
                ret += hash(s.second) + 1ll * Hash[s.first] * Hash[s.first] % mod;
            }
        }
        Hash[v] = ret % mod;
    }

    void dfs2(int v, int p) {
        for (auto &s : adj[v]) {
            if (s.first != p) {
                long long t = Hash[v];
                t -= hash(s.second) + 1ll * Hash[s.first] * Hash[s.first] % mod;
                t += Hashp[v];
                t %= mod;
                if (t < 0) t += mod;
                Hashp[s.first] = (hash(s.second) + t * t) % mod;
                dfs2(s.first, v);
            }
        }
    }

public:
    TreeHash(int n, int mod) : n(n), mod(mod), adj(n), label(n, 1), lca(n), Hash(n), Hashp(n) {}

    void add_edge(int a, int b, int w = 1) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        adj[a].emplace_back(b, w);
        adj[b].emplace_back(a, w);
        lca.add_edge(a, b);
        es[{min(a, b), max(a, b)}] = w;
    }

    void set_label(int v, int x) {
        assert(0 <= v && v < n);
        label[v] = x;
    }

    void build() {
        dfs(0, -1);
        dfs2(0, -1);
        lca.build();
    }

    // rootを根として見たときの頂点v以下の部分木のhash
    int get(int root, int v) {
        if (root == v) return Hash[v] + Hashp[v];
        if (lca.lca(root, v) == v) {
            int s = lca.kth_node(v, root, 1);
            int w = es[{min(s, v), max(s, v)}];
            long long t = Hash[v];
            t -= hash(w) + 1ll * Hash[s] * Hash[s] % mod;
            t += Hashp[v];
            t %= mod;
            if (t < 0) t += mod;
            return t;
        } else {
            return Hash[v];
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<vector<int>> adj(n);

    PrimeUtil pu;
    int p1 = pu.gen_prime(), p2 = pu.gen_prime();

    TreeHash hash1(n, p1), hash2(n, p2);

    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
        hash1.add_edge(a, b);
        hash2.add_edge(a, b);
    }
    hash1.build();
    hash2.build();

    for (int i = 0; i < n; i++) {
        cout << hash1.get(2, i) << endl;
    }
}
