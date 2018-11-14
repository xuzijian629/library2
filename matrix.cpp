#include <bits/stdc++.h>
using namespace std;
using i64 = int64_t;
using vi = vector<i64>;
using vvi = vector<vi>;
constexpr long double EPS = 1e-12;
constexpr i64 MOD = 1e9 + 7;

template<typename T>
class mat : public vector<vector<T>> {
private:
    int r, c;
public:
    int row() const {
        return r;
    }
    int column() const {
        return c;
    }
    mat(int n, int m, T val = 0) {
        r = n;
        c = m;
        for (int i = 0; i < n; i++) {
            this->push_back(vector<T>(m, val));
        }
    }
    mat operator+(const mat& rhs) {
        assert(r == rhs.r && c == rhs.c);
        mat<T> ret(r, c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ret[i][j] = (*this)[i][j] + rhs[i][j];
            }
        }
        return ret;
    }
    mat operator+(const T val) {
        mat<T> ret(r, c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ret[i][j] = (*this)[i][j] + val;
            }
        }
        return ret;
    }
    mat operator-(const mat& rhs) {
        assert(r == rhs.r && c == rhs.c);
        mat<T> ret(r, c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ret[i][j] = (*this)[i][j] - rhs[i][j];
            }
        }
        return ret;
    }
    mat operator-(const T val) {
        mat<T> ret(r, c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ret[i][j] = (*this)[i][j] - val;
            }
        }
        return ret;
    }
    vector<T> operator*(const vector<T>& rhs) {
        assert(c == rhs.size());
        vector<T> vec(r, 0);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                vec[i] += (*this)[i][j] * rhs[j];
            }
        }
        return vec;
    }
    mat operator*(const mat& rhs) {
        assert(c == rhs.r);
        mat<T> ret(r, rhs.c);
        for (int i = 0; i < r; i++) {
            for (int k = 0; k < c; k++) {
                for (int j = 0; j < rhs.c; j++) {
                    ret[i][j] += (*this)[i][k] * rhs[k][j];
                }
            }
        }
        return ret;
    }
    mat operator-() {
        mat<T> ret(r, c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                ret[i][j] = -(*this)[i][j];
            }
        }
        return ret;
    }
	int rank() {
	    int res = 0;
	    mat<double> B(r, c);
	    for (int i = 0; i < r; i++) {
	        for (int j = 0; j < c; j++) {
				B[i][j] = (*this)[i][j];
			}
	    }
	    for (int i = 0; i < c; i++) {
	        if (res == r) return res;
	        int pivot = res;
	        for (int j = res + 1; j < r; j++) {
	            if (abs(B[j][i]) > abs(B[pivot][i])) {
	                pivot = j;
	            }
	        }
	        if (abs(B[pivot][i]) < EPS) continue;
	        swap(B[pivot], B[res]);
	        for (int j = i + 1; j < c; j++) {
	            B[res][j] /= B[res][i];
	        }
	        for (int j = res + 1; j < r; j++) {
	            for (int k = i + 1; k < c; k++) {
	                B[j][k] -= B[res][k]*B[j][i];
	            }
	        }
	        res++;
	    }
	    return res;
	}
    T det() {
        assert(r == c);
        T ans = 1;
        mat B(r, r);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < r; j++) {
                B[i][j] = (*this)[i][j];
            }
        }
        for (int i = 0; i < r; i++) {
            for (int j = i + 1; j < r; j++) {
                for (; B[j][i] != 0; ans = -ans) {
                    T tm = B[i][i] / B[j][i];
                    for (int k = i; k < r; k++) {
                        T t = B[i][k] - tm * B[j][k];
                        B[i][k] = B[j][k];
                        B[j][k] = t;
                    }
                }
           }
           ans *= B[i][i];
       }
       return ans;
    }
    mat inverse() {
        assert(r == c);
        mat B(r, 2 * r);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < r; j++) {
                B[i][j] = (*this)[i][j];
            }
        }
        for (int i = 0; i < r; i++) {
            B[i][r + i] = 1;
        }
        for (int i = 0; i < r; i++) {
            int pivot = i;
            for (int j = i; j < r; j++) {
                if (abs(B[j][i]) > abs(B[pivot][i])) {
                    pivot = j;
                }
            }

            // assert regular
            assert(abs(B[pivot][i]) > EPS);
            swap(B[i], B[pivot]);
            for (int j = i + 1; j <= 2 * r; j++) {
                B[i][j] /= B[i][i];
            }
            for (int j = 0; j < r; j++) {
                if (i != j) {
                    for (int k = i + 1; k <= 2*r; k++) {
                        B[j][k] -= B[j][i] * B[i][k];
                    }
                }
            }
        }
        mat res(r, r);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < r; j++) {
                res[i][j] = B[i][r + j];
            }
        }
        return res;
    }
    void print() {
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c - 1; j++) {
                cout << (*this)[i][j] << '\t';
            }
            cout << (*this)[i][c - 1] << endl;
        }
    }
};

template<typename T>
vector<T> eq_solve(const mat<T>& A,const vector<T>& b) {
    assert(A.row() == A.column());
    int n = A.row();
    mat<T> B(n, n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = A[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        B[i][n] = b[i];
    }
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i; j < n; j++) {
            if (abs(B[j][i]) > abs(B[pivot][i])) {
                pivot = j;
            }
        }
        // assert having a unique root
        assert(abs(B[pivot][i]) > EPS);
        swap(B[i], B[pivot]);
        for (int j = i + 1; j <= n; j++) {
            B[i][j] /= B[i][i];
        }
        for (int j = 0; j < n; j++) {
            if (i != j) {
                for (int k = i + 1; k <= n; k++) {
                    B[j][k] -= B[j][i] * B[i][k];
                }
            }
        }
    }
    vector<T> ret(n);
    for (int i = 0; i < n; i++) {
        ret[i] = B[i][n];
    }
    return ret;
}

template<typename T>
mat<T> pow(mat<T> A, i64 k) {
    assert(A.row() == A.column());
    int n = A.row();
	mat<T> B(n, n);
	for (int i = 0; i < n; i++) {
		B[i][i] = 1;
	}
	while (k > 0) {
		if (k & 1) {
			B = B * A;
		}
		A = A * A;
		k >>= 1;
	}
	return B;
}

template<typename T>
mat<T> mod_mul(mat<T>& A, mat<T>& B) {
    assert(A.column() == B.row());
    mat<T> ret(A.row(), B.column());
    for (int i = 0; i < A.row(); i++) {
        for (int k = 0; k < A.column(); k++) {
            for (int j = 0; j < B.column(); j++) {
                ret[i][j] = (ret[i][j] + A[i][k]*B[k][j]) % MOD;
            }
        }
    }
    return ret;
}

template<typename T>
mat<T> mod_pow(mat<T> A, i64 k) {
    assert(A.row() == A.column());
    int n = A.row();
	mat<T> B(n, n);
	for (int i = 0; i < n; i++) {
		B[i][i] = 1;
	}
	while (k > 0) {
		if (k & 1) {
			B = mod_mul(B, A);
		}
		A = mod_mul(A, A);
		k >>= 1;
	}
	return B;
}

int main() {
    int n;
    cin >> n;
    mat<i64> m(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char c;
            cin >> c;
            m[i][j] = c - '0';
        }
    }

    cout << (m.det() % 2 ? "Odd" : "Even") << endl;
}
