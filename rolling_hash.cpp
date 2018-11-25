#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

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

bool is_prime(i64 n, int k = 50) {
    if (n == 2) return true;
    if (n % 2 == 0 || n < 2) return false;
    i64 d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }
    for (int i = 0; i < k; i++) {
        i64 a = rnd() % (n - 2) + 1;
        i64 t = d;
        i64 y = modpow(a, t, n);
        while (t != n - 1 && y != 1 && y != n - 1) {
            y = modpow(y, 2, n);
            t *= 2;
        }
        if (y != n - 1 && t % 2 == 0) {
            return false;
        }
    }
    return true;
}

i64 gen_prime() {
    while (1) {
        i64 d = rnd() & ((i64(1) << 31) - 1);
        d |= 1;
        if (is_prime(d)) {
            return d;
        }
    }
}

struct RollingHash {
    vi hash;
    vi pows;
    i64 p, m;
    RollingHash(string s, i64 p = gen_prime(), i64 m = gen_prime()) : hash(s.size() + 1), pows(s.size() + 1), p(p), m(m) {
        hash[0] = 1;
        pows[0] = 1;
        for (int i = 0; i < s.size(); i++) {
            hash[i + 1] = (hash[i] * p % m + s[i]) % m;
            pows[i + 1] = pows[i] * p % m;
        }
    }

    // [l, r)
    i64 encode(int l, int r) {
        return ((hash[r] - hash[l] * pows[r - l] % m) % m + m) % m;
    }

    bool eq(int l0, int r0, int l1, int r1) {
        return encode(l0, r0) == encode(l1, r1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string s;
    cin >> s;
    string t;
    cin >> t;
    RollingHash rh(t);
    int zeros = 0, ones = 0;
    for (char c : s) {
        if (c == '0') zeros++;
        else ones++;
    }

    int ans = 0;
    for (int i = 1; i < t.size(); i++) {
        int zl = zeros * i;
        if (zl >= t.size()) break;
        int ol = t.size() - zl;
        if (ol % ones) continue;
        int j = ol / ones;

        i64 enczero = -1, encone = -1;
        int cur = 0, ok = 1;
        for (int k = 0; k < s.size(); k++) {
            if (!ok) break;
            if (s[k] == '0') {
                i64 ez = rh.encode(cur, cur + i);
                if (enczero == -1) {
                    enczero = ez;
                } else if (enczero != ez) {
                    ok = 0;
                }
                cur += i;
            } else {
                i64 eo = rh.encode(cur, cur + j);
                if (encone == -1) {
                    encone = eo;
                } else if (encone != eo) {
                    ok = 0;
                }
                cur += j;
            }
            if (enczero == encone) ok = 0;
        }
        if (ok) ans++;
    }

    cout << ans << endl;
}