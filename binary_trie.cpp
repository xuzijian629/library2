#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template<typename U = unsigned, int B = 32>
class lazy_binary_trie {
    struct node {
        int cnt;
        U lazy;
        node *ch[2];
        node() : cnt(0), lazy(0), ch{ nullptr, nullptr } {}
    };
    void push(node* t, int b) {
        if ((t->lazy >> (U)b) & (U)1) swap(t->ch[0], t->ch[1]);
        if (t->ch[0]) t->ch[0]->lazy ^= t->lazy;
        if (t->ch[1]) t->ch[1]->lazy ^= t->lazy;
        t->lazy = 0;
    }
    node* add(node* t, U val, int b = B - 1) {
        if (!t) t = new node;
        t->cnt += 1;
        if (b < 0) return t;
        push(t, b);
        bool f = (val >> (U)b) & (U)1;
        t->ch[f] = add(t->ch[f], val, b - 1);
        return t;
    }
    node* sub(node* t, U val, int b = B - 1) {
        assert(t);
        t->cnt -= 1;
        if (t->cnt == 0) return nullptr;
        if (b < 0) return t;
        push(t, b);
        bool f = (val >> (U)b) & (U)1;
        t->ch[f] = sub(t->ch[f], val, b - 1);
        return t;
    }
    U get_min(node* t, U val, int b = B - 1) {
        assert(t);
        if (b < 0) return 0;
        push(t, b);
        bool f = (val >> (U)b) & (U)1; f ^= !t->ch[f];
        return get_min(t->ch[f], val, b - 1) | ((U)f << (U)b);
    }
    U get(node* t, int k, int b = B - 1) {
        if (b < 0) return 0;
        push(t, b);
        int m = t->ch[0] ? t->ch[0]->cnt : 0;
        return k < m ? get(t->ch[0], k, b - 1) : get(t->ch[1], k - m, b - 1) | ((U)1 << (U)b);
    }
    int count_lower(node* t, U val, int b = B - 1) {
        if (!t || b < 0) return 0;
        push(t, b);
        bool f = (val >> (U)b) & (U)1;
        return (f && t->ch[0] ? t->ch[0]->cnt : 0) + count_lower(t->ch[f], val, b - 1);
    }
    node *root;
public:
    lazy_binary_trie() : root(nullptr) {}
    int size() const {
        return root ? root->cnt : 0;
    }
    bool empty() const {
        return !root;
    }
    void insert(U val) {
        root = add(root, val);
    }
    void erase(U val) {
        root = sub(root, val);
    }
    void xor_all(U val) {
        if (root) root->lazy ^= val;
    }
    U max_element(U bias = 0) {
        return get_min(root, ~bias);
    }
    U min_element(U bias = 0) {
        return get_min(root, bias);
    }
    int lower_bound(U val) { // return id
        return count_lower(root, val);
    }
    int upper_bound(U val) { // return id
        return count_lower(root, val + 1);
    }
    U operator[](int k) {
        assert(0 <= k && k < size());
        return get(root, k);
    }
    int count(U val) {
        if (!root) return 0;
        node *t = root;
        for (int i = B - 1; i >= 0; i--) {
            push(t, i);
            t = t->ch[(val >> (U)i) & (U)1];
            if (!t) return 0;
        }
        return t->cnt;
    }
};

signed main() {
    int n, x;
    cin >> n >> x;
    lazy_binary_trie<> bt;
    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        adj[a].emplace_back(b, c);
        adj[b].emplace_back(a, c);
    }
    vector<int> xr(n);
    function<void(int, int, int)> dfs = [&](int v, int p, int x) {
        xr[v] = x;
        for (auto& e : adj[v]) {
            if (e.first != p) {
                dfs(e.first, v, x ^ e.second);
            }
        }
    };
    dfs(0, -1, 0);

    for (int i = 0; i < n; i++) {
        bt.insert(xr[i]);
    }

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        ans += bt.count(x ^ xr[i]);
    }

    cout << (x ? ans / 2 : (ans - n) / 2) << endl;
}
