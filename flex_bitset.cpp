#include <bits/stdc++.h>
using namespace std;

class Bitset {
    using dui = deque<uint64_t>;
    dui data;

    void trim() {
        while (data.size() && data.front() == 0) {
            data.pop_front();
        }
    }

public:
    Bitset(string s) {
        reverse(s.begin(), s.end());
        while (s.size() % 64) {
            s += '0';
        }
        reverse(s.begin(), s.end());
        for (int i = 0; i < s.size() / 64; i++) {
            data.push_back(stoull(s.substr(64 * i, 64 * (i + 1)), 0, 2));
        }
        trim();
    }

    void operator<<(const int n) {
        int size = data.size();
        int jump = (n + 63) / 64;
        for (int i = 0; i < jump; i++) {
            data.push_front(0);
        }

        int offset = n % 64;
        for (int i = 0; i < size; i++) {
            data[i] |= data[i + jump] >> (64 - offset);
            data[i + 1] |= data[i + jump] << offset;
            if (i == size - 1) {
                data[i + 1] &= (0ull - 1) << offset;
                for (int j = i + 2; j < data.size(); j++) {
                    data[j] = 0;
                }
            }
        }
        trim();
    }

    void print_data() {
        for (uint64_t d: data) {
                cout << d << ' ';
        }
        cout << endl;
    }
};

int main() {
    Bitset hoge("000110");
    hoge.print_data();
    hoge << 2;
    hoge.print_data();
}