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