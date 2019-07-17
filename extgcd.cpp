// added

#include <bits/stdc++.h>

using namespace std;

int extgcd(int a, int b, int &x, int &y) {
    int g = a;
    x = 1, y = 0;
    if (b) {
        g = extgcd(b, a % b, y, x);
        y -= a / b * x;
    }
    return g;
}

int main() {
    int a, b, x, y;
    cin >> a >> b;
    extgcd(a, b, x, y);
    cout << x << " " << y << endl;
}