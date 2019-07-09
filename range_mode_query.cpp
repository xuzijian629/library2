#include <bits/stdc++.h>
using namespace std;

struct Mo {
    int block;
    vector<int> order, le, ri;
    vector<bool> visited;
    function<void(int)> add, del;
    function<int()> get;
    int nl, nr;
    Mo() {}
    Mo(int n, function<void(int)> add, function<void(int)> del, function<int()> get) : block((int) sqrt(n)), nl(0), nr(0), visited(n), add(move(add)), del(move(del)), get(move(get)) {};

    // [l, r)
    void add_query(int l, int r) {
        le.push_back(l);
        ri.push_back(r);
    }

    void shift(int idx) {
        visited[idx].flip();
        if (visited[idx]) add(idx);
        else del(idx);
    }

    vector<int> answer() {
        int qcnt = le.size();
        order.resize(qcnt);
        vector<int> ret(qcnt);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return le[a] / block != le[b] / block ? le[a] < le[b] : ri[a] < ri[b];
        });
        for (int i = 0; i < qcnt; i++) {
            int idx = order[i];
            while (nl > le[idx]) shift(--nl);
            while (nr < ri[idx]) shift(nr++);
            while (nl < le[idx]) shift(nl++);
            while (nr > ri[idx]) shift(--nr);
            ret[order[i]] = get();
        }
        return ret;
    }
};

struct OfflineRangeModeQuery {
    int n;
    int MAX;
    vector<int> as;
    // cnt[a]: 数aの現在のカウント
    // cntrev[k]: カウントがkの個数が何種類あるか
    // best: 現在のmodeのカウント
    vector<int> cnt, cntrev;
    int best = 0;
    vector<array<int, 4>> queries;
    Mo mo;
    OfflineRangeModeQuery(const vector<int> &as, int MAX) : n(as.size()), as(as), MAX(MAX), cnt(MAX + 1), cntrev(n + 1) {
        auto add = [&](int idx) {
            cntrev[cnt[as[idx]]]--;
            cnt[as[idx]]++;
            cntrev[cnt[as[idx]]]++;
            if (cnt[as[idx]] > best) best++;
        };
        auto del = [&](int idx) {
            cntrev[cnt[as[idx]]]--;
            if (cnt[as[idx]] == best && cntrev[best] == 0) best--;
            cnt[as[idx]]--;
            cntrev[cnt[as[idx]]]++;
        };
        auto ans = [&]() {
            return best;
        };
        mo = Mo(n, add, del, ans);
    }

    // [l, r]閉区間!!!!!!!!!! 0-indexed
    void add_query(int l, int r) {
        mo.add_query(l, r + 1);
    }
    vector<int> answer() {
        return mo.answer();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    while (1) {
        int n;
        cin >> n;
        if (n == 0) break;
        int q;
        cin >> q;
        vector<int> as(n);
        for (int i = 0; i < n; i++) cin >> as[i];
        for (int i = 0; i < n; i++) as[i] += 100000;
        OfflineRangeModeQuery rmq(as, 200000);
        while (q--) {
            int l, r;
            cin >> l >> r;
            l--, r--;
            rmq.add_query(l, r);
        }
        vector<int> ans = rmq.answer();
        for (int a : ans) {
            cout << a << '\n';
        }
    }
}