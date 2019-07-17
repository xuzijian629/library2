// iran

#include <bits/stdc++.h>
using namespace std;
using Long = __int128_t;

template<typename T>
T pow(T a, long long n, T e = 1) {
    T ret = e;
    while (n) {
        if (n & 1) ret *= a;
        a *= a;
        n >>= 1;
    }
    return ret;
}

struct ModuloMatrix {
    int mod;
    int N;
    vector<vector<int>> A;

    ModuloMatrix(const vector<vector<int>> &A, int mod) : N(A.size()), A(A), mod(mod) {}

    ModuloMatrix &operator+=(const ModuloMatrix &rhs) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] += rhs.A[i][j];
                if (A[i][j] >= mod) A[i][j] -= mod;
            }
        }
        return *this;
    };
    ModuloMatrix &operator-=(const ModuloMatrix &rhs) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] -= rhs.A[i][j];
                if (A[i][j] < 0) A[i][j] += mod;
            }
        }
        return *this;
    }
    ModuloMatrix operator+(const ModuloMatrix &rhs) const {
        return ModuloMatrix(*this) += rhs;
    }
    ModuloMatrix operator-(const ModuloMatrix &rhs) const {
        return ModuloMatrix(*this) -= rhs;
    }
    ModuloMatrix operator*(const ModuloMatrix &rhs) const {
        vector<vector<int>> B(N, vector<int>(N));
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                for (int j = 0; j < N; j++) {
                    B[i][j] += 1ll * A[i][k] * rhs.A[k][j] % mod;
                    if (B[i][j] >= mod) B[i][j] -= mod;
                }
            }
        }
        return ModuloMatrix(B, mod);
    }
    ModuloMatrix &operator*=(const ModuloMatrix &rhs) {
        return *this = *this * rhs;
    }
};

ModuloMatrix eye(int N, int mod) {
    vector<vector<int>> A(N, vector<int>(N));
    for (int i = 0; i < N; i++) A[i][i] = 1;
    return ModuloMatrix(A, mod);
}

class ModuloLinearTransform {
    int N;
    ModuloMatrix A;
    vector<int> x0;

    // A0 + A1 + ... + A{n-1}
    ModuloMatrix acc(long long n) {
        int mod = A.mod;
        if (n == 1) return eye(N, mod);
        if (n % 2 == 0) {
            ModuloMatrix B = acc(n / 2);
            return (eye(N, mod) + pow(A, n / 2, eye(N, mod))) * B;
        }
        return pow(A, n - 1, eye(N, mod)) + acc(n - 1);
    }

public:
    ModuloLinearTransform(int mod, const vector<vector<int>> &B, vector<int> x) : N(B.size()), A(B, mod), x0(move(x)) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A.A[i][j] %= mod;
                if (A.A[i][j] < 0) A.A[i][j] += mod;
            }
            x0[i] %= mod;
            if (x0[i] < 0) x0[i] += mod;
        }
    }

    // xn = An * x0, 0-indexed
    vector<int> get(long long n) {
        assert(n >= 0);
        int mod = A.mod;
        ModuloMatrix An = pow(A, n, eye(N, mod));
        vector<int> ret(N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                ret[i] += 1ll * An.A[i][j] * x0[j] % mod;
                if (ret[i] >= mod) ret[i] -= mod;
            }
        }
        return ret;
    }

    // x0 + x1 + ... + x{n-1} = (A0 + A1 + ... + A{n-1}) * x0
    vector<int> sum(long long n) {
        assert(n >= 1);
        int mod = A.mod;
        ModuloMatrix S = acc(n);
        vector<int> ret(N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                ret[i] += 1ll * S.A[i][j] * x0[j] % mod;
                if (ret[i] >= mod) ret[i] -= mod;
            }
        }
        return ret;
    }
};

int modpow(int a, Long n, int mod) {
    if (n == 0) return 1;
    if (n & 1) {
        return 1ll * a * modpow(a, n - 1, mod) % mod;
    }
    int t = modpow(a, n / 2, mod);
    return 1ll * t * t % mod;
}

int calc(int k, int a, int d, long long n, int m) {
    ModuloLinearTransform lt(m, {{k % m, -d % m}, {0, k % m}}, {int(a + (n - 1) % m * d % m) % m, k % m});
    return lt.sum(n)[0];
}

void solve(long long l, long long a, long long d, int m) {
    int ans = 0;
    long long ba = 1;
    vector<int> tmp;
    vector<Long> keta;
    for (int i = 1; i <= 18; i++) {
        // ba未満の項をまとめる
        ba *= 10;
        long long k = (ba - a + d - 1) / d;
        if (k < 1) continue;
        long long A = a;
        if (l > k) {
            l -= k;
            tmp.push_back(calc(ba % m, A % m, d % m, k, m));
            keta.push_back((Long) i * k);
            if (l == 0) break;
        } else {
            tmp.push_back(calc(ba % m, A % m, d % m, l, m));
            keta.push_back((Long) i * l);
            break;
        }
        a += k * d;
    }
    for (int i = (int) keta.size() - 2; i >= 0; i--) {
        keta[i] += keta[i + 1];
    }

    for (int i = 0; i < tmp.size(); i++) {
        ans += 1ll * tmp[i] * modpow(10, i + 1 == tmp.size() ? 0 : keta[i + 1], m) % m;
        ans %= m;
    }

    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long l, a, d;
    cin >> l >> a >> d;

    int m;
    cin >> m;

    solve(l, a, d, m);
}