namespace std {
template <typename T0, typename T1>
class numeric_limits<pair<T0, T1>> {
public:
    static constexpr pair<T0, T1> min() { return {numeric_limits<T0>::min(), numeric_limits<T1>::min()}; }
    static constexpr pair<T0, T1> max() { return {numeric_limits<T0>::max(), numeric_limits<T1>::max()}; }
};
}  // namespace std

// ペアの第一要素でソートし、第一要素がxのものとxより左側のものに対し第二要素の累積を返す
template <typename T0, typename T1, bool ascending = true>
struct PairQuery {
    // 累積用。第二要素を管理
    SumUpdateQuery<T1, T1> tr;
    // 順序管理用
    MinUpdateQuery<pair<T0, T1>, pair<T0, T1>> tr2;
    int cnt = 0;

    void add(const pair<T0, T1> &a) {
        int p = tr2.binary_search(0, tr2.size(), a, !ascending);
        if (ascending) {
            tr.insert(p + 1, a.second);
            tr2.insert(p + 1, a);
        } else {
            if (p == -1) {
                tr.insert(tr.size(), a.second);
                tr2.insert(tr2.size(), a);
            } else {
                tr.insert(p, a.second);
                tr2.insert(p, a);
            }
        }
        cnt++;
    }

    // 第一要素がxのものとxより左側のものに対し第二要素の累積を返す
    T1 query(T0 x) {
        if (ascending) {
            int p = tr2.binary_search(0, tr2.size(), {x, numeric_limits<T1>::max()}, false);
            p++;
            return tr.query(0, p);
        } else {
            int p = tr2.binary_search(0, tr2.size(), {x, numeric_limits<T1>::min()});
            if (p == -1) p = cnt;
            return tr.query(0, p);
        }
    }

    void erase_at(int k) {
        assert(0 <= k && k < cnt);
        tr.erase(k);
        tr2.erase(k);
        cnt--;
    }

    void erase_value(const pair<T0, T1> &a) {
        int p = tr2.binary_search(0, tr2.size(), a, !ascending);
        if (ascending) {
            p++;
        } else {
            if (p == -1) p = cnt;
            p--;
        }
        assert(0 <= p && p < cnt && tr2[p] == a);
        erase_at(p);
    }

    int size() const { return cnt; }

    T1 sum(int k) { return tr.query(0, k); }

    pair<T0, T1> operator[](int k) { return tr2[k]; }

    void dump() { tr2.dump(); }
};