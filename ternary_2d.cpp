#include <bits/stdc++.h>
using namespace std;

template<class T>
struct ext {
    double val;
    T x, y;
};

template<class T>
ext<T> ternary_2d(function<double(T, T)> f, T xl, T xr, T yl, T yr, bool maximum = true) {
    if (!maximum) {
        function<double(T, T)> g = [&](T a, T b){return -f(a, b);};
        ext<T> ret = ternary_2d(g, xl, xr, yl, yr, true);
        return {-ret.val, ret.x, ret.y};
    }
    // fix y
    function<ext<T>(T, T, T)> fx = [&](T y, T l, T r) {
        for (int i = 0; i < 100; i++) {
            T ml = (l * 2 + r) / 3.0;
            T mr = (l + r * 2) / 3.0;
            if (f(ml, y) < f(mr, y)) {
                l = ml;
            } else {
                r = mr;
            }
        }
        ext<T> ret;
        ret.x = l;
        ret.y = y;
        ret.val = f(l, y);
        return ret;
    };

    for (int i = 0; i < 100; i++) {
        T ml = (yl * 2 + yr) / 3.0;
        T mr = (yl + yr * 2) / 3.0;
        if (f(fx(ml, xl, xr).x, ml) < f(fx(mr, xl, xr).x, mr)) {
            yl = ml;
        } else {
            yr = mr;
        }
    }
    ext<T> ret;
    ret.x = fx(yl, xl, xr).x;
    ret.y = yl;
    ret.val = f(ret.x, ret.y);
    return ret;
}

struct vec3 {
    double  x, y, z;
    vec3 operator+(const vec3 rhs) {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }
    vec3 operator-() {
        return {-x, -y, -z};
    }
    vec3 operator-(const vec3 rhs) {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }
    vec3 operator*(const double k) {
        return {x * k, y * k, z * k};
    }
    vec3 operator/(const double k) {
        return {x / k, y / k, z / k};
    }
};

double dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
};

double norm2(vec3 a) {
    return dot(a, a);
}

int main() {
    vec3 vs[4];
    double x, y, z;
    for (int i = 0; i < 4; i++) {
        cin >> x >> y >> z;
        vs[i] = {x, y, z};
    }

    function<double(vec3, vec3)> dist = [&](vec3 a, vec3 b) {
        return norm2(b - a);
    };
    ext<vec3> ans;
    ans = ternary_2d(dist, vs[0], vs[1], vs[2], vs[3], false);

    printf("%.10f\n", sqrt(ans.val));
}