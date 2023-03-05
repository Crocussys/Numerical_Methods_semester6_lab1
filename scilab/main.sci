e = 0.001
x0s = [1, 0, 0; 1, 1, 1; 1, 2, 3; 1, 3, 7]
matrx = [5, 6, 3; -1, 0, 1; 1, 2, 0]

function printmatrix(mtrx)
    [n, m] = size(mtrx)
    for i = 1:n do
        for j = 1:m do
            mprintf("%f ", mtrx(i, j))
        end
        mprintf("\n")
    end
endfunction

function printarray(array)
    [m, n] = size(array)
    for i = 1:n do
        mprintf("%f ", array(i))
    end
    mprintf("\n")
endfunction

function lmbd_new=method(x, matrx)
    x_new = matrx * x
    mprintf("x1:        ")
    printarray(x_new')
    lmbd = sum(x .* x_new) / sum(x .* x)
    mprintf("lambda 1 = %f\n", lmbd)
    x = x_new
    x_new = matrx * x
    mprintf("x2:        ")
    printarray(x_new')
    lmbd_new = sum(x .* x_new) / sum(x .* x)
    mprintf("lambda 2 = %f\n", lmbd_new)
    i = 3
    while abs(lmbd - lmbd_new) > e do
        x = x_new
        lmbd = lmbd_new
        x_new = matrx * x
        mprintf("x%i:             ", i)
        printarray(x_new')
        lmbd_new = sum(x .* x_new) / sum(x .* x)
        mprintf("lambda %i = %f\n", i, lmbd_new)
        i = i + 1
    end
endfunction

n = size(matrx)
n = n(1)
inv_matrx = inv(matrx)
mprintf("Матрица: \n")
printmatrix(matrx)
mprintf("Обратная матрица: \n")
printmatrix(inv_matrx)
count_vectors = size(x0s)
count_vectors = count_vectors(1)
for i = 1:count_vectors do
    x = x0s(i, :)
    mprintf("Начальный вектор: \n")
    printarray(x)
    mprintf("Прямой ход: \n")
    mprintf("Ответ: %f\n", method(x', matrx))
    mprintf("Обратный ход: \n")
    mprintf("Ответ: %f\n", 1 / method(x', inv_matrx))
end
