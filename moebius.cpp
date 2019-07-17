// added

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

// before: ss[i]: 集合族iの共通部分の大きさ
// after : ss[i]: 集合族iに含まれていて、集合族~iに含まれない部分の大きさ
void moebius(vector<int> &ss) {
    int N = ss.size();
    int n = 0;
    while (N > 1) {
        n++;
        N >>= 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (1 << n); j++) {
            if (!(j & (1 << i))) {
                ss[j] -= ss[j | (1 << i)];
            }
        }
    }
}

int intsqrt(int n) {
    int l = 0, r = n + 1;
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (__int128_t(m) * m <= n) {
            l = m;
        } else {
            r = m;
        }
    }
    return l;
}

int cntsqrt(int l, int r) {
    return intsqrt(r) - intsqrt(l - 1);
}

int cntpx(int l, int r, int p) {
    return r / p - (l - 1) / p;
}

int get_min_p(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return i;
        }
    }
    return n;
}

int cntpxsqrt(int l, int r, int p) {
    int q = 1;
    while (p != 1) {
        int k = get_min_p(p);
        int cnt = 0;
        while (p % k == 0) {
            p /= k;
            cnt++;
        }
        for (int i = 0; i < (cnt + 1) / 2; i++) {
            q *= k;
        }
    }

    return intsqrt(r) / q - intsqrt(l - 1) / q;
}

int gcd(int a, int b) {
    return b ? gcd(b, a % b) : a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int cntrange(int l, int r, int zone) {
    if (r < l) return 0;
    vector<int> ss(1 << zone);
    vector<int> ps;
    for (int i = 2; i <= zone; i++) {
        ps.push_back(i);
    }

    for (int i = 0; i < (1 << zone); i++) {
        int p = 1;
        for (int j = 0; j < zone - 1; j++) {
            if ((i >> j) & 1) {
                p = lcm(p, ps[j]);
            }
        }
        if ((i >> (zone - 1)) & 1) {
            // sqrt
            ss[i] = cntpxsqrt(l, r, p);
            if (p == 1) {
                assert(cntpxsqrt(l, r, p) == cntsqrt(l, r));
            }
        } else {
            ss[i] = cntpx(l, r, p);
        }
    }
    assert(ss[0] == r - l + 1);

    moebius(ss);

    int cnt = r - l + 1, tmp = 0;;
    for (int i = 0; i < (1 << zone); i++) {
        assert(ss[i] >= 0);
        if (__builtin_popcount(i) & 1) {
            cnt -= ss[i];
        } else {
            tmp += ss[i];
        }
    }
    assert(cnt >= 0);
    assert(cnt == tmp);
    return cnt;
}

int main() {
    int n;
    cin >> n;
    int B = 1;
    for (int i = 0; i < 18; i++) {
        B *= 10;
    }

    if (n <= B) {
        cout << intsqrt(n) << endl;
        return 0;
    }

    int cnt = intsqrt(B);
    cnt += cntrange(B + 1, min(n, 2 * B), 1);
    cnt += cntrange(2 * B + 1, min(n, 3 * B), 2);
    cnt += cntrange(3 * B + 1, min(n, 4 * B), 3);
    cnt += cntrange(4 * B + 1, min(n, 5 * B), 4);
    cnt += cntrange(5 * B + 1, min(n, 6 * B), 5);
    cnt += cntrange(6 * B + 1, min(n, 7 * B), 6);
    cnt += cntrange(7 * B + 1, min(n, 8 * B), 7);
    cnt += cntrange(8 * B + 1, min(n, 9 * B), 8);
    cnt += cntrange(9 * B + 1, n, 9);
    cout << cnt << endl;
}
