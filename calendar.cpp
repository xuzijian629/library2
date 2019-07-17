// さすがにいらん

#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;

class Calendar {
    regex yyyymmdd = regex("^\\d{4}/\\d{2}/\\d{2}$");
    regex mmdd = regex("^\\d{2}/\\d{2}$");
    const vvi days = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

public:
    // [year1, year2)
    i64 leap_count(i64 year1, i64 year2) {
        year2--;
        year1--;
        i64 cnt = year2 / 4 - year1 / 4;
        cnt -= (year2 / 100 - year1 / 100);
        cnt += (year2 / 400 - year1 / 400);
        return cnt;
    }

    bool is_leap(i64 year) {
        return year % 400 == 0 || (year % 4 == 0 && year % 100);
    }

    i64 date_count(i64 year1, i64 year2) {
        int c = leap_count(year1, year2);
        return 365 * (year2 - year1 - c) + 366 * c; 
    }

    // 0-indexed
    i64 ith_date(string date, bool is_leap = false) {
        assert(regex_match(date.begin(), date.end(), mmdd));
        int m = stoi(date.substr(0, 2)) - 1;
        int d = stoi(date.substr(3, 2)) - 1;
        return accumulate(days[is_leap].begin(), days[is_leap].begin() + m, 0) + d;
    }

    // 0-indexed
    i64 ith_date_posix(string date) {
        assert(regex_match(date.begin(), date.end(), yyyymmdd));
        int y = stoi(date.substr(0, 4));
        int m = stoi(date.substr(5, 2)) - 1;
        int d = stoi(date.substr(8, 2)) - 1;
        return date_count(1970, y) + ith_date(date.substr(5, 5), is_leap(y));
    }   

    string to_string(int date, bool is_leap = false) {
        int m = 0;
        while (m < 12 && date - days[is_leap][m] >= 0) {
            date -= days[is_leap][m++];
        }
        stringstream ss;
        ss << setw(2) << setfill('0') << m + 1 << "/" << setw(2) << setfill('0') << date + 1;
        return ss.str();
    }

    string to_string_posix(int date) {
        int l = 1970, r = 10000;
        while (l < r - 1) {
            int m = (l + r) / 2;
            if (date_count(1970, m) <= date) {
                l = m;
            } else {
                r = m;
            }
        }
        stringstream ss;
        ss << setw(4) << setfill('0') << l << "/" << to_string(date - date_count(1970, l), is_leap(l));
        return ss.str();
    }

    int get_day(int date) {
        // 0: Sun, 1: Mon, ...
        // 1970/01/01 is Thu
        return (date % 7 + 4) % 7;
    }

    void check() {
        for (int i = 0; i < 10000; i++) {
            string date = to_string_posix(i);
            assert(ith_date_posix(date) == i);
        }
    }
};

int main() {
    Calendar cal;
    cal.check();
    const int period = 2800;
    vi sum(period + 1);
    for (int i = 1; i <= period; i++) {
        stringstream ss;
        ss << setw(4) << setfill('0') << 2000 + i;
        int k = cal.ith_date_posix(ss.str() + "/07/23");
        if (cal.get_day(k) == 3) {
            sum[i] = sum[i - 1] + 1;
        } else {
            sum[i] = sum[i - 1];
        }
    }

    i64 n;
    cin >> n;
    n -= 2000;
    if (n <= period) {
        cout << sum[n] - sum[14] << endl;
    } else {
        i64 cnt = sum[period] - sum[14];
        i64 range = (n + period - 1) / period - 2;
        cnt += sum[period] * range;
        cnt += sum[(n - 1) % period + 1];
        cout << cnt << endl;
    }
}
