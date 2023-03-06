import sys
import json


def transpose(x: list):
    ans = list()
    for i in range(len(x)):
        line = x[i]
        if type(line) != list:
            raise ValueError
        for j in range(len(x[i])):
            if i == 0:
                ans.append(list())
            ans[j].append(x[i][j])
    return ans


def det(x: list):
    ans = 0
    n = len(x)
    if n == 0:
        raise ValueError
    for i in range(n):
        line = x[i]
        if type(line) != list or len(line) != n:
            raise ValueError
    if n == 1:
        return x[0][0]
    for i in range(n):
        temp = list()
        for j in range(1, n):
            temp.append(list())
            for k in range(n):
                if k != i:
                    temp[j - 1].append(x[j][k])
        ans += x[0][i] * pow(-1, i) * det(temp)
    return ans


def algebraic_additions(x: list):
    ans = list()
    n = len(x)
    if n == 0:
        raise ValueError
    for i in range(n):
        line = x[i]
        if type(line) != list or len(line) != n:
            raise ValueError
    for i in range(n):
        ans.append(list())
        for j in range(n):
            minor = list()
            flag = True
            for k in range(n):
                if k == i:
                    flag = False
                    continue
                minor.append(list())
                for l in range(n):
                    if l == j:
                        continue
                    if flag:
                        minor[k].append(x[k][l])
                    else:
                        minor[k - 1].append(x[k][l])
            ans[i].append(pow(-1, i + j) * det(minor))
    return ans


def inverse(x: list):
    n = len(x)
    if n == 0:
        raise ValueError
    for i in range(n):
        line = x[i]
        if type(line) != list or len(line) != n:
            raise ValueError
    d = det(x)
    if d == 0:
        print("The inverse matrix does not exist!")
        raise ValueError
    ans = transpose(algebraic_additions(x))
    for i in range(n):
        for j in range(n):
            ans[i][j] /= d
    return ans


def dot(x: list, y: list):
    ans = list()
    n = len(x)
    m = len(y)
    if n == 0 or m == 0:
        raise ValueError
    l = 0
    for i in range(n):
        line = x[i]
        if type(line) != list or len(line) != m:
            raise ValueError
    y = transpose(y)
    for i in range(len(y)):
        line = y[i]
        l += 1
        if type(line) != list or len(line) != m:
            raise ValueError
    for k in range(l):
        for i in range(n):
            s = 0
            for j in range(m):
                s += x[i][j] * y[k][j]
            if k == 0:
                ans.append(list())
            ans[i].append(s)
    return ans


def mult(x: list, y: list):
    ans = 0
    n = len(x)
    if n == 0:
        raise ValueError
    l = None
    for i in range(n):
        if len(y) != n:
            raise ValueError
        line_x = x[i]
        line_y = y[i]
        m = len(line_x)
        if l is None:
            l = m
        elif m != l:
            raise ValueError
        for j in range(m):
            if len(line_y) != m:
                raise ValueError
            ans += line_x[j] * line_y[j]
    return ans


def method(x: list, matrix: list):
    x_new = dot(matrix, x)
    print(f"x1:        {transpose(x_new)[0]}")
    lmbd = mult(x, x_new) / mult(x, x)
    print(f"lambda 1 = {lmbd}")
    x = x_new
    x_new = dot(matrix, x)
    print(f"x2:        {transpose(x_new)[0]}")
    lmbd_new = mult(x, x_new) / mult(x, x)
    print(f"lambda 2 = {lmbd_new}")
    i = 3
    while abs(lmbd - lmbd_new) > e:
        x = x_new
        lmbd = lmbd_new
        x_new = dot(matrix, x)
        print(f"x{i}:        {transpose(x_new)[0]}")
        lmbd_new = mult(x, x_new) / mult(x, x)
        print(f"lambda {i} = {lmbd_new}")
        i += 1
    return lmbd_new


def main(argv: list):
    if len(argv) < 2:
        print("Not enough arguments!")
        return 1
    text = list()
    try:
        with open(str(argv[1]), 'r') as matrix_file:
            for line in matrix_file.read().split('\n'):
                for word in line.split(' '):
                    text.append(word)
    except Exception:
        print("File Error!")
        return 2
    matrix = list()
    try:
        n = int(text[0])
        for i in range(n):
            matrix.append(list())
            for j in range(n):
                matrix[i].append(float(text[i * n + j + 1]))
    except Exception:
        print("File value Error!")
        return 2
    try:
        inv_matrix = inverse(matrix)
        print("Матрица:")
        for i in range(n):
            print(matrix[i])
        print("Обратная матрица:")
        for i in range(n):
            print(inv_matrix[i])
        for x0 in x0s:
            print(f"Начальный вектор: {x0}")
            x0 = transpose([x0])
            print("Прямой ход")
            print(f"Ответ: {method(x0, matrix)}")
            print("Обратный ход")
            print(f"Ответ: {1 / method(x0, inv_matrix)}")
    except ValueError:
        print("Something went wrong!")
        return 3
    return 0


if __name__ == '__main__':
    with open("config.json", 'r') as config_file:
        data = json.load(config_file)
    e = data['error']
    x0s = data['initial_vectors']
    response = main(sys.argv)
