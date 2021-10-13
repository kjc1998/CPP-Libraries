#include "imports.h"

template <typename numbers = double>
class BaseMatrix
{
public:
    // 2D Matrix
    BaseMatrix(vector<vector<numbers>> main_matrix)
    {
        mainMatrix_ = main_matrix;
        matrixSize_.first = main_matrix.size();
        matrixSize_.second = main_matrix[0].size();
    }
    // 1D Vector (simple definition purpose)
    BaseMatrix(vector<numbers> main_matrix, bool row_vector = true)
    {
        vector<vector<numbers>> dummy;
        isVector_ = true;
        if (row_vector)
        {
            matrixSize_.first = 1;
            matrixSize_.second = main_matrix.size();
            dummy.push_back(main_matrix);
        }
        else
        {
            matrixSize_.first = main_matrix.size();
            matrixSize_.second = 1;
            for (int i = 0; i < matrixSize_.first; ++i)
            {
                dummy.push_back({main_matrix[i]});
            }
        }
        mainMatrix_ = dummy;
    }
    // print method
    void printOutline()
    {
        for (int i = 0; i < matrixSize_.first; ++i)
        {
            string line = "|\t";
            for (int j = 0; j < matrixSize_.second; ++j)
            {
                string num_text = to_string(this->mainMatrix_[i][j]);
                line += num_text.substr(0, num_text.find(".") + 3) + "  ";
            }
            line += "\t|";
            cout << line << endl;
        }
    }
    // getter (matrix)
    vector<vector<numbers>> getBaseMatrix()
    {
        return mainMatrix_;
    }
    // getter (size)
    pair<unsigned int, unsigned int> getBaseSize()
    {
        return matrixSize_;
    }

    // base operator * type
    BaseMatrix<numbers> operator*(BaseMatrix<numbers> target)
    {
        vector<vector<numbers>> ans;
        numbers value;
        if (matrixSize_.second != target.matrixSize_.first)
        {
            throw invalid_argument("Invalid size for multiplcation");
        }
        for (int i = 0; i < matrixSize_.first; ++i)
        {
            vector<numbers> currentRow = {};
            for (int j = 0; j < (int)target.matrixSize_.second; ++j)
            {
                value = 0;
                for (int k = 0; k < (int)target.matrixSize_.first; ++k)
                {
                    value += mainMatrix_[i][k] * target.mainMatrix_[k][j];
                }
                currentRow.push_back(value);
            }
            ans.push_back(currentRow);
        }
        BaseMatrix<numbers> final = this->checkType(ans);
        return final;
    }

protected:
    vector<vector<numbers>> mainMatrix_;
    pair<unsigned int, unsigned int> matrixSize_;
    bool isVector_ = false;

    // protected methods //
    BaseMatrix<numbers> getBaseTranspose()
    {
        vector<vector<numbers>> ans;
        for (int j = 0; j < matrixSize_.second; ++j)
        {
            vector<numbers> transposed_row = {};
            for (int i = 0; i < matrixSize_.first; ++i)
            {
                transposed_row.push_back(mainMatrix_[i][j]);
            }
            ans.push_back(transposed_row);
        }
        BaseMatrix<numbers> final = this->checkType(ans);
        return final;
    }
    BaseMatrix<numbers> checkType(vector<vector<numbers>> target)
    {
        if (target.size() != 1 and target[0].size() != 1)
        {
            return BaseMatrix<numbers>(target);
        }
        else if (target.size() == 1)
        {
            return BaseMatrix<numbers>(target[0]);
        }
        else
        {
            vector<numbers> ans;
            for (int i = 0; i < target.size(); ++i)
            {
                ans.push_back(target[i][0]);
            }
            return BaseMatrix<numbers>(ans, false);
        }
    }
};