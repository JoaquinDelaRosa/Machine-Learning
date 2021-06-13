#include "MLR.h"
#include "bits/stdc++.h"
#include "../../../Shared/include/Outputs/Model.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "../../../Shared/include/DataSet/Converter.h"
#define EPSILON 1e-10

MLR::MLR(std::string target, double alpha) : Model()
{
    this->target = target;
    this->learningRate = alpha;
    this->bias = 0;
    this->converter = new Converter();
    this->weights = *(new std::map<std::string, double>());
}

void MLR::grow(DataSet* trainingData){
    // Initialize weights and bias.
    for(int i = 0 ; i < trainingData->getFeatureCount(); i++){
        std::string feature = trainingData->getFeatureAt(i).first;
        if(feature != target){
            this->weights.emplace(std::pair<std::string, double>(feature, 0.1));
        }
    }

    // Iteratively learn
    double delta= 0;
    double l2 = 0;
    double prev = 0;
    int N = trainingData->getEntryCount();
    do{
        prev = l2;
        l2 = 0;
        for(int j = 0; j < N; j++){
            std::map<std::string, std::string> query = *(trainingData->makeQuery(j));
            double error = (evaluate(query) - converter->toDouble(query[target])) / N;
            l2 += error;
            // We divide by N to normalize the errors.
            for(int i = 0; i < trainingData->getFeatureCount(); i++){
                std::string feature = trainingData->getFeatureAt(i).first;
                if(feature == target)
                    continue;

                delta = error * converter->toDouble(trainingData->getEntryFeatureAt(j, i));
                this->weights[feature] -= this->learningRate * delta;
            }
        }

        std::cout<<l2<<"\n";
    }
    // Convergence criteria
    while(abs(l2 - prev) > EPSILON);
}

double MLR::evaluate(std::map<std::string, std::string> query){
    double answer = this->bias;
    for(auto it = query.begin(); it != query.end(); it++){
        if(it->first == target)
            continue;
        answer += weights[it->first] * converter->toDouble(query[it->first]);
    }
    return answer;
}

double MLR::test(DataSet* testData){
    double error = 0;
    int N = testData->getEntryCount();

    for(int i = 0; i < N; i++){
        std::map<std::string, std::string> query = *(testData->makeQuery(i));
        error += (evaluate(query) - converter->toDouble(query[target]))/N;
    }
    return error;
}

void MLR::print(){
    for(auto it = weights.begin(); it != weights.end(); it++){
        std::cout<<it->first<<" "<<it->second<<"\n";
    }
}
