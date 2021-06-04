#include "NearestNeighbor.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "KDTree.h"
#include "../../../Shared/include/DataSet/Converter.h"
#include "../../../Shared/include/DataSet/StatisticsManager.h"
#include "../include/Metric.h"

#define EPSILON 1e-15
extern Converter* converter;

NearestNeighbor::NearestNeighbor(std::string target, int k) : Model {}{
    this->dataset = nullptr;
    this->target = target;
    this->K = k;
    this->statistics = new StatisticsManager();
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

double getDistance(std::map<std::string, std::string> query, DataSet* entry){
    double dist = 0;
    Metric* m = new Metric();
    if(entry == nullptr)
        return 0;

    for(int i = 0; i < entry->getFeatureCount(); i++){
        if(entry->getFeatureAt(i).second == DataSet::Categorical)
            dist += m->levenshtein(query[entry->getFeatureAt(i).first], entry->getEntryFeatureAt(0, i));
        else{
            double temp = m->squareEuclidean(query[entry->getFeatureAt(i).first], entry->getEntryFeatureAt(0, i));
            dist += temp * temp;
        }
    }

    return dist;
}

std::string NearestNeighbor::getWeightedResult(std::map<std::string, std::string> query){
    if(dataset->isFeatureCategorical(target)){
        std::map<std::string, double> modal;
        for(int i = 0; i < K; i++){
            KDTree* answer = this->tree->query(query);
            if(answer == nullptr)
                continue;
            // Perform weighted voting.
            std::string label = answer->getEntries()->getEntryFeatureAt(answer->getLabel(), target);
            std::map<std::string, double>::iterator it = modal.find(label);

            double distance = getDistance(query, answer->getEntries()) + EPSILON;

            if(it != modal.end()){
                it->second+= 1.0 / distance;
            }
            else if(label != ""){
                modal.emplace(std::pair<std::string, int>(label , distance));
            }
            tree->unvisitChildren();
        }

        // Return the mode by iterating over map
        double maximum = modal.begin()->second;
        std::string mode = "";

        for(auto it = modal.begin(); it != modal.end(); ++it){
            if(it->second >= maximum){
                mode = it->first;
                maximum = it->second;
            }
        }

        return mode;
    }

    else{
        double result = 0;
        double total = 0;
        for(int i = 0; i < K; i++){
            KDTree* answer = this->tree->query(query);
            if(answer == nullptr)
                continue;

            // Get distance from the query
            double weight = getDistance(query, answer->getEntries()) + EPSILON;
            answer->getEntries()->printEntries();
            std::string label = answer->getEntries()->getEntryFeatureAt(answer->getLabel(), target);
            result += converter->toDouble(label) / (weight);
            total += 1.0 / weight;
            tree->unvisitChildren();
        }

        return converter->toString(result / total);
    }

}

double NearestNeighbor::test(DataSet* testdata){
    // Return the error
    double error = 0;

    for(int i = 0; i < testdata->getEntryCount(); i++){
        std::map<std::string, std::string> *query = testdata->makeQuery(i);

        std::string result = this->getWeightedResult(*query);
        if(testdata->isFeatureCategorical(target) && (*query)[target] != result){
            error++;
        }
        else{
            double e = stod(testdata->getEntryFeatureAt(i, target));
            double r = stod(result);
            error += (e - r) * (e -r);
        }

        tree->unmarkChildren();
    }

    if(this->dataset->isFeatureCategorical(target))
        error = error / testdata->getEntryCount();

    return error;
}
