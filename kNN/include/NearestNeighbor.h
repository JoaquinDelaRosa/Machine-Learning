#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H
#include <../../../Shared/include/Outputs/Model.h>
#include <../../../Shared/include/DataSet/StatisticsManager.h>
#include <../../../Shared/include/DataSet/DataSet.h>
#include <KDTree.h>

class NearestNeighbor : public Model
{
    private:
        DataSet* dataset;
        KDTree* tree;
        StatisticsManager* statistics;
        int K;

        std::string target;

        void updateDataSet(DataSet* trainingdata);
        std::string getWeightedResult(std::map<std::string, std::string> query);


    public:
        NearestNeighbor(std::string target, int k);
        DataSet* getInternalData();

        void grow(DataSet* trainingdata) override;
        double test(DataSet* testdata) override;
};

#endif // NEARESTNEIGHBOR_H
