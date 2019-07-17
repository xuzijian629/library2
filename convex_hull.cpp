// added
#include <bits/stdc++.h>
using namespace std;

string to_string(__int128_t a) {
    if (a == 0) return "0";
    if (a < 0) return "-"s + to_string(-a);
    string ret = "";
    while (a) {
        ret += '0' + a % 10;
        a /= 10;
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

struct Rational {
    using Long = __int128_t;
    Long a, b;
    Rational() : a(0), b(1) {}
    Rational(Long a) : a(a), b(1) {}
    Rational(Long a, Long b) : a(a), b(b) { normalize(); }
    explicit operator double() const {
        return (double) a / b;
    }
    void normalize() {
        Long g = __gcd(a, b);
        a /= g, b /= g;
        if (b < 0) {
            a = -a, b = -b;
        }
    }
    Rational& operator+=(const Rational& rhs) {
        Long d = b * rhs.b;
        a = a * rhs.b + b * rhs.a;
        b = d;
        normalize();
        return *this;
    }
    Rational& operator-=(const Rational& rhs) {
        Long d = b * rhs.b;
        a = a * rhs.b - b * rhs.a;
        b = d;
        normalize();
        return *this;
    }
    Rational& operator*=(const Rational& rhs) {
        a *= rhs.a;
        b *= rhs.b;
        normalize();
        return *this;
    }
    Rational& operator/=(const Rational& rhs) {
        a *= rhs.b;
        b *= rhs.a;
        normalize();
        return *this;
    }
    Rational operator-() const {
        return Rational(-a, b);
    }
    Rational operator+(const Rational& rhs) const {
        return Rational(*this) += rhs;
    }
    Rational operator-(const Rational& rhs) const {
        return Rational(*this) -= rhs;
    }
    Rational operator*(const Rational& rhs) const {
        return Rational(*this) *= rhs;
    }
    Rational operator/(const Rational& rhs) const {
        return Rational(*this) /= rhs;
    }
    bool operator<(const Rational& rhs) const {
        return a * rhs.b < b * rhs.a;
    }
    bool operator>(const Rational& rhs) const {
        return rhs < *this;
    }
    bool operator<=(const Rational& rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const Rational& rhs) const {
        return !(*this < rhs);
    }
    bool operator==(const Rational& rhs) const {
        return !(*this < rhs) && !(rhs < *this);
    }
    bool operator!=(const Rational& rhs) const {
        return *this < rhs || rhs < *this;
    }
    friend ostream& operator<<(ostream& s, const Rational& f) {
        if (f.b == 1) {
            s << to_string(f.a);
        } else {
            s << to_string(f.a) << '/' << to_string(f.b);
        }
        return s;
    }
};

// https://lumakernel.github.io/ecasdqina/geometory/geometory
// http://www.prefield.com/algorithm/geometry/convex_hull.html
using val_t = Rational;
using Point = complex<val_t>;
using Polygon = vector<Point>;
const val_t EPS = 0;
struct Line : public pair<Point, Point> {
    Line(const Point &a, const Point &b) : pair<Point, Point>(a, b) {}
};
struct Segment : public pair<Point, Point> {
    Segment(const Point &a, const Point &b) : pair<Point, Point>(a, b) {}
};
val_t dot(const Point &a, const Point &b) {
    return real(conj(a) * b);
}
val_t cross(const Point &a, const Point &b) {
    return imag(conj(a) * b);
}
val_t X(const Point &a) {
    return real(a);
}
val_t Y(const Point &a) {
    return imag(a);
}
val_t norm2(const Point &a) {
    return X(a) * X(a) + Y(a) * Y(a);
}
int sign(val_t x) {
    if (x < -EPS) return -1;
    if (x > EPS) return +1;
    return 0;
}
int ccw(const Point &a, Point b, Point c) {
    b -= a, c -= a;
    if (cross(b, c) > EPS) return +1; // a->b->c is ccw
    if (cross(b, c) < -EPS) return -1; // a->b->c is cw
    if (dot(b, c) < 0) return 0; // b--a--c
    if (norm2(b) < norm2(c)) return +2; // a--b--c
    return -2; // b--c--a
}
bool is_crossing(const Segment &a, const Segment &b) {
    return ccw(a.first, a.second, b.first) * ccw(a.first, a.second, b.second) <= 0
        && ccw(b.first, b.second, a.first) * ccw(b.first, b.second, a.second) <= 0;
}
Point intersection(const Line &a, const Line &b) {
    return a.first + (a.second - a.first) * cross(b.second - b.first, a.first - b.first) / cross(b.second - b.first, a.first - a.second);
}
val_t dist2(const Point &a, const Point &b) {
    return norm2(a - b);
}
val_t dist2(const Line &line, const Point &p) {
    val_t t = cross(p - line.first, line.second - line.first);
    return t * t / norm2(line.second - line.first);
}
val_t dist2(const Point &p, const Line &line) {
    return dist2(line, p);
}
val_t dist2(const Segment &segment, const Point &p) {
    if (sign(dot(segment.first - segment.second, p - segment.second)) * sign(dot(segment.second - segment.first, p - segment.first)) >= 0) {
        return dist2(Line(segment.first, segment.second), p);
    }
    return min(norm2(p - segment.first), norm2(p - segment.second));
}
val_t dist2(const Point &p, const Segment &segment) {
    return dist2(segment, p);
}
val_t dist2(const Segment &a, const Segment &b) {
		if (is_crossing(a, b)) return val_t(0);
    return min({dist2(a, b.first), dist2(a, b.second), dist2(b, a.first), dist2(b, a.second)});
}

bool operator<(const Point &a, const Point &b) {
    return X(a) != X(b) ? X(a) < X(b) : Y(a) < Y(b);
}

// assume all points are not on a same line
Polygon convex_hull(vector<Point> ps, bool ignore_on_edge = false) {
    int n = ps.size(), k = 0;
    sort(ps.begin(), ps.end());
    Polygon ret(2 * n);
    for (int i = 0; i < n; ret[k++] = ps[i++]) {
        if (!ignore_on_edge) {
            while (k >= 2 && ccw(ret[k - 2], ret[k - 1], ps[i]) <= 0) k--;
        } else {
            while (k >= 2 && ccw(ret[k - 2], ret[k - 1], ps[i]) != 1) k--;
        }
    }
    for (int i = n - 2, t = k + 1; i >= 0; ret[k++] = ps[i--]) {
        if (!ignore_on_edge) {
            while (k >= t && ccw(ret[k - 2], ret[k - 1], ps[i]) <= 0) k--;
        } else {
            while (k >= t && ccw(ret[k - 2], ret[k - 1], ps[i]) != 1) k--;
        }
    }
    ret.resize(k - 1);
    return ret;
}

double ang(Point a, Point b) {
    b -= a;
    if (Y(b) == 0) {
        if (X(b) < 0) return acos(-1) / 2;
        return -acos(-1) / 2;
    }
    return atan2((double) -X(b), (double) Y(b));
}

int main() {
    cout.setf(ios::fixed);
    cout.precision(10);
    int n;
    cin >> n;
    vector<Point> ps;
    map<Point, int> id;
    
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        ps.emplace_back(x, y);
        id[ps.back()] = i;
    }
    sort(ps.begin(), ps.end());
    bool all_on_line = true;
    for (int i = 0; i < n - 2; i++) {
        int c = ccw(ps[0], ps[1], ps[i + 2]);
        if (c == 1 || c == -1) all_on_line = false;
    }
    if (all_on_line) {
        vector<double> ans(n);
        ans[id[ps[0]]] = ans[id[ps[n - 1]]] = 0.5;
        for (int i = 0; i < n; i++) {
            cout << ans[i] << '\n';
        }
    } else {
        Polygon ch = convex_hull(ps, true);
        vector<double> angs;
        for (int i = 0; i < (int) ch.size() - 1; i++) {
            angs.push_back(ang(ch[i], ch[i + 1]));
        }
        angs.push_back(ang(ch.back(), ch.front()));
        vector<double> diffs;
        diffs.push_back(angs.front() - angs.back());
        for (int i = 1; i < ch.size(); i++) {
            diffs.push_back(angs[i] - angs[i - 1]);
        }
        double sum = 0;
        for (auto &d : diffs) {
            if (d < 0) d += 2 * acos(-1);
        }
        vector<double> ans(n);
        for (int i = 0; i < ch.size(); i++) {
            ans[id[ch[i]]] = diffs[i] / 2 / acos(-1);
        }
        for (int i = 0; i < n; i++) {
            cout << ans[i] << '\n';
        }
    }
}
