#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

vi z_algorithm(string s) {
    int n = s.size();
    vi A(n);
    A[0] = n;
    int i = 1, j = 0;
    while (i < n) {
        while (i + j < n && s[j] == s[i + j]) {
            j++;
        }
        A[i] = j;
        if (j == 0) {
            i++;
            continue;
        }
        int k = 1;
        while (i + k < n && k + A[k] < j) {
            A[i + k] = A[k];
            k++;
        }
        i += k;
        j -= k;
    }
    return A;
}

int main() {
    string s;
    cin >> s;
    int n = s.size();
    vi lcp = z_algorithm(s);
    reverse(s.begin(), s.end());
    vi lcp_back = z_algorithm(s);
    reverse(s.begin(), s.end());
    i64 ans = 0;
    for (int i = 3; i < n - 1; i++) {
        i64 x = i, y = n - i;
        if (x <= y) continue;
        i64 a = min(y - 1, lcp[x]);
        i64 c = min(y - 1, lcp_back[y]);
        if (a > 0 && c > 0 && a + c >= y) {
            ans += a + c - y + 1;
        }
    }
    cout << ans << endl;
}