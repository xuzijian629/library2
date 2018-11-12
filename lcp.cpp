#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

template <class T>
class SegTree {
    using F = function<T(T, T)>;
    int n;
    vector<T> data;
    F f;
    T e;

public:
    SegTree<T>() {}
    SegTree<T>& operator=(const SegTree<T>& t) {
        n = t.n;
        data = t.data;
        f = t.f;
        e = t.e;
        return *this;
    }

    SegTree<T>(const vector<T>& as, const F f, const T& e) : f(f), e(e) {
        n = 1;
        while (n < as.size()) n <<= 1;
        data.assign(2 * n, e);
        for (int i = 0; i < as.size(); i++) {
            data[n + i] = as[i];
        }
        for (int i = n - 1; i > 0; i--) {
            data[i] = f(data[2 * i], data[2 * i + 1]);
        }
    }

    void update(int k, const T& x) {
        k += n;
        data[k] = x;
        while (k >>= 1) {
            data[k] = f(data[2 * k], data[2 * k + 1]);
        }
    }

    T query(int a, int b) {
        T L = e, R = e;
        for (a += n, b += n; a < b; a >>= 1, b >>= 1) {
            if (a & 1) {
                L = f(L, data[a++]);
            }
            if (b & 1) {
                R = f(data[--b], R);
            }
        }
        return f(L, R);
    }
};

class SuffixArray {
    vi sa_is(const vi& arr, const int k) {
        const int n = arr.size();
        vector<bool> is_S(n);
        is_S[n - 1] = 1;
        vector<bool> is_LMS(n);
        vi LMSs;
        for (int i = n - 2; i >= 0; i--) {
            is_S[i] = arr[i] < arr[i + 1] || (arr[i] == arr[i + 1] && is_S[i + 1]);
        }
        for (int i = 0; i < n; i++) {
            if (is_S[i] && (i == 0 || !is_S[i - 1])) {
                is_LMS[i] = 1;
                LMSs.push_back(i);
            }
        }
        vi pseudo_sa = induced_sort(arr, LMSs, is_S, k);
        vi orderedLMSs(LMSs.size());
        int id = 0;
        for (int x: pseudo_sa) {
            if (is_LMS[x]) {
                orderedLMSs[id++] = x;
            }
        }
        pseudo_sa[orderedLMSs[0]] = 0;
        int rk = 0;
        if (orderedLMSs.size() > 1) {
            pseudo_sa[orderedLMSs[1]] = ++rk;
        }
        for (int i = 1; i < orderedLMSs.size() - 1; i++) {
            bool is_diff = 0;
            for (int j = 0; j < n; j++) {
                int p = orderedLMSs[i] + j;
                int q = orderedLMSs[i + 1] + j;
                if (arr[p] != arr[q] || is_LMS[p] != is_LMS[q]) {
                    is_diff = 1;
                    break;
                }
                if (j > 0 && is_LMS[p]) {
                    break;
                }
            }
            pseudo_sa[orderedLMSs[i + 1]] = is_diff ? ++rk : rk;
        }
        vi new_arr(LMSs.size());
        id = 0;
        for (int i = 0; i < n; i++) {
            if (is_LMS[i]) {
                new_arr[id++] = pseudo_sa[i];
            }
        }
        vi LMS_sa;
        if (rk + 1 == LMSs.size()) {
            LMS_sa = orderedLMSs;
        } else {
            LMS_sa = sa_is(new_arr, rk + 1);
            for (auto& x: LMS_sa) {
                x = LMSs[x];
            }
        }
        return induced_sort(arr, LMS_sa, is_S, k);
    }

