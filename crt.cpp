#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

i64 modpow(i64 a, i64 n, i64 mod) {
    if (n == 0) return 1;
    if (n % 2 == 0) {
        i64 t = modpow(a, n / 2, mod);
        return t * t % mod;
    }
    return a * modpow(a, n - 1, mod) % mod;
}

i64 modinv(i64 a, i64 mod) {
    return modpow(a, mod - 2, mod);
}

pair<i64, i64> chrem(vi ps, vi rs) {
    using Long = __int128_t;
    i64 P = 1;
    for (i64 p: ps) {
        P *= p;
    }
    Long ret = 0;
    for (int i = 0; i < ps.size(); i++) {
        i64 p = P / ps[i];
        ret += Long(1) * rs[i] * modinv(p, ps[i]) * p;
    }

    return make_pair(ret % P, P);
}

int main() {
    vi rs(3);
    for (int i = 0; i < 3; i++) {
        cin >> rs[i];
    }

    auto p = chrem({17, 107, (int) 1e9 + 7}, rs);
    cout << p.first << '\n';
}

