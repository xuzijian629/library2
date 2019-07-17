// ushiadded
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;

template<class T>
void extend(T& a) {
    int n = int(a.size());
    int exp = 1;
    while (exp < 2 * n + 1) {
        exp <<= 1;
    }
    while (a.size() != exp) {
        a.push_back(0);
    }
}

void fft(vector<complex<double>>& a, bool inv = false) {
    int n = int(a.size());
    if (n == 1) return;
    vector<complex<double>> even(n / 2), odd(n / 2);
    for (int i = 0; i < n / 2; i++) {
        even[i] = a[2 * i];
        odd[i] = a[2 * i + 1];
    }
    fft(even, inv);
    fft(odd, inv);
    complex<double> omega = polar(1.0, (-2 * inv + 1) * 2 * acos(-1) / n);
    complex<double> pow_omega = 1.0;
    for (int i = 0; i < n / 2; i++) {
        a[i] = even[i] + pow_omega * odd[i];
        a[i + n / 2] = even[i] - pow_omega * odd[i];
        pow_omega *= omega;
    }
}

void conv(vector<complex<double>>& a, vector<complex<double>>& b) {
    fft(a);
    fft(b);
    int n = int(a.size());
    for (int i = 0; i < n; i++) {
        a[i] *= b[i] / complex<double>(n);
    }
    fft(a, true);
}

void conv(vi& a, vi& b) {
    vector<complex<double>> ac, bc;
    for (int i = 0; i < a.size(); i++) {
        ac.push_back(a[i]);
        bc.push_back(b[i]);
    }
    conv(ac, bc);
    a.resize(ac.size());
    for (int i = 0; i < ac.size(); i++) {
        a[i] = long(real(ac[i]) + 0.5);
    }
}

int main() {
    // assert(freopen("/Users/xuzijian/tmp/barcelona_bootcamp/day6_b/b_input", "r", stdin));
    // assert(freopen("/Users/xuzijian/tmp/barcelona_bootcamp/day6_b/b_output", "w", stdout));
    vi a[2];
    int sign = 1;
    for (int i = 0; i < 2; i++) {
        string s;
        cin >> s;

        reverse(s.begin(), s.end());
        if (s.back() == '-') {
            sign = -sign;
            s.pop_back();
        }

        assert('0' <= s.back() && s.back() <= '9');
        assert(s.size() < (1 << 19));

        while (s.size() != (1 << 19)) {
            s += '0';
        }
        reverse(s.begin(), s.end());

        for (char c: s) {
            a[i].push_back(c - '0');
        }
    }

    conv(a[0], a[1]);
    
    reverse(a[0].begin(), a[0].end());

    bool zero = true;
    for (int i = 1; i < a[0].size(); i++) {
        if (a[0][i]) zero = false;
        int k = a[0][i];
        if (i < a[0].size() - 1) {
            a[0][i] = k % 10;
        }
        int j = 0;
        while (1) {
            k /= 10;
            j++;
            if (k) {
                if (i + j < a[0].size() - 1) {
                    a[0][i + j] += k % 10;
                } else {
                    a[0][i + j] += k;
                }
            } else {
                break;
            }
        }
    }
    
    reverse(a[0].begin(), a[0].end());

    if (zero) {
        cout << 0 << endl;
        return 0;
    }

    cout << (sign == 1 ? "" : "-");
    int leadingzero = true;
    for (int i = 0; i < a[0].size() - 1; i++) {
        if (leadingzero) {
            if (a[0][i]) {
                cout << a[0][i];
                leadingzero = false;
            }
        } else {
            cout << a[0][i];
        }
    }
    cout << endl;
}
