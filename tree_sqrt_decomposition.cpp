// iran
// verified LCA: https://onlinejudge.u-aizu.ac.jp/problems/GRL_5_C

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class TreeSqrtDecomp {
    int n, B;
    int size() const { return n; }
    // block_id[v]: vが属するblock
    // block_parent[v]: vが属するblockのparent(vと同じblockには入らない)
    vi block_id, parent, depth, block_parent, tops, visited;
    vvi adj;
public:
    vector<set<int>> block_nodes;
    TreeSqrtDecomp(int n_) : n(n_), adj(n) {
        int l = 0, r = n;
        while (l < r - 1) {
            i64 m = (l + r) / 2;
            if (m * m >= n) {
                r = m;
            } else {
                l = m;
            }
        }
        B = r;
    }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void decomp(int root = 0) {
        block_id.assign(n, -1);
        parent.assign(n, -1);
        depth.assign(n, 0);
        block_parent.assign(n, -1);
        visited.assign(n, 0);
        vi q(n + 1);
        int l = 0, r = 0;
        q[r++] = root;
        while (l != r) {
            int v = q[l++];
            visited[v] = 1;
            tops.push_back(v);
            for (int c : adj[v]) {
                if (!visited[c]) {
                    parent[c] = v;
                    depth[c] = depth[v] + 1;
                    q[r++] = c;
                }
            }
        }
        int cnt = 0;
        for (int u: tops) {
            if (block_id[u] != -1) continue;
            l = 0, r = 0;
            q[r++] = u;
            for (int i = 0; i < B && l != r; i++) {
                int v = q[l++];
                block_id[v] = cnt;
                block_parent[v] = parent[u];
                for (int c: adj[v]) {
                    if (block_id[c] == -1) {
                        q[r++] = c;
                    }
                }
            }
            cnt++;
        }
        block_nodes = vector<set<int>>(cnt);
        for (int i = 0; i < n; i++) {
            block_nodes[block_id[i]].insert(i);
        }
    }
    int get_block_parent(int node) {
        return block_parent[node];
    }
    int get_block_parent_by_id(int block_id) {
        // if block_id is 0, return -1
        if (block_id == 0) return -1;
        return get_block_parent(tops[block_id - 1]);
    }
    int get_top_depth(int block_id) {
        if (block_id == 0) return 0;
        return depth[tops[block_id - 1]];
    }
    int get_block_top_depth(int node) {
        return get_top_depth(block_id[node]);
    }
    int lca(int u, int v) {
        while (get_block_top_depth(u) > get_block_top_depth(v)) u = block_parent[u];
        while (get_block_top_depth(u) < get_block_top_depth(v)) v = block_parent[v];
        while (depth[u] > depth[v]) u = parent[u];
        while (depth[u] < depth[v]) v = parent[v];
        while (u != v) u = parent[u], v = parent[v];
        return u;
    }
    int block_cnt() {
        return tops.size();
    }
    int dist(int u, int v) {
        int w = lca(u, v);
        return depth[u] - depth[w] + depth[v] - depth[w];
    }
    bool in_same_block(int u, int v) {
        return get_block_parent(u) == get_block_parent(v);
    }
    int get_block_id(int node) {
        return block_id[node];
    }
    int get_parent(int node) {
        return parent[node];
    }
    int get_depth(int node) {
        return depth[node];
    }
};

int main() {
    int n;
    cin >> n;
    TreeSqrtDecomp tree(n);
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;
        for (int j = 0; j < k; j++) {
            int a;
            cin >> a;
            tree.addEdge(i, a);
        }
    }
    tree.decomp();
    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << tree.lca(u, v) << endl;
    }
}
