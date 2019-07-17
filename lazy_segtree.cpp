// hoge
#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;

class LazySegTree {
  int n;
  vi node, lazy;

public:
  LazySegTree(vi v) {
    int s = int(v.size());
    n = 1;
    while (n < s) {
      n <<= 1;
    }
    node.resize(2 * n - 1);
    lazy.resize(2 * n - 1 , 0);

    for (int i = 0; i < s; i++) {
      node[i + n - 1] = v[i];
    }
    for (int i = n - 2; i >= 0; i--) {
      node[i] = node[i * 2 + 1] + node[i * 2 + 2];
    }
  }

  void eval(int k, int l, int r) {
    if (lazy[k]) {
      node[k] += lazy[k];
      if (r - l > 1) {
        lazy[2 * k + 1] += lazy[k] / 2;
        lazy[2 * k + 2] += lazy[k] / 2;
      }

      lazy[k] = 0;
    }
  }

  void add(int a, int b, i64 x, int k = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;
    eval(k, l, r);
    if (b <= l || r <= a) {
      return;
    }
    if (a <= l && r <= b) {
      lazy[k] += (r - l) * x;
      eval(k, l, r);
    } else {
      add(a, b, x, 2 * k + 1, l, (l + r) / 2);
      add(a, b, x, 2 * k + 2, (l + r) / 2, r);
      node[k] = node[2 * k + 1] + node[2 * k + 2];
    }
  }

  i64 rangeSum(int a, int b, int k = 0, int l = 0, int r = -1) {
    if (r < 0) r = n;
    eval(k, l, r);
    if (b <= l || r <= a) {
      return 0;
    }
    if (a <= l && r <= b) {
      return node[k];
    }
    i64 vl = rangeSum(a, b, 2 * k + 1, l, (l + r) / 2);
    i64 vr = rangeSum(a, b, 2 * k + 2, (l + r) / 2, r);
    return vl + vr;
  }

  void set(int a, i64 x) {
    add(a, a + 1, -get(a));
    add(a, a + 1, x);
  }

  i64 get(int a) {
    return rangeSum(a, a + 1);
  }
};

int main() {
    int n;
    cin >> n;
    vi as(n);
    LazySegTree seg(as);
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        char c;
        cin >> c;
        if (c == 'A') {
            int id, x;
            cin >> id >> x;
            seg.set(id - 1, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << seg.rangeSum(l - 1, r) << endl;
        }
    }
}