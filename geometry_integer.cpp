// not verified!!
// hoge

#include <bits/stdc++.h>
using namespace std;
using i128 = __int128_t;

struct vec2 {
    i128 x, y;
    vec2 operator-(vec2 rhs) {
        return {x - rhs.x, y - rhs.y};
    }
    bool operator==(vec2 rhs) {
        return x == rhs.x && y == rhs.y;
    }
};

struct seg {
    vec2 a, b;
};

i128 cross(vec2 a, vec2 b) {
    return a.x * b.y - a.y * b.x;
}

bool is_crossed(seg s, seg t) {
    vec2 p1 = s.a, p2 = s.b, p3 = t.a, p4 = t.b;
    i128 t1 = (p1.x - p2.x) * (p3.y - p1.y) + (p1.y - p2.y) * (p1.x - p3.x);
    i128 t2 = (p1.x - p2.x) * (p4.y - p1.y) + (p1.y - p2.y) * (p1.x - p4.x);
    i128 t3 = (p3.x - p4.x) * (p1.y - p3.y) + (p3.y - p4.y) * (p3.x - p1.x);
    i128 t4 = (p3.x - p4.x) * (p2.y - p3.y) + (p3.y - p4.y) * (p3.x - p2.x);
    return t1 * t2 <= 0 && t3 * t4 <= 0;
    // return t1 * t2 < 0 && t3 * t4 < 0;
}

bool is_parallel(seg s, seg t) {
    return !cross(s.b - s.a, t.b - t.a);
}

i128 signedArea2(vector<vec2> ps) {
    int n = ps.size();
    i128 sum = 0;
    for (int i = 0; i < n; i++) {
        sum += cross(ps[i], ps[(i + 1) % n]);
    }
    return sum;
}
