#include "imports.h"

template <typename numbers = double>
class SimpleVector
{
public:
    // Default Constructor
    SimpleVector(){};
    // 1D Vector
    SimpleVector(vector<numbers> vector_coef, bool rowVector = true)
    {
        mainVector_ = vector_coef;
        vectorSize_ = (int)vector_coef.size();
        rowVector_ = rowVector;
    }

    void printOutline()
    {
        string line = "";
        if (rowVector_)
        {
            line += "|\t";
            for (int i = 0; i < (int)mainVector_.size(); ++i)
            {
                string num_text = to_string(mainVector_[i]);
                line += num_text.substr(0, num_text.find(".") + 3) + "  ";
            }
            line += "\t|";
        }
        else
        {
            for (int i = 0; i < (int)mainVector_.size(); ++i)
            {
                string num_text = to_string(mainVector_[i]);
                line += "|\t" + num_text.substr(0, num_text.find(".") + 3) + "\t|";
                if (i != mainVector_.size() - 1)
                {
                    line += "\n";
                }
            }
        }
        cout << line << endl;
    }
    // getter function (vector)
    vector<numbers> getVector()
    {
        return mainVector_;
    }
    // getter function (vector type)
    bool isRowVector()
    {
        return rowVector_;
    }

    SimpleVector<numbers> getTranspose()
    {
        return SimpleVector<numbers>(mainVector_, !rowVector_);
    }
    // scalar multiplication
    SimpleVector<numbers> operator*(numbers target)
    {
        vector<numbers> ans;
        for (int i = 0; i < (int)mainVector_.size(); ++i)
        {
            ans.push_back(mainVector_[i] * target);
        }
        return SimpleVector(ans, rowVector_); // size remain the same
    }

private:
    vector<numbers> mainVector_;
    unsigned int vectorSize_;
    bool rowVector_;
};

template <typename numbers = double>
class SimpleMatrix
{
public:
    // 2D Matrix
    SimpleMatrix(vector<vector<numbers>> matrix_coef)
    {
        mainMatrix_ = matrix_coef;
        mainRow_ = mainMatrix_.size();
        mainColumn_ = mainMatrix_[0].size();
    }
    void printOutline()
    {
        for (int i = 0; i < (int)mainMatrix_.size(); ++i)
        {
            string line = "|\t";
            for (int j = 0; (int)j < mainMatrix_[0].size(); ++j)
            {
                string num_text = to_string(mainMatrix_[i][j]);
                line += num_text.substr(0, num_text.find(".") + 3) + "  ";
            }
            line += "\t|";
            cout << line << endl;
        }
    }

    void getMatrixInfo()
    {
        std::cout << "Matrix row:\t\t" << mainRow_ << std::endl;
        std::cout << "Matrix column:\t\t" << mainColumn_ << std::endl;
        numbers determinant = this->getDeterminant();
        if (determinant)
        {
            std::cout << "Matrix determinant:\t" << determinant << endl;
        }
        else
        {
            std::cout << "Matrix is not unique" << endl;
        }
    }

    bool isSquareMatrix()
    {
        bool ans = (mainRow_ == mainColumn_) ? true : false;
        return ans;
    }
    // getter function
    vector<vector<numbers>> getMatrix()
    {
        return mainMatrix_;
    }

    numbers getDeterminant()
    {
        numbers ans = this->findDeterminant(mainMatrix_);
        return ans;
    }

    SimpleMatrix<numbers> getTranspose(vector<vector<numbers>> currentMatrix = {{}})
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
        return SimpleMatrix<numbers>(ans);
    }

    SimpleMatrix<numbers> getAdjoint()
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
                numbers determinantMinor = this->findDeterminant(minor);
                numbers pRowValue = sign * determinantMinor;
                // END //

                adjointRow.push_back(pRowValue);
            }
            ans.push_back(adjointRow);
        }
        return SimpleMatrix<numbers>(ans);
    }

    SimpleMatrix<numbers> getInverse()
    {
        numbers determinant = this->getDeterminant();
        if (determinant == 0)
        {
            return {{}};
        }
        SimpleMatrix<numbers> transposedMatrix = this->getTranspose(this->getAdjoint().getMatrix());
        SimpleMatrix<numbers> inverseMatrix = transposedMatrix.operator*(1 / determinant);
        return inverseMatrix;
    }
    // Operator overloading of type SimpleMatrix
    SimpleMatrix<numbers> operator*(SimpleMatrix<numbers> target)
    {
        // check valid
        vector<vector<numbers>> ans;
        if (mainMatrix_[0].size() != target.mainMatrix_.size())
        {
            throw invalid_argument("Matrix size does not match.");
        }
        for (int i = 0; i < (int)mainMatrix_.size(); ++i)
        {
            vector<numbers> currentRow = {};
            for (int j = 0; j < (int)target.mainMatrix_[0].size(); ++j)
            {
                numbers value = 0;
                for (int k = 0; k < (int)target.mainMatrix_.size(); ++k)
                {
                    value += mainMatrix_[i][k] * target.mainMatrix_[k][j];
                }
                currentRow.push_back(value);
            }
            ans.push_back(currentRow);
        }
        return SimpleMatrix<numbers>(ans);
    }
    // Operator overloading of type Scalar
    SimpleMatrix<numbers> operator*(numbers target)
    {
        vector<vector<numbers>> ans;
        for (int i = 0; i < (int)mainMatrix_.size(); ++i)
        {
            vector<numbers> currentRow = {};
            for (int j = 0; j < (int)mainMatrix_[0].size(); ++j)
            {
                currentRow.push_back(mainMatrix_[i][j] * target);
            }
            ans.push_back(currentRow);
        }
        return SimpleMatrix<numbers>(ans);
    }

