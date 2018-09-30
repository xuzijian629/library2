#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;

template <typename T>
class AVL {
    int s = 0;
    struct node {
        T val;
        node *ch[2];
        int dep, size;
        node(T v, node* l = nullptr, node* r = nullptr) : val(v), dep(1), size(1) {
            ch[0] = l; ch[1] = r;
        }
    };
    int depth(node *t) { return t == nullptr ? 0 : t->dep; }
    int count(node *t) { return t == nullptr ? 0 : t->size; }
    node *update(node *t) {
        t->dep = max(depth(t->ch[0]), depth(t->ch[1])) + 1;
        t->size = count(t->ch[0]) + count(t->ch[1]) + 1;
        return t;
    }
    node *rotate(node *t, int b) {
        node *s = t->ch[1 - b];
        t->ch[1 - b] = s->ch[b];
        s->ch[b] = t;
        t = update(t);
        s = update(s);
        return s;
    }
    node *fetch(node *t) {
        if (t == nullptr) return t;
        if (depth(t->ch[0]) - depth(t->ch[1]) == 2) {
            if (depth(t->ch[0]->ch[1]) > depth(t->ch[0]->ch[0])) {
                t->ch[0] = rotate(t->ch[0], 0);
            }
            t = rotate(t, 1);
        }
        else if (depth(t->ch[0]) - depth(t->ch[1]) == -2) {
            if (depth(t->ch[1]->ch[0]) > depth(t->ch[1]->ch[1])) {
                t->ch[1] = rotate(t->ch[1], 1);
            }
            t = rotate(t, 0);
        }
        return t;
    }
    node *insert(node *t, int k, T v) {
        if (t == nullptr) return new node(v);
        int c = count(t->ch[0]), b = (k > c);
        t->ch[b] = insert(t->ch[b], k - (b ? (c + 1) : 0), v);
        update(t);
        return fetch(t);
    }
    node *erase(node *t) {
        if (t == nullptr) return nullptr;
        if (t->ch[0] == nullptr && t->ch[1] == nullptr) {
            delete t;
            return nullptr;
        }
        if (t->ch[0] == nullptr || t->ch[1] == nullptr) {
            node *res = t->ch[t->ch[0] == nullptr];
            delete t;
            return res;
        }
        node *res = new node(find(t->ch[1], 0)->val, t->ch[0], erase(t->ch[1], 0));
        delete t;
        return fetch(update(res));
    }
    node *erase(node *t, int k) {
        if (t == nullptr) return nullptr;
        int c = count(t->ch[0]);
        if (k < c) {
            t->ch[0] = erase(t->ch[0], k);
            t = update(t);
        }
        else if (k > c) {
            t->ch[1] = erase(t->ch[1], k - (c + 1));
            t = update(t);
        }
        else {
            t = erase(t);
        }
        return fetch(t);
    }
    node *find(node *t, int k) {
        if (t == nullptr) return t;
        int c = count(t->ch[0]);
        return k < c ? find(t->ch[0], k) : k == c ? t : find(t->ch[1], k - (c + 1));
    }
    int cnt(node *t, T v) {
        if (t == nullptr) return 0;
        if (t->val < v) return count(t->ch[0]) + 1 + cnt(t->ch[1], v);
        if (t->val == v) return count(t->ch[0]);
        return cnt(t->ch[0], v);
    }
    node *root;
public:
    AVL() : root(nullptr) {}
    void insert(T val) {
        root = insert(root, cnt(root, val), val);
        s++;
    }
    
    // 0-indexed
    T get_kth_small(int k) {
        assert(k < s);
        return find(root, k)->val;
    }
    // 0-indexed
    void erase_kth_small(int k) {
        assert(k < s);
        root = erase(root, k);
        s--;
    }
    
    // 0-indexed
    T get_kth_big(int k) {
        assert(k < s);
        return get_kth_small(s - 1 - k);
    }
    // 0-indexed
    void erase_kth_big(int k) {
        assert(k < s);
        erase_kth_small(s - 1 - k);
    }
    
    int count(T val) {
        return cnt(root, val);
    }
    int size() {
        return s;
    }
    bool empty() {
        return s == 0;
    }
    
    int count_lt(T val) {
        int l = -1, r = s;
        while (r - l > 1) {
            int m = l + (r - l) / 2;
            if (get_kth_small(m) < val) {
                l = m;
            } else {
                r = m;
            }
        }
        return r;
    }
    int count_le(T val) {
        int l = -1, r = s;
        while (r - l > 1) {
            int m = l + (r - l) / 2;
            if (get_kth_small(m) <= val) {
                l = m;
            } else {
                r = m;
            }
        }
        return r;
    }
    int count_gt(T val) {
        int l = -1, r = s;
        while (r - l > 1) {
            int m = l + (r - l) / 2;
            if (get_kth_big(m) > val) {
                l = m;
            } else {
                r = m;
            }
        }
        return r;
    }
    int count_ge(T val) {
        int l = -1, r = s;
        while (r - l > 1) {
            int m = l + (r - l) / 2;
            if (get_kth_big(m) >= val) {
                l = m;
            } else {
                r = m;
            }
        }
        return r;
    }
    
    void erase(T val) {
        int kth = count_lt(val);
        assert(get_kth_small(kth) == val);
        erase_kth_small(kth);
    }
};

int main() {
    int n;
    cin >> n;
    AVL<i64> avl;
    for (int i = 0; i < n; i++) {
        int cmd;
        cin >> cmd;
        if (cmd > 0) {
            int k;
            cin >> k;
            avl.insert(k);
        } else if (cmd < 0) {
            int k;
            cin >> k;
            avl.erase(k);
        } else {
            int k;
            cin >> k;
            cout << avl.get_kth_big(k - 1) << endl;
        }
    }
}
