#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;

// 負の数は扱っていない
// 宣言するときはconvert関数を使う
// 掛け算でFFTをやるので畳み込み後の配列の最大要素を1e13程度にして誤差を小さくしたい
// BASELOGを大きくすると桁数が1/BASELOGになる代わりに配列の要素が指数関数的に大きくなる
// 掛け算をしないのであれば定数倍早くなるのでBASELOGを大きくするとよい。掛け算をするときは3が精度的に安心
constexpr i64 BASE = 1000;
constexpr int BASELOG = 3;
struct BigInt {
    // 掛け算のために本来の最大桁数の2倍必要
    vi digit = vi(1 << 18);
    int size;
    BigInt(int size = 1, i64 a = 0) : size(size) {
        digit[0] = a;
    }
    BigInt(const BigInt& a) {
        size = a.size;
        digit = vi(a.digit);
    }
};

bool operator<(BigInt x, BigInt y) {
    if (x.size != y.size) {
        return x.size < y.size;
    }
    for (int i = x.size - 1; i >= 0; i--) {
        if (x.digit[i] != y.digit[i]) {
            return x.digit[i] < y.digit[i];
        }
    }
    return false;
}

bool operator>(BigInt x, BigInt y) {
    return y < x;
}
bool operator<=(BigInt x, BigInt y) {
    return !(y < x);
}
bool operator>=(BigInt x, BigInt y) {
    return !(x < y);
}
bool operator!=(BigInt x, BigInt y) {
    return x < y || y < x;
}
bool operator==(BigInt x, BigInt y) {
    return !(x < y) && !(y < x);
}

BigInt normal(BigInt x, bool all = false) {
    i64 c = 0;
    if (all) {
        x.size = int(x.digit.size()) - 1;
    }
    for (int i = 0; i < x.size; i++) {
        while (x.digit[i] < 0) {
            x.digit[i + 1] -= 1;
            x.digit[i] += BASE;
        }
        while (x.digit[i] >= BASE) {
            x.digit[i + 1] += 1;
            x.digit[i] -= BASE;
        }
        i64 a = x.digit[i] + c;
        x.digit[i] = a % BASE;
        c = a / BASE;
    }
    for (; c > 0; c /= BASE) {
        x.digit[x.size++] = c % BASE;
    }
    while (x.size > 1 && x.digit[x.size - 1] == 0) {
        x.size--;
    }
    return x;
}

BigInt convert(i64 a) {
    return normal(BigInt(1, a), true);
}

BigInt convert(const string& s) {
    BigInt x;
    assert(s.size() / BASELOG <= x.digit.size() / 2);
    int i = s.size() % BASELOG;
    if (i > 0) {
        i -= BASELOG;
    }
    for (; i < int(s.size()); i += BASELOG) {
        i64 a = 0;
        for (int j = 0; j < BASELOG; j++) {
            a = 10 * a + (i + j >= 0 ? s[i + j] - '0' : 0);
        }
        x.digit[x.size++] = a;
    }
    reverse(x.digit.begin(), x.digit.begin() + x.size);
    return normal(x);
}

ostream &operator<<(ostream& os, BigInt x) {
    os << x.digit[x.size - 1];
    for (int i = x.size - 2; i >= 0; i--) {
        os << setw(BASELOG) << setfill('0') << x.digit[i];
    }
    return os;
}

istream &operator>>(istream& is, BigInt &x) {
    string s;
    is >> s;
    x = convert(s);
    return is;
}

string to_string(BigInt &x) {
    stringstream ss;
    ss << x.digit[x.size - 1];
    for (int i = x.size - 2; i >= 0; i--) {
        ss << setw(BASELOG) << setfill('0') << x.digit[i];
    }
    return ss.str();
}

BigInt operator+(BigInt x, BigInt y) {
    if (x.size < y.size) {
        x.size = y.size;
    }
    for (int i = 0; i < y.size; i++) {
        x.digit[i] += y.digit[i];
    }
    return normal(x);
}

BigInt operator-(BigInt x, BigInt y) {
    assert(x >= y);
    for (int i = 0; i < y.size; i++) {
        x.digit[i] -= y.digit[i];
    }
    return normal(x);
}

BigInt operator*(BigInt x, i64 a) {
    for (int i = 0; i < x.size; i++) {
        x.digit[i] *= a;
    }
    return normal(x);
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

BigInt operator*(BigInt x, BigInt y) {
    conv(x.digit, y.digit);
    return normal(x, true);
}

pair<BigInt, i64> divmod(BigInt x, i64 a) {
    i64 c = 0, t;
    for (int i = x.size - 1; i >= 0; i--) {
        t = BASE * c + x.digit[i];
        x.digit[i] = t / a;
        c = t % a;
    }
    return pair<BigInt, i64>(normal(x), c);
}

BigInt operator/(BigInt x, i64 a) {
    return divmod(x, a).first;
}

i64 operator%(BigInt x, i64 a) {
    return divmod(x, a).second;
}

int main() {
    string a;
    cin >> a;
    if (a == "1" || a == "2") {
        cout << "I'm sorry, Hexy" << endl;
        return 0;
    }
    BigInt A = convert(a);
    if ((a.back() - '0') % 2) {
        BigInt m = (A + convert(1)) / 2, n = (A - convert(1)) / 2;
        cout << m * n * 2 << " " << m * m + n * n << endl;
    } else {
        BigInt m = A / 2;
        BigInt M = m * m;
        cout << M - convert(1) << " " << M + convert(1) << endl;
    }
}
