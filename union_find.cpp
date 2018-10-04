#include <bits/stdc++.h>
using namespace std;

int par[10101];
int find(int x) {
    if (x == par[x]) return x;
    return par[x] = find(par[x]);
}

void unite(int x, int y) {
    x = find(x);
    y = find(y);
    par[x] = y;
}

int main() {
    int n, q;
    cin >> n >> q;

    for (int i = 0; i < 10101; i++) {
        par[i] = i;
    }

    while (q--) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t) { 
            cout << (find(x) == find(y)) << endl;
        } else {
            unite(x, y);
        }
    }
}