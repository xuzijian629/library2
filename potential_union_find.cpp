//added

#include <bits/stdc++.h>

using namespace std;

int par[101010];
int diff[101010];

int find(int x) {
    if (par[x] == x) return x;
    int r = find(par[x]);
    diff[x] += diff[par[x]];
    return par[x] = r;
}

int weight(int x) {
    find(x);
    return diff[x];
}

void unite(int x, int y, int w) {
    w += weight(x);
    w -= weight(y);
    x = find(x);
    y = find(y);
    par[y] = x;
    diff[y] = w;
}

int main() {
    int n, q;
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        par[i] = i;
    }
    while (q--) {
        int t;
        cin >> t;
        if (t == 0) {
            int x, y, w;
            cin >> x >> y >> w;
            unite(x, y, w);
        } else {
            int x, y;
            cin >> x >> y;
            if (find(x) == find(y)) {
                cout << weight(y) - weight(x) << endl;
            } else {
                cout << "?" << endl;
            }
        }
    }
}