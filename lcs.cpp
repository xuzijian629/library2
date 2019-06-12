#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

string LCS(const string &s, const string &t) {
	int n = s.size(), m = t.size();
	vvi lcs(n + 1, vi(m + 1));
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (s[i - 1] == t[j - 1]) {
				lcs[i][j] = max(lcs[i][j], lcs[i - 1][j - 1] + 1);
			} else {
				lcs[i][j] = max({lcs[i][j], lcs[i - 1][j], lcs[i][j - 1]});
			}
		}
	}
	string ret = "";
	for (int i = n, j = m; i > 0 && j > 0;) {
		if (lcs[i][j] == lcs[i - 1][j]) {
			i--;
			continue;
		}
		if (lcs[i][j] == lcs[i][j - 1]) {
			j--;
			continue;
		}
		assert(s[i - 1] == t[j - 1]);
		ret += s[i - 1];
		i--, j--;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

int main() {
	string s, t;
	cin >> s >> t;
	cout << LCS(s, t) << endl;
}
