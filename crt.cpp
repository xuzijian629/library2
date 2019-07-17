#include <bits/stdc++.h>

using namespace std;

int modpow(int a, int n, int mod) {
    if (n == 0) return 1;
    if (n % 2 == 0) {
        int t = modpow(a, n / 2, mod);
        return t * t % mod;
    }
    return a * modpow(a, n - 1, mod) % mod;
}

int modinv(int a, int mod) {
    return modpow(a, mod - 2, mod);
}

pair<int, int> chrem(const vector<int> &ps, const vector<int> &rs) {
    using Long = __int128_t;
    int P = 1;
    for (int p: ps) {
        P *= p;
    }
    Long ret = 0;
    for (int i = 0; i < ps.size(); i++) {
        int p = P / ps[i];
        ret += Long(1) * rs[i] * modinv(p, ps[i]) * p;
    }

    return make_pair(ret % P, P);
}

int main() {
    vector<int> rs(3);
    for (int i = 0; i < 3; i++) {
        cin >> rs[i];
    }

    auto p = chrem({17, 107, (int) 1e9 + 7}, rs);
    cout << p.first << '\n';
}

