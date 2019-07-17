// iran

#include <bits/stdc++.h>
using namespace std;

vector<string> split_string(string s) {
    istringstream iss(s);
    return vector<string>(istream_iterator<string>(iss), istream_iterator<string>());
}

struct GetAccepted {
    string answer(string question) {
        vector<string> vs = split_string(question);
        int cnt = 0;
        for (string& s: vs) {
            if (s == "not") {
                cnt++;
            }
        }
        return (cnt % 2 == 0 ? "True." : "False.");
    }
};

int main() {
    GetAccepted ac;
    cout << ac.answer("Do you want to get not not accepted?"s) << endl;
}