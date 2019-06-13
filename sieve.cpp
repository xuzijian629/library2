#include <bits/stdc++.h>
using namespace std;

struct Sieve {
    vector<int> smallest_factor;
    vector<int> smallest_power;
    vector<int> moebius;
    vector<int> totient;
    vector<bool> prime;
    vector<int> primes;
    Sieve(int m) : smallest_factor(m + 1), smallest_power(m + 1), moebius(m + 1), totient(m + 1), prime(m + 1, true) {
        moebius[1] = totient[1] = 1;
        prime[0] = prime[1] = false;

        for (int i = 2; i <= m; i++) {
            if (prime[i]) {
                smallest_factor[i] = i;
                smallest_power[i] = 1;
                moebius[i] = -1;
                totient[i] = i - 1;
                primes.push_back(i);
            }

            for (int p : primes) {
                if (p > smallest_factor[i] || i * p > m) break;
                prime[i * p] = false;
                smallest_factor[i * p] = p;
                smallest_power[i * p] = smallest_factor[i] == p ? smallest_power[i] + 1 : 1;
                moebius[i * p] = smallest_factor[i] == p ? 0 : -moebius[i];
                totient[i * p] = smallest_factor[i] == p ? p * totient[i] : (p - 1) * totient[i];
            }
        }
    }
};

struct CatAndMice {
    long long countDirections(int n, int c) {
        if (c == n) return 8;
        Sieve sieve(n + 1);
        vector<long long> sum(n + 1);
        for (int i = 1; i <= n; i++) {
            sum[i] = sum[i - 1] + sieve.totient[i];
        }
        return (sum[n / c] - sum[n / (c + 1)]) * 8;
    }
};

int main() {
    CatAndMice sol;
    cout << sol.countDirections(5000000, 1) << endl;
}
