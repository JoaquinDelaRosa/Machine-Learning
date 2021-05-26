#ifndef PRUNER_H
#define PRUNER_H

#include <DataSet.h>
#include <DecisionTree.h>
#include <StatisticsManager.h>

class Pruner
{
    private:
        StatisticsManager statistics;
        void reducedErrorPrune(DataSet* testData, DecisionTree* root, DecisionTree* current);
    public:
        Pruner();
        DecisionTree* reducedErrorPrune(DataSet* testData, DecisionTree* root);
        DecisionTree* costComplexityPrune(DataSet* testData, DataSet* validationData, DecisionTree* root);
        DecisionTree* costComplexityPrune(DataSet* testData, DecisionTree* root, double alpha);
};

#endif // PRUNER_H