    vi induced_sort(const vi& arr, const vi& LMSs, const vector<bool>& is_S, const int k) {
        const int n = arr.size();
        vi buckets(n);
        vi cs(k + 1);
        for (int c: arr) {
            cs[c + 1]++;
        }
        for (int i = 0; i < k; i++) {
            cs[i + 1] += cs[i];
        }
        vi cnt(k);
        for (int i = LMSs.size() - 1; i >= 0; i--) {
            int c = arr[LMSs[i]];
            buckets[cs[c + 1] - 1 - cnt[c]++] = LMSs[i];
        }
        cnt = vi(k);
        for (int i = 0; i < n; i++) {
            if (buckets[i] == 0 || is_S[buckets[i] - 1]) continue;
            int c = arr[buckets[i] - 1];
            buckets[cs[c] + cnt[c]++] = buckets[i] - 1;
        }
        cnt = vi(k);
        for (int i = n - 1; i >= 0; i--) {
            if (buckets[i] == 0 || !is_S[buckets[i] - 1]) continue;
            int c = arr[buckets[i] - 1];
            buckets[cs[c + 1] - 1 - cnt[c]++] = buckets[i] - 1;
        }
        return buckets;
    }

public:
    string S;
    int N;

    vi sa;
    // sa[i]: suffixが辞書順i番目となる開始位置のindex

    SuffixArray(string s) : S(s), N(s.size()) {
        s += '$';
        vi str(N + 1);
        for (int i = 0; i <= N; i++) {
            str[i] = s[i] - '$';
        }
        sa = sa_is(str, 128);
        sa.erase(sa.begin());
    }

    int operator[](int id) {
        return sa[id];
    }

    vi::iterator lower_bound(string s) {
        // sizeがsと等しく初めてs以上になるようなSの部分文字列(sa)
        int l = -1, r = N;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if (S.compare(sa[m], s.size(), s) < 0) {
                l = m;
            } else {
                r = m;
            }
        }
        return sa.begin() + r;
    }
    
    vi::iterator upper_bound(string s) {
        // sizeがsと等しく初めてsより大きくなるようなSの部分文字列(sa)
        int l = -1, r = N;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if (S.compare(sa[m], s.size(), s) <= 0) {
                l = m;
            } else {
                r = m;
            }
        }
        return sa.begin() + r;
    }

    int cntsub(string s) {
        // sが部分文字列として何回出現するか
        return upper_bound(s) - lower_bound(s);
    }
};

class LCP {
    SegTree<i64> segtree;
    vi lcp, lcp_begin;
public:
    const string S;
    int N;
    vi sa, rank;
    LCP(const string& s) : S(s), N(s.size()), rank(s.size()), lcp(s.size()), lcp_begin(s.size()) {
        sa = SuffixArray(s).sa;
        for (int i = 0; i < N; i++) {
            rank[sa[i]] = i;
        }
        int h = 0;
        for (int i = 0; i < N; i++) {
            if (h > 0) h--;
            if (rank[i] == N - 1) continue;
            int j = sa[rank[i] + 1];
            for (; i + h < N && j + h < N; h++) {
                if (S[i + h] != S[j + h]) break;
            }
            lcp[rank[i]] = h;
        }

        lcp_begin[0] = N;
        for (int i = rank[0] - 1; i >= 0; i--) {
            lcp_begin[sa[i]] = min(lcp_begin[sa[i + 1]], lcp[i]);
        }
        for (int i = rank[0] + 1; i < N; i++) {
            lcp_begin[sa[i]] = min(lcp_begin[sa[i - 1]], lcp[i - 1]);
        }

        segtree = SegTree<i64>(lcp, [](i64 a, i64 b){return min(a, b);}, 1e18);
    }

    // S[i..], S[j..]が先頭何文字一致しているか
    int query(int i, int j) {
        assert(0 <= i && i < N && 0 <= j && j < N);
        if (i == j) return N - i;
        return segtree.query(min(rank[i], rank[j]), max(rank[i], rank[j]));
    }
};

int main() {
    string s;
    cin >> s;
    string t(s);
    reverse(t.begin(), t.end());
    LCP lcp(s), lcp_back(t);
    int n = s.size();
    i64 ans = 0;
    for (int i = 3; i < n - 1; i++) {
        int x = i, y = n - i;
        if (x <= y) continue;
        int a = min(y - 1, lcp.query(0, x));
        int c = min(y - 1, lcp_back.query(0, y));
        if (a > 0 && c > 0 && a + c >= y) {
            ans += a + c - y + 1;
        }
    }
    cout << ans << endl;
}