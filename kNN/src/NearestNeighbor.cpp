#include "NearestNeighbor.h"
#include "Dataset/DataSet.h"
#include "KDTree.h"

NearestNeighbor::NearestNeighbor(std::string target) : Model {}{
    this->dataset = nullptr;
    this->target = target;
}

DataSet* NearestNeighbor::getInternalData(){
    return this->dataset;
}

void NearestNeighbor::updateDataSet(DataSet* trainingdata){
    if(this->dataset == nullptr){
        dataset = trainingdata;
    }
    else if(trainingdata != nullptr){
        // Extend our model by adding instances from the training data.
        // We assume that the training data and our current data set have the same features in the same order.
        for(int i = 0; i < trainingdata->getEntryCount(); i++){
            this->dataset->addEntry(trainingdata->getEntryAt(i));
        }
    }
    else{
        // Assume that setting it as nullptr means we want to reset our data
        this->dataset = nullptr;
    }
}

void NearestNeighbor::grow(DataSet* trainingdata){
    this->updateDataSet(trainingdata);
    this->tree = new KDTree();

    tree->grow(trainingdata, this->target);

}

double NearestNeighbor::test(DataSet* testdata){
    // Return the error
    double error = 0;

    for(int i = 0; i < testdata->getEntryCount(); i++){
        std::map<std::string, std::string> *query = testdata->makeQuery(i);

        std::string result = this->tree->query(*query);
        if(testdata->isFeatureCategorical(target)){
            error++;
        }
        else{
            double e = stod(testdata->getEntryFeatureAt(i, target));
            double r = stod(result);
            error += (e - r) * (e -r);
        }
    }

    if(this->dataset->isFeatureCategorical(target))
        error = error / testdata->getEntryCount();

    return error;
}
