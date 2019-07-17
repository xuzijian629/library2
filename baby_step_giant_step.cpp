// added

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;

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

i64 modlog(i64 b, i64 y, i64 mod) {
    // find minimam x such that modpow(b, x, mod) == y
    b %= mod;
    y %= mod;
    assert(b);
    i64 l = -1, r = mod;
    while (l < r - 1) {
        i64 m = (l + r) / 2;
        if (m * m >= mod) r = m;
        else l = m;
    }
    i64 sqrtM = r;
    unordered_map<i64, i64> bpow;
    i64 p = 1;
    for (int i = 0; i < sqrtM; i++) {
        if (!bpow.count(p)) {
            bpow[p] = i;
        }
        p *= b;
        p %= mod;
    }

    i64 B = modpow(modinv(b, mod), sqrtM, mod);
    p = y;
    for (int i = 0; i < sqrtM; i++) {
        if (bpow.count(p)) {
            return i * sqrtM + bpow[p];
        }
        p *= B;
        p %= mod;
    }
    
    return -1;
}

int main() {
    i64 p, q;
    cin >> p >> q;
    for (int i = 0; i < q; i++) {
        i64 x, y;
        cin >> x >> y;
        if (x == 0) {
            if (y == 0) {
                cout << 1 << endl;
            } else if (y == 1) {
                cout << 0 << endl;
            } else {
                cout << -1 << endl;
            }
            continue;
        }
        cout << modlog(x, y, p) << endl;
    }
}