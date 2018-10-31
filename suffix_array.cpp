#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

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

int main() {
    i64 cnt = 0;
    string s;
    cin >> s;
    SuffixArray sa(s);
    int m;
    cin >> m;
    while (m--) {
        string t;
        cin >> t;
        cnt += sa.cntsub(t);
    }
    cout << cnt << endl;
}