#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <string>
#include <typeinfo>
#include <numeric>
#include <map>
#include <algorithm>
using namespace std;

template <typename numbers>
class TwoDMatrix
{
public:
    //constructor for defining a matrix
    TwoDMatrix(vector<vector<numbers>> matrix_coef)
    {
        inner_ = matrix_coef;
        row_ = inner_.size();
        column_ = inner_[0].size();
    }

    bool matrixInfo()
    {
        std::cout << "Matrix row:\t" << row_ << std::endl;
        std::cout << "Matrix column:\t" << column_ << std::endl;
    }

    bool isSquareMatrix()
    {
        bool ans = (row_ == column_) ? true : false;
        return ans;
    }

    numbers getDeterminant()
    {
        numbers ans = 0;
        vector<numbers> ansVector = this->findPRow(inner_, 0);
        for (int i = 0; i < ansVector.size(); ++i)
        {
            ans += ansVector[i];
        }
        return ans;
    }

    vector<vector<numbers>> getTranspose(vector<vector<numbers>> currentMatrix = {{}})
    {
        vector<vector<numbers>> ans;
        // default to defined matrix
        if (currentMatrix[0].empty())
        {
            currentMatrix = inner_;
        }
        for (int i = 0; i < row_; ++i)
        {
            vector<numbers> transposed_row = {};
            for (int j = 0; j < column_; j++)
            {
                transposed_row.push_back(currentMatrix[j][i]);
            }
            ans.push_back(transposed_row);
        }
        return ans;
    }

    vector<vector<numbers>> getAdjoint()
    {
        return this->findAdjoint();
    }

    vector<vector<numbers>> getMultiplication() {}

    vector<vector<numbers>> getInverse()
    {
        numbers determinant = this->getDeterminant();
        vector<vector<numbers>> transposedMatrix = this->getTranspose(this->findAdjoint());
        // matrix multiplication
        return transposedMatrix;
    }

private:
    // private variables and methods
    vector<vector<numbers>> inner_;
    unsigned int row_, column_;

    numbers tbtDeterminant(vector<vector<numbers>> currentMatrix)
    {
        if (currentMatrix.size() == 2 && currentMatrix[0].size() == 2)
        {
            numbers ans = (currentMatrix[0][0] * currentMatrix[1][1]) - (currentMatrix[0][1] * currentMatrix[1][0]);
            return ans;
        }
        // can't find determinant
        return 0;
    }

    vector<vector<numbers>> findMinor(int row, int column, vector<vector<numbers>> currentMatrix)
    {
        vector<vector<numbers>> ans;
        int currentRowNumber = currentMatrix.size();
        int currentColumnNumber = currentMatrix[0].size();
        for (int i = 0; i < currentRowNumber; ++i)
        {
            vector<numbers> remainingRow = {};
            for (int j = 0; j < currentColumnNumber; ++j)
            {
                if (i == row || j == column)
                {
                    continue;
                }
                remainingRow.push_back(currentMatrix[i][j]);
            }
            // can't push back an empty vector
            if (remainingRow.size() > 0)
            {
                ans.push_back(remainingRow);
            }
        }
        return ans;
    }

    vector<numbers> findPRow(vector<vector<numbers>> currentMatrix, int ithRow, bool coFactor = false)
    {
        vector<numbers> ans;
        int currentRowNumber = currentMatrix.size();
        int currentColumnNumber = currentMatrix[0].size();
        if (currentRowNumber <= 2 && currentColumnNumber <= 2)
        {
            // consistent with recursive return type
            numbers ansValue = tbtDeterminant(currentMatrix);
            ans.push_back(ansValue);
            return ans;
        }
        // only dealing with i_th row
        for (int j = 0; j < currentRowNumber; ++j)
        {
            int sign = ((ithRow + j + 1) % 2 == 1) ? 1 : -1;
            // resets in every loop
            numbers pRowValue = 0;
            vector<vector<numbers>> remainingMatrix = findMinor(ithRow, j, currentMatrix);

            vector<numbers> pRow = findPRow(remainingMatrix, 0); // will always return determinant of minor
            for (int k = 0; k < pRow.size(); ++k)
            {
                pRowValue += pRow[k];
            }
            // two types of operation
            numbers value;
            if (coFactor)
            {
                value = sign * pRowValue;
            }
            else
            {
                value = sign * currentMatrix[ithRow][j] * pRowValue;
            }
            ans.push_back(value);
        }
        return ans;
    }

    vector<vector<numbers>> findAdjoint()
    {
        int currentRowNumber = inner_.size();
        int currentColumnNumber = inner_[0].size();
        // square Matrix of One
        if (currentRowNumber < 2 && currentColumnNumber < 2)
        {
            return inner_;
        }
        vector<vector<numbers>> ans;
        for (int i = 0; i < currentRowNumber; ++i)
        {
            vector<numbers> pRow = this->findPRow(inner_, i, true);
            ans.push_back(pRow);
        }
        return ans;
    }
};

main()
{

    TwoDMatrix<double> matrix_coef({
        {1, 0, 4, -6},
        {2, 5, 0, 3},
        {-1, 2, 3, 5},
        {2, 1, -2, 3},
    });
    vector<vector<double>> matrix = matrix_coef.getInverse();
    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < matrix.size(); ++j)
        {
            cout << matrix[i][j] << endl;
        }
    }
    return 0;
}

/*
{3,8},
{4,6} -14

{6, 1, 1},
{4, -2, 5},
{2, 8, 7}, -306

{1, 0, 4, -6},
{2, 5, 0, 3},
{-1, 2, 3, 5},
{2, 1, -2, 3}, 

*/