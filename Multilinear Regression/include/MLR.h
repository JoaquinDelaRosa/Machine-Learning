#ifndef MLR_H
#define MLR_H

#include <bits/stdc++.h>
#include "../../../Shared/include/Outputs/Model.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "../../../Shared/include/DataSet/Converter.h"

class MLR : public Model
{
    private:
        std::map<std::string, double> weights;
        double bias;
        double learningRate;
        std::string target;

        Converter* converter;

    public:
        MLR(std::string target, double alpha);
        void grow(DataSet* trainingData) override;
        double test(DataSet* testData) override;

        double evaluate(std::map<std::string, std::string> query);
        void print();
};

#endif // MLR_H
