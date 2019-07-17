// iran

template<typename T>
T pow(T a, long long n, T e = 1) {
    T ret = e;
    while (n) {
        if (n & 1) ret *= a;
        a *= a;
        n >>= 1;
    }
    return ret;
}

template<int mod>
struct ModInt {
    int x;
    ModInt(int x_) { if ((x = x_ % mod + mod) >= mod) x -= mod; }
    ModInt& operator+=(ModInt rhs) { if ((x += rhs.x) >= mod) x -= mod; return *this; }
    ModInt& operator-=(ModInt rhs) { if ((x -= rhs.x) < 0) x += mod; return *this; }
    ModInt& operator*=(ModInt rhs) { x = (unsigned long long) x * rhs.x % mod; return *this; }
    ModInt& operator/=(ModInt rhs) { x = (unsigned long long) x * rhs.inv().x % mod; return *this; }

    ModInt operator-() const { return -x < 0 ? mod - x : -x; }
    ModInt operator+(ModInt rhs) const { return ModInt(*this) += rhs; }
    ModInt operator-(ModInt rhs) const { return ModInt(*this) -= rhs; }
    ModInt operator*(ModInt rhs) const { return ModInt(*this) *= rhs; }
    ModInt operator/(ModInt rhs) const { return ModInt(*this) /= rhs; }
    bool operator==(ModInt rhs) const { return x == rhs.x; }
    bool operator!=(ModInt rhs) const { return x != rhs.x; }
    ModInt inv() const { return pow(*this, mod - 2); }

    friend ostream& operator<<(ostream& s, ModInt<mod> a) { s << a.x; return s; }
    friend istream& operator>>(istream& s, ModInt<mod>& a) { s >> a.x; return s; }
};

using mint = ModInt<(int) 1e9 + 7>;