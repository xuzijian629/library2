// added
#include <bits/stdc++.h>

using namespace std;

vector<int> manacher(const string &s) {
    int n = s.size();
    vector<int> ret(n);
    for (int i = 0, j = 0; i < n;) {
        while (i - j >= 0 && i + j < n && s[i - j] == s[i + j]) j++;
        ret[i] = j;
        int k = 1;
        while (i - k >= 0 && i + k < n && k + ret[i - k] < j) {
            ret[i + k] = ret[i - k];
            k++;
        }
        i += k;
        j -= k;
    }
    return ret;
}

int main() {
    int n;
    cin >> n;
    string s;
    cin >> s;
    string t = "$";
    for (char c: s) {
        t += c;
        t += '$';
    }
    vector<int> m = manacher(t);
    cout << *max_element(m.begin(), m.end()) - 1 << endl;
}