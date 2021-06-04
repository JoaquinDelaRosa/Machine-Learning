#ifndef PRUNER_H
#define PRUNER_H

#include <../../../Shared/include/DataSet/DataSet.h>
#include <DecisionTree.h>
#include <../../../Shared/include/Outputs/Model.h>

class Pruner
{
    private:
        StatisticsManager statistics;
        void reducedErrorPrune(DataSet* testData, DecisionTree* root, DecisionTree* current);
    public:
        Pruner();
        DecisionTree* reducedErrorPrune(DataSet* testData, DecisionTree* root);
        DecisionTree* costComplexityPrune(DataSet* testData, DecisionTree* root);
};

#endif // PRUNER_H
