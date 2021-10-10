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
class SimpleMatrix
{
public:
    //constructor for defining a matrix
    SimpleMatrix(vector<vector<numbers>> matrix_coef)
    {
        mainMatrix_ = matrix_coef;
        mainRow_ = mainMatrix_.size();
        mainColumn_ = mainMatrix_[0].size();
    }

    void getMatrixInfo()
    {
        std::cout << "Matrix row:\t" << mainRow_ << std::endl;
        std::cout << "Matrix column:\t" << mainColumn_ << std::endl;
    }

    bool isSquareMatrix()
    {
        bool ans = (mainRow_ == mainColumn_) ? true : false;
        return ans;
    }

    numbers getDeterminant()
    {
        numbers ans = 0;
        vector<numbers> ansVector = this->findPRow(mainMatrix_);
        for (int i = 0; i < (int)ansVector.size(); ++i)
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
            currentMatrix = mainMatrix_;
        }
        for (int i = 0; i < (int)mainRow_; ++i)
        {
            vector<numbers> transposed_row = {};
            for (int j = 0; j < (int)mainColumn_; j++)
            {
                transposed_row.push_back(currentMatrix[j][i]);
            }
            ans.push_back(transposed_row);
        }
        return ans;
    }

    vector<vector<numbers>> getAdjoint()
    {
        int currentRowNumber = mainMatrix_.size();
        int currentColumnNumber = mainMatrix_[0].size();
        if (currentRowNumber < 2 && currentColumnNumber < 2)
        {
            return mainMatrix_;
        }
        vector<vector<numbers>> ans;
        for (int i = 0; i < (int)currentRowNumber; ++i)
        {
            vector<numbers> adjointRow = {};
            for (int j = 0; j < (int)currentColumnNumber; ++j)
            {
                int sign = ((i + j + 1) % 2 == 1) ? 1 : -1;

                // Determinant of Minor Handling //
                vector<vector<numbers>> minor = findMinor(i, j, mainMatrix_);
                vector<numbers> pRow = this->findPRow(minor);
                numbers pRowValue = 0;
                for (int k = 0; k < (int)pRow.size(); ++k)
                {
                    pRowValue += sign * pRow[k];
                }
                // END //

                adjointRow.push_back(pRowValue);
            }
            ans.push_back(adjointRow);
        }
        return ans;
    }

    //sequence specific, standardised to matrix multiplcation only
    // if scalar quantity is present, set to targetA
    vector<vector<numbers>> getMultiplication(vector<vector<numbers>> targetA, vector<vector<numbers>> targetB)
    {
        vector<vector<numbers>> ans;
        if (targetA[0].size() == 0)
        {
            return {{}};
        }
        if (targetA[0].size() == 1)
        {
            for (int i = 0; i < (int)targetB.size(); ++i)
            {
                vector<numbers> currentRow = {};
                for (int j = 0; j < (int)targetB[0].size(); ++j)
                {
                    currentRow.push_back(targetA[0][0] * targetB[i][j]);
                }
                ans.push_back(currentRow);
            }
        }
        else
        {
            if (targetA[0].size() == targetB.size())
            {
                for (int i = 0; i < (int)targetA.size(); ++i) // targetA row
                {
                    vector<numbers> currentRow = {};
                    for (int j = 0; j < (int)targetB[0].size(); ++j) // targetB column
                    {
                        numbers value = 0;
                        for (int k = 0; k < (int)targetB.size(); ++k) // multiplication instance
                        {
                            value += targetA[i][k] * targetB[k][j];
                        }
                        currentRow.push_back(value);
                    }
                    ans.push_back(currentRow);
                }
            }
            else
            {
                return {{}};
            }
        }
        return ans;
    }

    vector<vector<numbers>> getInverse()
    {
        numbers determinant = this->getDeterminant();
        if (determinant == 0)
        {
            return {{}};
        }
        vector<vector<numbers>> transposedMatrix = this->getTranspose(this->getAdjoint());
        vector<vector<numbers>> inverseMatrix = this->getMultiplication({{1 / determinant}}, transposedMatrix);
        return inverseMatrix;
    }

    vector<numbers> getVectorSolve(vector<vector<numbers>> vectorAns)
    {
        vector<numbers> ans = {};
        vector<vector<numbers>> inverseMatrix = this->getInverse();
        vector<vector<numbers>> ansRaw = this->getMultiplication(inverseMatrix, vectorAns);
        for (int i = 0; i < (int)ansRaw.size(); ++i)
        {
            ans.push_back(ansRaw[i][0]);
        }
        return ans;
    }

private:
    vector<vector<numbers>> mainMatrix_;
    unsigned int mainRow_, mainColumn_;

    numbers tbtDeterminant(vector<vector<numbers>> currentMatrix)
    {
        if (currentMatrix.size() == 2 && currentMatrix[0].size() == 2)
        {
            numbers ans = (currentMatrix[0][0] * currentMatrix[1][1]) - (currentMatrix[0][1] * currentMatrix[1][0]);
            return ans;
        }
        // can't resolve determinant
        return 0;
    }

    vector<vector<numbers>> findMinor(int row, int column, vector<vector<numbers>> currentMatrix)
    {
        vector<vector<numbers>> ans;
        int currentRowNumber = currentMatrix.size();
        int currentColumnNumber = currentMatrix[0].size();
        for (int i = 0; i < (int)currentRowNumber; ++i)
        {
            vector<numbers> remainingRow = {};
            for (int j = 0; j < (int)currentColumnNumber; ++j)
            {
                if (i == row || j == column)
                {
                    continue;
                }
                remainingRow.push_back(currentMatrix[i][j]);
            }
            // skip empty vector
            if (remainingRow.size() > 0)
            {
                ans.push_back(remainingRow);
            }
        }
        return ans;
    }

    vector<numbers> findPRow(vector<vector<numbers>> currentMatrix)
    {
        vector<numbers> ans;
        int currentRowNumber = currentMatrix.size();
        int currentColumnNumber = currentMatrix[0].size();
        if (currentRowNumber <= 2 && currentColumnNumber <= 2)
        {
            numbers ansValue = tbtDeterminant(currentMatrix);
            ans.push_back(ansValue);
            return ans;
        }
        // only deal with 0th row
        for (int j = 0; j < (int)currentRowNumber; ++j)
        {
            int sign = ((j + 1) % 2 == 1) ? 1 : -1;

            // Determinant of Minor Handling //
            numbers pRowValue = 0;
            vector<vector<numbers>> remainingMatrix = findMinor(0, j, currentMatrix);
            vector<numbers> pRow = findPRow(remainingMatrix);
            for (int k = 0; k < (int)pRow.size(); ++k)
            {
                pRowValue += sign * currentMatrix[0][j] * pRow[k];
            }
            // END //

            ans.push_back(pRowValue);
        }
        return ans;
    }
};