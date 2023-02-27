#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

vector<vector<long double>> transpose(vector<vector<long double>> x){
    vector<vector<long double>> ans;
    for (int i = 0; i < x.size(); i++){
        for (int j = 0; j < x[i].size(); j++){
            ans[j][i] = x[i][j];
        }
    }
    return ans;
}

long double det(vector<vector<long double>> x){
    long double ans = 0;
    int n = x.size();
    if (n == 1){
        return x[0][0];
    }
    for (int i = 0; i < n; i++){
        vector<vector<long double>> temp;
        for (int j = 1; j < n; j++){
            for (int k = 0; k < n; k++){
                if (k != i){
                    temp[j - 1].push_back(x[j][k]);
                }
            }
        }
        ans += x[0][i] * pow(-1, i) * det(temp);
    }
    return ans;
}

vector<vector<long double>> algebraic_additions(vector<vector<long double>> x){
    vector<vector<long double>> ans;
    int n = x.size();
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            vector<vector<long double>> minor;
            bool flag = true;
            for (int k = 0; k < n; k++){
                if (k == i){
                    flag = false;
                    continue;
                }
                for (int l = 0; l < n; l++){
                    if (l == j){
                        continue;
                    }
                    if (flag){
                        minor[k].push_back(x[k][l]);
                    } else {
                        minor[k - 1].push_back(x[k][l]);
                    }
                }
            }
            ans[i].push_back(pow(-1, i + j) * det(minor));
        }
    }
    return ans;
}

vector<vector<long double>> inverse(vector<vector<long double>> x){
    int n = x.size();
    long double d = det(x);
    if (d == 0){
        cout << "The inverse matrix does not exist!" << endl;
        return {};
    }
    vector<vector<long double>> ans = transpose(algebraic_additions(x));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            ans[i][j] /= d;
        }
    }
    return ans;
}

vector<vector<long double>> dot(vector<vector<long double>> x, vector<vector<long double>> y){
    vector<vector<long double>> ans;
    int n = x.size();
    int m = y.size();
    y = transpose(y);
    int l = y.size();
    for (int k = 0; k < l; k++){
        for (int i = 0; i < n; i++){
            long double s = 0;
            for (int j = 0; j < m; j++){
                s += x[i][j] * y[k][j];
            }
            ans[i].push_back(s);
        }
    }
    return ans;
}

long double mult(vector<vector<long double>> x, vector<vector<long double>> y){
    long double ans = 0;
    int n = x.size();
    int m = y.size();
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            ans += x[i][j] * y[i][j];
        }
    }
    return ans;
}

long double method(vector<vector<long double>> x, vector<vector<long double>> matrix, long double e){
    vector<vector<long double>> x_new = dot(matrix, x);
    vector<long double> line = transpose(x_new)[0];
    int n = line.size();
    cout << "x1:        [";
    for (int i = 0; i < n; i++){
        cout << line[i];
        if (i != n - 1){
            cout << ", ";
        }
    }
    cout << "]" << endl;
    long double lmbd = mult(x, x_new) / mult(x, x);
    cout << "lambda 1 = " << lmbd << endl;
    x = x_new;
    x_new = dot(matrix, x);
    line = transpose(x_new)[0];
    n = line.size();
    cout << "x2:        [";
    for (int i = 0; i < n; i++){
        cout << line[i];
        if (i != n - 1){
            cout << ", ";
        }
    }
    cout << "]" << endl;
    long double lmbd_new = mult(x, x_new) / mult(x, x);
    cout << "lambda 2 = " << lmbd_new << endl;
    int i = 3;
    while (abs(lmbd - lmbd_new) > e){
        x = x_new;
        lmbd = lmbd_new;
        x_new = dot(matrix, x);
        line = transpose(x_new)[0];
        n = line.size();
        cout << "x" << i << ":        [";
        for (int j = 0; j < n; j++){
            cout << line[j];
            if (j != n - 1){
                cout << ", ";
            }
        }
        cout << "]" << endl;
        lmbd_new = mult(x, x_new) / mult(x, x);
        cout << "lambda " << i << " = " << lmbd_new << endl;
        i += 1;
    }
    return lmbd_new;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout << "Not enough arguments!" << endl;
        return 1;
    }

    int n;
    float e;
    vector<vector<long double>> matrix;
    vector<vector<long double>> x0s;

    fstream matrix_file(argv[1], ios::in);
    if (!matrix_file){
        cout << "File open error!" << endl;
        return 2;
    }
    matrix_file >> n;
    for (unsigned long i = 0; i < n; i++){
        for(unsigned long j = 0; j < n; j++){
            matrix_file >> matrix[i][j];
        }
    }
    matrix_file.close();

    fstream config_file("config.txt", ios::in);
    if (!config_file){
        cout << "File open error!" << endl;
        return 2;
    }
    config_file >> e;
    unsigned long i = 0;
    while (!config_file.eof()){
        for (unsigned long j = 0; j < n; j++){
            config_file >> x0s[i][j];
        }
        i++;
    }
    config_file.close();

    vector<vector<long double>> inv_matrix = inverse(matrix);
    cout << "Матрица:" << endl;
    for (unsigned long i = 0; i < n; i++){
        vector<long double> line = matrix[i];
        n = line.size();
        cout << "[";
        for (int j = 0; j < n; j++){
            cout << line[j];
            if (j != n - 1){
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }
    cout << "Обратная матрица:" << endl;
    for (int i = 0; i < n; i++){
        vector<long double> line = inv_matrix[i];
        n = line.size();
        cout << "[";
        for (int j = 0; j < n; j++){
            cout << line[j];
            if (j != n - 1){
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }


    return 0;
}