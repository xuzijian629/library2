#include <bits/stdc++.h>
using namespace std;
constexpr int MAX_N = 202020;
constexpr int MAX_ROOT = 8000000;

// https://github.com/anh1l1ator/Spoj/blob/master/Desktop/Codes/DQUERY_Online.cpp
struct Node {
    int cnt, L, R;
    Node() {
        cnt = 0;
        L = R = -1;
    }
    Node(int x, int y, int z) {
        L = x, R = y, cnt = z;
    }
} tree[MAX_ROOT];

class DistinctElements {
    int gc = 0, N;
    vector<int> rt;

    int build(int L, int R) {
        gc++;
        if (L == R) return gc;
        int x = gc;
        tree[x] = Node(build(L, (L + R) / 2), build((L + R) / 2 + 1, R), 0);
        return x;
    }

    int update(int L, int R, int root, int idx, int val) {
        assert(root < MAX_ROOT);
        if (L > idx || R < idx) {
            return root;
        }
        gc++;
        if (L == idx && R == idx) {
            tree[gc] = Node(-1, -1, tree[root].cnt + val);
            return gc;
        }
        int x = gc;
        tree[x] = Node(update(L, (L + R) / 2, tree[root].L, idx, val), update((L + R) / 2 + 1, R, tree[root].R, idx, val), tree[root].cnt + val);
        return x;
    }

    int query(int L, int R, int root, int ql, int qr) {
        if (qr < L || ql > R) return 0;
        if (ql <= L && R <= qr) {
            return tree[root].cnt;
        }
        return query(L, (L + R) / 2, tree[root].L, ql, qr) + query((L + R) / 2 + 1, R, tree[root].R, ql, qr);
    }

    int query1idx(int l, int r) {
        return query(1, N, rt[r], l, r);
    }

public:
    DistinctElements(const vector<int>& as) {
        N = as.size();
        rt = vector<int>(MAX_N);
        vector<int> G(1 << 20, -1);
        rt[0] = build(1, N);
        for (int i = 1; i <= N; i++) {
            int p = rt[i - 1];
            if (G[as[i - 1]] != -1) {
                p = update(1, N, p, G[as[i - 1]], -1);
            }
            rt[i] = update(1, N, p, i, 1);
            G[as[i - 1]] = i;
        }
    }

    int query(int l, int r) {
        return query1idx(l + 1, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int n;
    cin >> n;
    vector<int> as(n);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }
    DistinctElements ds(as);
    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        l--;
        cout << ds.query(l, r) << '\n';
    }
}
