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
    SimpleVector<double> solve = getVectorSolve(inverse, SimpleVector<double>({-3.0, 2.0, -2.0, -19.0}, false));
    vector<double> solveVector = solve.getVector();
    return 0;
}