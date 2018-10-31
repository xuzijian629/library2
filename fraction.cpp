#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

struct frac {
    i64 p, q;
    void norm() {
        i64 g = __gcd(p, q);
        p /= g;
        q /= g;
        if (q < 0) {
            q = -q;
            p = -p;
        }
    }

    frac operator+(const frac& rhs) const {
        frac ret = {p * rhs.q + q * rhs.p, q * rhs.q};
        ret.norm();
        return ret;
    }

    frac operator-(const frac& rhs) const {
        frac ret = {p * rhs.q - q * rhs.p, q * rhs.q};
        ret.norm();
        return ret;
    }

    frac operator*(const frac& rhs) const {
        frac ret = {p * rhs.p, q * rhs.q};
        ret.norm();
        return ret;
    }

    frac operator*(const i64 rhs) const {
        frac ret = {p * rhs, q};
        ret.norm();
        return ret;
    }

    frac operator~() const {
        return {q, p};
    }

    bool operator<(const frac& rhs) const {
        frac d = (frac){p, q} - rhs;
        return d.p < 0;
    }

    bool operator>(const frac& rhs) const {
        frac d = (frac){p, q} - rhs;
        return d.p > 0;
    }

    bool operator==(const frac& rhs) const {
        frac d = (frac){p, q} - rhs;
        return d.p == 0;
    }

    frac operator%(const frac& rhs) {
        if ((frac){p, q} < rhs) {
            return {p, q};
        } else {
            frac t = (frac){p, q} * (~rhs);
            i64 k = t.p / t.q;
            return (frac){p, q} - rhs * k;
        }
    }
};

ostream& operator<<(ostream& os, frac f) {
    os << f.p << "/" << f.q;
    return os;
}

frac gcd(frac a, frac b) {
    return b.p ? gcd(b, a % b) : a;
}

int main() {
    vi t(3);
    for (int i = 0; i < 3; i++) {
        cin >> t[i];
    }
    sort(t.begin(), t.end());
    frac a = {t[1] * t[0], t[1] - t[0]};
    frac b = {t[2] * t[1], t[2] - t[1]};
    a.norm();
    b.norm();
    frac g = gcd(a, b);
    cout << a * (~g) * b << endl;
}