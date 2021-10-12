#include "matrix.h"

int main()
{
    SimpleMatrix<> matrix_coef({
        {0, 1, 1, -2},
        {1, 2, -1, 0},
        {2, 4, 1, -3},
        {1, 4, -7, -1},
    });
    double determinant = matrix_coef.getDeterminant();
    SimpleMatrix<double> adjoint = matrix_coef.getAdjoint();
    SimpleMatrix<double> inverse = matrix_coef.getInverse();
    SimpleVector<double> solve = matrix_coef.getVectorSolve(SimpleVector<double>({-3.0, 2.0, -2.0, -19.0}, false));
    vector<double> solveVector = solve.getVector();
    SimpleMatrix<> objB({
        {1, 2, 3},
        {5, 6, 7},
        {3, 4, 5},
        {22, 1, 2},
    });
    matrix_coef *objB;
    matrix_coef * 2;
    matrix_coef.printOutline();
    solve.printOutline();
    solve.getTranspose().printOutline();
    return 0;
}