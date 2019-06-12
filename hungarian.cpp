// MOVED

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

// 二部グラフの最大重みマッチング
class Hungarian {
    int n, p, q;
    vvi mat;
    vi fx, fy, x, y;
    const i64 INF = 1e9;

public:
    Hungarian(const vvi& mat) : mat(mat) {
        n = mat.size();
        fx.assign(n, INF);
        fy.assign(n, 0);
        x.assign(n, -1);
        y.assign(n, -1);
    } 

    i64 run() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fx[i] = max(fx[i], mat[i][j]);
            }
        }

        for (int i = 0; i < n;) {
            vi t(n, -1), s(n + 1, i);
            for (p = q = 0; p <= q && x[i] < 0; p++) {
                for (int k = s[p], j = 0; j < n && x[i] < 0; j++) {
                    if (fx[k] + fy[j] == mat[k][j] && t[j] < 0) {
                        s[++q] = y[j];
                        t[j] = k;
                        if (s[q] < 0) {
                            for (p = j; p >= 0; j = p) {
                                y[j] = k = t[j];
                                p = x[k];
                                x[k] = j;
                            }
                        }
                    }
                }
            }
            if (x[i] < 0) {
                i64 d = INF;
                for (int k = 0; k <= q; k++) {
                    for (int j = 0; j < n; j++) {
                        if (t[j] < 0) {
                            d = min(d, fx[s[k]] + fy[j] - mat[s[k]][j]);
                        }
                    }
                }
                for (int j = 0; j < n; j++) {
                    fy[j] += (t[j] < 0 ? 0 : d);
                }
                for (int k = 0; k <= q; k++) {
                    fx[s[k]] -= d;
                }
            } else {
                i++;
            }
        }
        i64 ret = 0;
        for (int i = 0; i < n; i++) {
            ret += mat[i][x[i]];
        }
        return ret;
    }

    int match_y(int k) {
        return x[k];
    }

    int match_x(int k) {
        return y[k];
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vvi b(n, vi(m, -1));
    int wcnt = 0, bcnt = 0;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < m; j++) {
            if (s[j] == 'w') {
                b[i][j] = wcnt++;
            } else if (s[j] == 'b') {
                b[i][j] = 10000 + bcnt++;
            }
        }
    }

    int k = max(wcnt, bcnt);
    vvi mat(k, vi(k));

    auto is_next = [](int i, int j, int ii, int jj) {
        return abs(i - ii) + abs(j - jj) == 1;
    };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int ii = 0; ii < n; ii++) {
                for (int jj = 0; jj < m; jj++) {
                    if (0 <= b[i][j] && b[i][j] < 10000 && 10000 <= b[ii][jj]) {
                        if (is_next(i, j, ii, jj)) {
                            mat[b[i][j]][b[ii][jj] - 10000] = 100;
                        } else {
                            mat[b[i][j]][b[ii][jj] - 10000] = 10;
                        }
                    }
                }
            }
        }
    }

    Hungarian h(mat);
    cout << h.run() + abs(wcnt - bcnt) << endl;
}