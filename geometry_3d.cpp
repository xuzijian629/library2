// iran

#include <bits/stdc++.h>
using namespace std;

constexpr double EPS = 1e-10;

struct Point {
    double x, y, z;
    Point operator*(const int k) {
        return {x * k, y * k, z * k};
    }
    Point operator+(const Point& rhs) {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }
    Point operator-(const Point& rhs) {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }
    bool operator==(const Point& rhs) {
        return abs(x - rhs.x) < EPS && abs(y - rhs.y) < EPS && (z - rhs.z) < EPS;
    }
};

struct Vector : Point {
    double norm() {
        return sqrt(x * x + y * y + z * z);
    }
};

double dot(const Vector& lhs, const Vector& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector cross(const Vector& lhs, const Vector& rhs) {
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

double triple(const Vector& a, const Vector& b, const Vector& c) {
    return dot(a, cross(b, c));
}

struct Line {
    Point p;
    Vector dir;
    bool operator==(const Line rhs) {

    }
};

struct Plane {
    Point p;
    Vector nor;
};

// 二点間のベクトル
// ベクトルの交点
// ベクトルと平面の交点
// 凸判定
// 体積

int main() {
    
}