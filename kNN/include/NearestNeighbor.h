#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H
#include <Outputs/Model.h>
#include <Dataset/DataSet.h>
#include <KDTree.h>

class NearestNeighbor : public Model
{
    private:
        DataSet* dataset;
        KDTree* tree;

        std::string target;

        void updateDataSet(DataSet* trainingdata);

    public:
        NearestNeighbor(std::string target);
        DataSet* getInternalData();

        void grow(DataSet* trainingdata) override;
        double test(DataSet* testdata) override;
};

#endif // NEARESTNEIGHBOR_H
