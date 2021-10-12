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
    vector<vector<double>> adjoint = matrix_coef.getAdjoint();
    vector<vector<double>> inverse = matrix_coef.getInverse();
    vector<double> solve = matrix_coef.getVectorSolve({{-3}, {2}, {-2}, {-19}});
    /*
    for (int i = 0; i < (int)adjoint.size(); ++i)
    {
        for (int j = 0; j < (int)adjoint[0].size(); ++j)
        {
            cout << inverse[i][j] << endl;
        }
    }
    */
    for (int i = 0; (int)i < solve.size(); ++i)
    {
        cout << solve[i] << endl;
    }
    matrix_coef.getMatrixInfo();
    return 0;
}