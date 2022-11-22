#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

double get_evkl_norm(const vector<double>& x) {
    double sum = 0;
    for (auto a : x) {
        sum += a * a;
    }
    return sqrt(sum);
}

vector<double> matr_vec_mult(vector<vector<double>> matr, vector<double> vec) {
    vector<double> res;
    for (int i = 0; i < matr.size(); i++){
        double sum = 0;
        for (int j = 0; j < matr.size(); j++){
            sum += matr[i][j] * vec[j];
        }
        res.push_back(sum);
    }
    return res;
}

vector<double> Gauss(vector<vector<double>> x, const vector<double>& vec) {
    int i = 0;
    double tmp;
    int m = x.size();

    for (auto v : vec) {
        x[i].push_back(v);
        i++;
    }

    vector<double> res;
    for (int i = 0; i < m; i++) {
        res.push_back(0);
    }

    for (i = 0; i < m; i++) {
        tmp = x[i][i];
        for (int j = m; j >= i; j--) {
            x[i][j] /= tmp;
        }
        for (int j = i + 1; j < m; j++) {
            tmp = x[j][i];
            for (int k = m; k >= i; k--)
                x[j][k] -= tmp * x[i][k];
        }
    }

    res[m - 1] = x[m - 1][m];
    for (int i = m - 2; i >= 0; i--) {
        res[i] = x[i][m];
        for (int j = i + 1; j < m; j++) {
            res[i] -= x[i][j] * res[j];
        }
    }
    return res;
}

vector<vector<double>> generate_matrix(int n, double a, double b, double por) {
    vector<vector<double>> matr;
    for (int i = 0; i < n; i++) {
        vector<double> arr;
        for (int j = 0; j < n; j++) {
            double r = a + (double)rand() / RAND_MAX * (b - a);
            if (i == j) {
                r = pow(r, por);
            }
            arr.push_back(r);
        }
        matr.push_back(arr);
    }
    return matr;
}

void input(const string& path, vector<vector<double>>& matr, vector<double>& vec) {
    ifstream fin(path);
    if (!fin.is_open()) {
        cout << "Error openning file\n";
        return;
    }
    string s;
    getline(fin, s);
    if (s == "input") {
        while (!s.empty()) {
            getline(fin, s);
            vector<double> x;
            string substr;
            for (int i = 0; i < s.size(); i++) {
                char c = s[i];
                if (c == ' ') {
                    x.push_back(stod(substr));
                    substr = "";
                } else {
                    substr += c;
                    if (i == s.size() - 1) {
                        x.push_back(stod(substr));
                    }
                }
            }
            if (!x.empty()) {
                matr.push_back(x);
            }
            x.clear();
        }
        getline(fin, s);
        string substr;
        for (int i = 0; i < s.size(); i++) {
            char c = s[i];
            if (c == ' ') {
                vec.push_back(stod(substr));
                substr = "";
            } else {
                substr += c;
                if (i == s.size() - 1) {
                    vec.push_back(stod(substr));
                }
            }
        }
    } else {
        getline(fin, s);
        int n = stoi(s);
        double a, b;
        fin >> a >> b;
        double por;
        fin >> por;
        matr = generate_matrix(n, a, b, por);
    }
    fin.close();
}

void print(const vector<vector<double>>& matr) {
    cout << '[';
    for (const auto& i : matr) {
        cout << '[';
        for (auto x : i) {
            cout << fixed << x << ", ";
        }
        cout << "],";
        cout << endl;
    }
    cout << ']' << endl;
}

void print(const vector<double>& vec) {
    cout << '[';
    for (auto x : vec) {
        cout << fixed << x << ", ";
    }
    cout << ']';
    cout << endl;
}

double check_vectors(vector<double> res, vector<double> v) {
    vector<double> diff;
    for (int i = 0; i < res.size(); i++) {
        diff.push_back(res[i] - v[i]);
    }
    return get_evkl_norm(diff);
}

int main() {
    vector<vector<double>> matr;
    vector<double> vec;
    for (int i = 2; i < 3; i++) {
        input("../tests/test" + to_string(i) + ".txt", matr, vec);
        cout << "TEST " << i << endl;
        vector<double> v;
        for (int i = 0; i < 5; i++) {
            v.push_back(-5 + (double)rand() / RAND_MAX * (5 + 5));
        }
        vec = matr_vec_mult(matr, v);
        auto res = Gauss(matr, vec);
        cout.precision(20);
        print(matr);
        cout << "--------\n";
        print(vec);
        cout << "--------\n";


        print(res);
        print(v);
        cout << check_vectors(res, v);


        cout << endl << "---------------" << endl;
    }
    return 0;
}