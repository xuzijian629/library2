#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
constexpr i64 MOD = 1e9 + 7;

i64 modpow(i64 a, i64 n, i64 mod) {
    if (n == 0) return 1;
    if (n % 2 == 0) {
        i64 t = modpow(a, n / 2, mod);
        return t * t % mod;
    }
    return a * modpow(a, n - 1, mod) % mod;
}

i64 modinv(i64 a, i64 mod) {
    // mod is prime
    return modpow(a, mod - 2, mod);
}

struct Combination {
    const int MAX_N = 1 << 21;
    vi fact = vi(MAX_N);
    vi factinv = vi(MAX_N);

    Combination() {
        fact[0] = 1;
        for (int i = 1; i < MAX_N; i++) {
            fact[i] = fact[i - 1] * i % MOD;
        }
        factinv[MAX_N - 1] = modinv(fact[MAX_N - 1], MOD);
        for (int i = MAX_N - 2; i >= 0; i--) {
            factinv[i] = factinv[i + 1] * (i + 1) % MOD;
        }
    }

    i64 ncr(i64 n, i64 r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * factinv[r] % MOD * factinv[n - r] % MOD;
    }

    i64 npr(i64 n, i64 r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * factinv[n - r] % MOD;
    }

    i64 nhr(i64 n, i64 r) {
        if (n == 0 && r == 0) return 1;
        return ncr(n + r - 1, r);
    }
} comb;

i64 f(int n, int k) {
    i64 ret = 0;
    for (int i = 1; i <= k; i++) {
        if ((k - i) % 2) {
            ret -= comb.ncr(k, i) * modpow(i, n, MOD) % MOD;
        } else {
            ret += comb.ncr(k, i) * modpow(i, n, MOD) % MOD;
        }
        ret %= MOD;
    }
    ret += MOD;
    return ret % MOD;
}

i64 S(int n, int k) {
    return f(n, k) * comb.factinv[k] % MOD;
}

i64 B(int n, int k) {
    i64 ret = 0;
    for (int i = 1; i <= k; i++) {
        ret += S(n, i);
        ret %= MOD;
    }
    return ret;
}

int main() {
    int n, k;
    cin >> n >> k;
    cout << S(n, k) << endl;
}