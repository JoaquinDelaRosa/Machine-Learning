#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H

#include "../../../Shared/include/Outputs/Model.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "NaiveBayes.h"
#include "bits/stdc++.h"

// Time: Approx 1 hr 30 mins

class NaiveBayes : public Model
{
    private:
        DataSet* dataset;
        // This follows the syntax A | B.
        std::map<std::pair<std::string, std::string>, double> conditionalprobabilities;
        std::map<std::string, double> targetprobabilities;

        std::string target;
        int smoothing;

    public:
        NaiveBayes(std::string target, int smoothing = 1);
        void grow(DataSet* trainingdata) override;
        double test(DataSet* testdata) override;

        void updateProbabilities();
        std::string evaluate(std::map<std::string, std::string> query);


        void print();
};

#endif // NAIVEBAYES_H
