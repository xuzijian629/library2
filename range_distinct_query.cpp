// online版がある
// https://www.spoj.com/problems/DQUERY/

#include <bits/stdc++.h>

using namespace std;

class BIT {
    int n, N;
    vector<int> data;

    int sum(int i) {
        int s = 0;
        while (i > 0) {
            s += data[i];
            i -= i & -i;
        }
        return s;
    }

public:
    BIT(int n) : n(n), data(n + 1) {
        N = 1;
        while (N <= n) N <<= 1;
        N >>= 1;
    }

    void add(int i, int x) {
        i++;
        while (i <= n) {
            data[i] += x;
            i += i & -i;
        }
    }

    // [l, r)
    int sum(int l, int r) {
        return sum(r) - sum(l);
    }
};

struct OfflineRangeDistinctQuery {
    int MAX = 0;
    int n;
    vector<int> as;
    vector<int> last_visit;
    BIT bit;
    vector<array<int, 4>> queries;
    int qcnt = 0;
    OfflineRangeDistinctQuery(const vector<int> &as, int MAX) : n(as.size()), as(as), MAX(MAX), last_visit(MAX + 1, -1), bit(n) {}

    // [l, r]閉区間に対するクエリ。0-indexed
    void add_query(int l, int r) {
        array<int, 4> arr = {qcnt++, l, r, 0};
        queries.emplace_back(arr);
    }
    vector<int> answer() {
        sort(queries.begin(), queries.end(), [](auto &a, auto &b) {
            return a[2] < b[2];
        });
        int idx = 0;
        for (int i = 0; i < n; i++) {
            if (last_visit[as[i]] != -1) {
                bit.add(last_visit[as[i]], -1);
            }
            last_visit[as[i]] = i;
            bit.add(i, 1);

            while (idx < qcnt && queries[idx][2] == i) {
                queries[idx][3] = bit.sum(queries[idx][1], queries[idx][2] + 1);
                idx++;
            }
        }
        sort(queries.begin(), queries.end(), [](auto &a, auto &b) {
            return a[0] < b[0];
        });
        vector<int> ret(qcnt);
        for (int i = 0; i < qcnt; i++) {
            ret[i] = queries[i][3];
        }
        return ret;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<int> as(n);
    for (int i = 0; i < n; i++) cin >> as[i];
    OfflineRangeDistinctQuery rdq(as, 1000000);
    int q;
    cin >> q;
    while (q--) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        rdq.add_query(a, b);
    }
    vector<int> ans = rdq.answer();
    for (int a : ans) {
        cout << a << '\n';
    }
}