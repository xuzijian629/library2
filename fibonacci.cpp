#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
constexpr i64 MOD = 1e9 + 7;

template<int n>
struct mat {
    vvi d;

    mat() {
        d = vvi(n, vi(n));
    }
    
    mat(initializer_list<initializer_list<i64>> m) {
        for (auto& a: m) {
            vi row(a.begin(), a.end());
            d.emplace_back(row);
        }
        assert(n == d.size());
        assert(n == d.front().size());
    };
    
    mat operator+(const mat& rhs) {
        mat ret;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ret.d[i][j] = d[i][j] + rhs.d[i][j];
                ret.d[i][j] %= MOD;
                ret.d[i][j] += MOD;
                ret.d[i][j] %= MOD;
            }
        }
        return ret;
    }
    
    mat operator-(const mat& rhs) {
        mat ret;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ret.d[i][j] = d[i][j] - rhs.d[i][j];
                ret.d[i][j] %= MOD;
                ret.d[i][j] += MOD;
                ret.d[i][j] %= MOD;
            }
        }
        return ret;
    }
    
    mat operator*(const mat& rhs) {
        mat ret;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    ret.d[i][j] += d[i][k] * rhs.d[k][j];
                    ret.d[i][j] %= MOD;
                    ret.d[i][j] += MOD;
                    ret.d[i][j] %= MOD;
                }
            }
        }
        return ret;
    }
    
    mat operator*(const i64 k) {
        mat ret;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ret.d[i][j] = d[i][j] * k;
                ret.d[i][j] %= MOD;
                ret.d[i][j] += MOD;
                ret.d[i][j] %= MOD;
            }
        }
        return ret;
    }
    
    static mat eye() {
        mat ret;
        for (int i = 0; i < n; i++) {
            ret.d[i][i] = 1;
        }
        return ret;
    }
};

template<int k>
mat<k> pow(mat<k>& a, i64 n) {
    if (n == 0) {
        return mat<k>::eye();
    }
    if (n % 2 == 0) {
        mat<k> t = pow(a, n / 2);
        return t * t;
    }
    return a * pow(a, n - 1);
}

i64 fib(i64 n) {
    if (n <= 1) return n;
    mat<2> f{{1, 1}, {1, 0}};
    mat<2> res = pow(f, n - 2);
    return (res.d[0][0] + res.d[0][1]) % MOD;
}

int main() {
    int n;
    cin >> n;
    cout << fib(n + 1) << endl;
}
