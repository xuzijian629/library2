#include <bits/stdc++.h>

using namespace std;


vector<long long> rbd(vector<long long> mat) {
    int n = mat.size();
    int rk = 0;
    for (int i = 62; i >= 0; i--) {
        bool exist = false;
        for (int j = rk; j < n; j++) {
            if (mat[j] >> i & 1) {
                exist = true;
                swap(mat[rk], mat[j]);
                break;
            }
        }
        if (exist) {
            for (int j = 0; j < n; j++) {
                if (j != rk && mat[j] >> i & 1) {
                    mat[j] ^= mat[rk];
                }
            }
            rk++;
        }
    }
    return mat;
}

int main() {
    int n;
    cin >> n;
    vector<long long> as(n), bs(n);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> bs[i];
    }

    cout << (rbd(as) == rbd(bs) ? "Yes" : "No") << endl;
}
