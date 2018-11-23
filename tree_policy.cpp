#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
#define endl '\n'

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    int q;
    cin >> q;
    tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> s;
    while (q--) {
        int t, x;
        cin >> t >> x;
        if (t == 1) {
            s.insert(x);
        } else {
            int k = *s.find_by_order(x - 1);
            cout << k << endl;
            s.erase(k);
        }
    }
}
