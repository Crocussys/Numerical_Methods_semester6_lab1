#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

vector<vector<long double>> transpose(vector<vector<long double>> x){
    vector<vector<long double>> ans;
    for (int i = 0; i < x[0].size(); i++){
        vector<long double> temp;
        for (int j = 0; j < x.size(); j++){
            temp.push_back(x[j][i]);
        }
        ans.push_back(temp);
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
        vector<vector<long double>> temp1;
        for (int j = 1; j < n; j++){
            vector<long double> temp2;
            for (int k = 0; k < n; k++){
                if (k != i){
                    temp2.push_back(x[j][k]);
                }
            }
            temp1.push_back(temp2);
        }
        ans += x[0][i] * pow(-1, i) * det(temp1);
    }
    return ans;
}

vector<vector<long double>> algebraic_additions(vector<vector<long double>> x){
    vector<vector<long double>> ans;
    int n = x.size();
    for (int i = 0; i < n; i++){
        vector<long double> temp1;
        for (int j = 0; j < n; j++){
            vector<vector<long double>> minor;
            for (int k = 0; k < n; k++){
                if (k == i){
                    continue;
                }
                vector<long double> temp2;
                for (int l = 0; l < n; l++){
                    if (l == j){
                        continue;
                    }
                    temp2.push_back(x[k][l]);
                }
                minor.push_back(temp2);
            }
            temp1.push_back(pow(-1, i + j) * det(minor));
        }
        ans.push_back(temp1);
    }
    return ans;
}

vector<vector<long double>> inverse(vector<vector<long double>> x){
    int n = x.size();
    long double d = det(x);
    if (d == 0){
        cout << "The inverse matrix does not exist!" << endl;
        throw (4);
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
        vector<long double> temp;
        for (int i = 0; i < n; i++){
            long double s = 0;
            for (int j = 0; j < m; j++){
                s += x[i][j] * y[k][j];
            }
            temp.push_back(s);
        }
        ans.push_back(temp);
    }
    return ans;
}

long double mult(vector<vector<long double>> x, vector<vector<long double>> y){
    long double ans = 0;
    int n = x.size();
    int m = x[0].size();
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            ans += x[i][j] * y[i][j];
        }
    }
    return ans;
}

void print_vector(vector<long double> arr){
    int n = arr.size();
    cout << "[";
    for (int j = 0; j < n; j++){
        cout << arr[j];
        if (j != n - 1){
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

long double method(vector<vector<long double>> x, vector<vector<long double>> matrix, long double e){
    vector<vector<long double>> x_new = dot(matrix, x);
    cout << "x1:        ";
    print_vector(x_new[0]);
    x_new = transpose(x_new);
    long double lmbd = mult(x, x_new) / mult(x, x);
    cout << "lambda 1 = " << lmbd << endl;
    x = x_new;
    x_new = dot(matrix, x);
    cout << "x2:        ";
    print_vector(x_new[0]);
    x_new = transpose(x_new);
    long double lmbd_new = mult(x, x_new) / mult(x, x);
    cout << "lambda 2 = " << lmbd_new << endl;
    int i = 3;
    while (abs(lmbd - lmbd_new) > e){
        x = x_new;
        lmbd = lmbd_new;
        x_new = dot(matrix, x);
        cout << "x" << i << ":        ";
        print_vector(x_new[0]);
        x_new = transpose(x_new);
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
        vector<long double> temp;
        for (int j = 0; j < n; j++){
            long double temp_value;
            matrix_file >> temp_value;
            temp.push_back(temp_value);
        }
        matrix.push_back(temp);
    }
    matrix_file.close();

    fstream config_file("config.txt", ios::in);
    if (!config_file){
        cout << "File open error!" << endl;
        return 2;
    }
    config_file >> e;
    while (!config_file.eof()){
        vector<long double> temp;
        for (unsigned long j = 0; j < n; j++){
            long double temp_value;
            config_file >> temp_value;
            temp.push_back(temp_value);
        }
        x0s.push_back(temp);
    }
    config_file.close();

    try {
        vector<vector<long double>> inv_matrix = inverse(matrix);
        cout << "Матрица:" << endl;
        for (int i = 0; i < n; i++){
            print_vector(matrix[i]);
        }
        cout << "Обратная матрица:" << endl;
        for (int i = 0; i < n; i++){
            print_vector(inv_matrix[i]);
        }
        for (int i = 0; i < x0s.size(); i++){
            cout << "Начальный вектор: ";
            print_vector(x0s[i]);
            vector<vector<long double>> temp;
            temp.push_back(x0s[i]);
            vector<vector<long double>> x0 = transpose(temp);
            cout << "Прямой ход" << endl;
            long double ans = method(x0, matrix, e);
            cout << "Ответ: " << ans << endl;
            cout << "Обратный ход" << endl;
            ans = 1 / method(x0, inv_matrix, e);
            cout << "Ответ: " << ans << endl;
        }
    } catch (int err) {
        cout << "Something went wrong!" << endl;
        return 3;
    }
    return 0;
}