private:
    vector<vector<numbers>> mainMatrix_;
    unsigned int mainRow_, mainColumn_;

    // private methods
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

    numbers findDeterminant(vector<vector<numbers>> currentMatrix)
    {
        numbers ans = 0;
        int currentRowNumber = currentMatrix.size();
        int currentColumnNumber = currentMatrix[0].size();
        if (currentRowNumber <= 2 && currentColumnNumber <= 2)
        {
            ans = tbtDeterminant(currentMatrix);
            return ans;
        }
        // only deal with 0th row
        for (int j = 0; j < (int)currentRowNumber; ++j)
        {
            int sign = ((j + 1) % 2 == 1) ? 1 : -1;

            // Determinant of Minor Handling //
            vector<vector<numbers>> remainingMatrix = findMinor(0, j, currentMatrix);
            numbers determinantMinor = findDeterminant(remainingMatrix);
            ans += sign * currentMatrix[0][j] * determinantMinor;
            // END //
        }
        return ans;
    }
};

/* OPERATOR OVERLOADING */

// SCALAR (other ordering handled in class declaration)
template <typename numbers = double>
SimpleVector<numbers> operator*(numbers target, SimpleVector<numbers> currentVector)
{
    vector<numbers> ans;
    for (int i = 0; i < (int)currentVector.getVector().size(); ++i)
    {
        ans.push_back(currentVector.getVector()[i] * target);
    }
    return SimpleVector<numbers>(ans, currentVector.isRowVector());
};
template <typename numbers = double>
SimpleMatrix<numbers> operator*(numbers target, SimpleMatrix<numbers> currentMatrix)
{
    vector<vector<numbers>> ans;
    for (int i = 0; i < (int)currentMatrix.getMatrix().size(); ++i)
    {
        vector<numbers> currentRow = {};
        for (int j = 0; j < (int)currentMatrix.getMatrix()[0].size(); ++j)
        {
            currentRow.push_back(currentMatrix.getMatrix()[i][j] * target);
        }
        ans.push_back(currentRow);
    }
    return SimpleMatrix<numbers>(ans);
}

// MATRIX and VECTOR
template <typename numbers = double>
SimpleVector<numbers> operator*(SimpleMatrix<numbers> currentMatrix, SimpleVector<numbers> target)
{
    vector<numbers> ans;
    if (target.isRowVector())
    {
        throw invalid_argument("Vector size does not match.");
    }
    else
    {
        // column to vector row check
        if (currentMatrix.getMatrix()[0].size() != target.getVector().size())
        {
            throw invalid_argument("Vector size does not match.");
        }
    }

    for (int i = 0; i < (int)currentMatrix.getMatrix().size(); ++i)
    {
        numbers value = 0;
        for (int j = 0; j < (int)currentMatrix.getMatrix()[0].size(); ++j)
        {
            value += target.getVector()[j] * currentMatrix.getMatrix()[j][i];
        }
        ans.push_back(value);
    }
    return SimpleVector<numbers>(ans, target.isRowVector());
}
template <typename numbers = double>
SimpleVector<numbers> operator*(SimpleVector<numbers> target, SimpleMatrix<numbers> currentMatrix)
{
    vector<numbers> ans;
    if (!target.isRowVector())
    {
        throw invalid_argument("Vector size does not match.");
    }
    else
    {
        // vector column to row check
        if (target.getVector().size() != currentMatrix.getMatrix().size())
        {
            throw invalid_argument("Vector size does not match.");
        }
    }

    for (int i = 0; i < (int)currentMatrix.getMatrix().size(); ++i)
    {
        numbers value = 0;
        for (int j = 0; j < (int)currentMatrix.getMatrix()[0].size(); ++j)
        {
            value += currentMatrix.getMatrix()[i][j] * target.getVector()[j];
        }
        ans.push_back(value);
    }
    return SimpleVector<numbers>(ans, target.isRowVector());
}

/* GENERAL FUNCTIONS */
template <typename numbers = double>
SimpleVector<numbers> getVectorSolve(SimpleMatrix<numbers> currentMatrix, SimpleVector<numbers> vectorAns)
{
    SimpleMatrix<numbers> inverseMatrix = currentMatrix.getInverse();
    SimpleVector<numbers> ans = inverseMatrix * vectorAns;
    return ans;
}