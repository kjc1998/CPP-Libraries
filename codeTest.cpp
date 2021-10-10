#include "matrix.h"

int main()
{
    SimpleMatrix<double> matrix_coef({
        {1, 2, 3},
        {2, 5, 3},
        {6, 2, 3},
    });
    vector<double> matrix = matrix_coef.getVectorSolve({{4}, {6}, {20}});
    for (int i = 0; i < (int)matrix.size(); ++i)
    {
        cout << matrix[i] << endl;
    }
    return 0;
}