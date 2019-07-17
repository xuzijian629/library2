// nazo

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

struct FullyIndexableDictionary {
    int len, blk;
    vi bit, sum;

    FullyIndexableDictionary() {}
    FullyIndexableDictionary(int len) : len(len), blk((len + 31) >> 5), bit(blk, 0), sum(blk, 0) {}

    void set(int k) {
        bit[k >> 5] |= 1 << (k & 31);
    }

    void build() {
        sum[0] = 0;
        for (int i = 1; i < blk; i++) {
            sum[i] = sum[i - 1] + __builtin_popcount(bit[i - 1]);
        }
    }

    bool operator[](int k) const {
        return bool((bit[k >> 5] >> (k & 31)) & 1);
    }

    int rank(int k) {
        return sum[k >> 5] + __builtin_popcount(bit[k >> 5] & ((1 << (k & 31)) - 1));
    }

    int rank(bool v, int k) {
        return (v ? rank(k) : k - rank(k));
    }

    int select(bool v, int k) {
        if (k < 0 || rank(v, len) <= k) return -1;
        int low = 0, high = len;
        while (low + 1 < high) {
            int mid = (low + high) / 2;
            if (rank(v, mid) >= k + 1) {
                high = mid;
            } else {
                low = mid;
            }
        }
        return low;
    }

    int select(bool v, int i, int l) {
        return select(v, i + rank(v, l));
    }
};

template<class T, int MAXLOG>
class WaveletMatrix {
    int len;
    FullyIndexableDictionary mat[MAXLOG];
    int zs[MAXLOG], buff1[MAXLOG], buff2[MAXLOG];
    static const T npos = -1;

    int freq_dfs(int d, int l, int r, T val, T a, T b) {
        
    }
}