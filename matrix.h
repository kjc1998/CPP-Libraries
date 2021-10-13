#include "imports.h"
#include "base.h"

// Matrix Class
template <typename numbers = double>
class SimpleMatrix : public BaseMatrix<numbers>
{
public:
    SimpleMatrix(vector<vector<numbers>> main_matrix) : BaseMatrix<numbers>(main_matrix)
    {
        // inherited variables
        this->mainMatrix_ = main_matrix;
        this->matrixSize_.first = main_matrix.size();
        this->matrixSize_.second = main_matrix[0].size();
    }

    void getBaseMatrixInfo()
    {
        std::cout << "Matrix row:\t\t" << this->matrixSize_.first << std::endl;
        std::cout << "Matrix column:\t\t" << this->matrixSize_.second << std::endl;
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
        bool ans = (this->matrixSize_.first == this->matrixSize_.second) ? true : false;
        return ans;
    }

    numbers getDeterminant()
    {
        numbers ans = this->findDeterminant(this->mainMatrix_);
        return ans;
    }

    SimpleMatrix<numbers> getTranspose()
    {
        BaseMatrix<numbers> ans = this->getBaseTranspose();
        return SimpleMatrix<numbers>(ans.getBaseMatrix());
    }

    SimpleMatrix<numbers> getAdjoint()
    {
        if (this->matrixSize_.first < 2 && this->matrixSize_.second < 2)
        {
            return this->mainMatrix_;
        }
        vector<vector<numbers>> ans;
        for (int i = 0; i < this->matrixSize_.first; ++i)
        {
            vector<numbers> adjointRow = {};
            for (int j = 0; j < this->matrixSize_.second; ++j)
            {
                int sign = ((i + j + 1) % 2 == 1) ? 1 : -1;

                // Determinant of Minor Handling //
                vector<vector<numbers>> minor = findMinor(i, j, this->mainMatrix_);
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
        SimpleMatrix<numbers> adjoinMatrix = SimpleMatrix<numbers>(this->getAdjoint().getBaseMatrix());
        SimpleMatrix<numbers> transposedMatrix = adjoinMatrix.getTranspose();
        SimpleMatrix<numbers> inverseMatrix = transposedMatrix.operator*(1 / determinant);
        return inverseMatrix;
    }
    // Operator overloading of type Scalar
    SimpleMatrix<numbers> operator*(numbers target)
    {
        vector<vector<numbers>> ans;
        for (int i = 0; i < (int)this->mainMatrix_.size(); ++i)
        {
            vector<numbers> currentRow = {};
            for (int j = 0; j < (int)this->mainMatrix_[0].size(); ++j)
            {
                currentRow.push_back(this->mainMatrix_[i][j] * target);
            }
            ans.push_back(currentRow);
        }
        return SimpleMatrix<numbers>(ans);
    }
    // Operator overloading of type SimpleMatrix
    SimpleMatrix<numbers> operator*(SimpleMatrix<numbers> target)
    {
        if (this->mainMatrix_[0].size() != target.mainMatrix_.size())
        {
            throw invalid_argument("Matrix size does not match.");
        }
        BaseMatrix<numbers> ans = BaseMatrix<numbers>(this->mainMatrix_) * BaseMatrix<numbers>(target.mainMatrix_);
        return SimpleMatrix<numbers>(ans);
    }

private:
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

// Vector Class
template <typename numbers = double>
class SimpleVector : public BaseMatrix<numbers>
{
public:
    SimpleVector(vector<numbers> main_vector, bool row_vector = true) : BaseMatrix<numbers>(main_vector, row_vector)
    {
        vector<vector<numbers>> dummy;
        this->isVector_ = true;
        if (row_vector)
        {
            this->matrixSize_.first = 1;
            this->matrixSize_.second = main_vector.size();
            dummy.push_back(main_vector);
        }
        else
        {
            this->matrixSize_.first = main_vector.size();
            this->matrixSize_.second = 1;
            for (int i = 0; i < main_vector.size(); ++i)
            {
                dummy.push_back({main_vector[i]});
            }
        }
        this->mainMatrix_ = dummy;
    }

    vector<numbers> getVector()
    {
        vector<numbers> current_vector = this->toVector(this->getBaseMatrix());
        return current_vector;
    }

    bool isRowVector()
    {
        return this->matrixSize_.first == 1;
    }

    SimpleVector<numbers> getTranspose()
    {
        BaseMatrix<numbers> ans_base = this->getBaseTranspose();
        vector<numbers> ans_vector = this->toVector(ans_base.getBaseMatrix());
        return SimpleVector<numbers>(ans_vector, !this->isRowVector());
    }
    // Operator overloading of type Scalar
    SimpleVector<numbers> operator*(numbers target)
    {
        vector<numbers> ans;
        for (int i = 0; i < this->mainMatrix_.size(); ++i)
        {
            for (int j = 0; j < this->mainMatrix_[0].size(); ++j)
            {
                ans.push_back(this->mainMatrix_[i][j] * target);
            }
        }
        return SimpleVector(ans, this->isRowVector());
    }
    // Operator overloading of type SimpleVector (return BaseClass - Vector, Matrix)
    BaseMatrix<numbers> operator*(SimpleVector<numbers> target)
    {
        if (this->mainMatrix_[0].size() != target.mainMatrix_.size())
        {
            throw invalid_argument("Vector size does not match.");
        }
        BaseMatrix<numbers> ans = BaseMatrix<numbers>(this->mainMatrix_) * BaseMatrix<numbers>(target.mainMatrix_);
        return ans;
    }

private:
    vector<numbers> toVector(vector<vector<numbers>> current_matrix)
    {
        vector<numbers> ans;
        if (current_matrix.size() == 1)
        {
            ans = current_matrix[0];
        }
        else
        {
            for (int i = 0; i < current_matrix.size(); ++i)
            {
                ans.push_back(current_matrix[i][0]);
            }
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
    for (int i = 0; i < (int)currentMatrix.getBaseMatrix().size(); ++i)
    {
        vector<numbers> currentRow = {};
        for (int j = 0; j < (int)currentMatrix.getBaseMatrix()[0].size(); ++j)
        {
            currentRow.push_back(currentMatrix.getBaseMatrix()[i][j] * target);
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
    if (target.isRowVector() || currentMatrix.getBaseMatrix()[0].size() != target.getVector().size())
    {
        throw invalid_argument("Vector size does not match.");
    }
    for (int i = 0; i < (int)currentMatrix.getBaseMatrix().size(); ++i)
    {
        numbers value = 0;
        for (int j = 0; j < (int)currentMatrix.getBaseMatrix()[0].size(); ++j)
        {
            value += target.getVector()[j] * currentMatrix.getBaseMatrix()[j][i];
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
        if (target.getVector().size() != currentMatrix.getBaseMatrix().size())
        {
            throw invalid_argument("Vector size does not match.");
        }
    }

    for (int i = 0; i < (int)currentMatrix.getBaseMatrix().size(); ++i)
    {
        numbers value = 0;
        for (int j = 0; j < (int)currentMatrix.getBaseMatrix()[0].size(); ++j)
        {
            value += currentMatrix.getBaseMatrix()[i][j] * target.getVector()[j];
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