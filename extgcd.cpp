#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;

i64 extgcd(i64 a, i64 b, i64& x, i64& y) {
    i64 g = a;
    x = 1, y = 0;
    if (b) {
        g = extgcd(b, a % b, y, x);
        y -= a / b * x;
    }
    return g;
}

int main() {
    i64 a, b, x, y;
    cin >> a >> b;
    extgcd(a, b, x, y);
    cout << x << " " << y << endl;
}