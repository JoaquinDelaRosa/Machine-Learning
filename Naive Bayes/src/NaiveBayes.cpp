#include "NaiveBayes.h"
#include "../../../Shared/include/Outputs/Model.h"
#include "../../../Shared/include/DataSet/DataSet.h"
#include "../../../Shared/include/DataSet/StatisticsManager.h"
#include "bits/stdc++.h"

NaiveBayes::NaiveBayes(std::string target, int smoothing) : Model (){
    this->dataset = nullptr;
    this->conditionalprobabilities = *(new std::map<std::pair<std::string, std::string>, double>());
    this->targetprobabilities = *(new std::map<std::string , double>());
    this->target = target;
    this->smoothing = smoothing;
}

void NaiveBayes::grow(DataSet* trainingData){
    this->dataset = trainingData;
    updateProbabilities();
}

void NaiveBayes::updateProbabilities(){
    if(dataset == nullptr)
        return;

    int N = dataset->getEntryCount();
    int levels = 0;

    std::map<std::pair<std::string, std::string>, int> counters;
    std::map<std::string, int> targets;

    targetprobabilities.clear();
    conditionalprobabilities.clear();

    for(int i = 0; i <N; i++){
        // We assume all entries are categorical because naive bayes is strictly a classifier.
        for(int j = 0; j < dataset->getFeatureCount(); j++){
            std::string feature = dataset->getEntryFeatureAt(i, j);

            if(dataset->getFeatureAt(j).first == target){
                std::string t= dataset->getEntryFeatureAt(i, j);
                std::map<std::string, int>::iterator it = targets.find(feature);

                if(it != targets.end()){
                    it->second+=1;
                }
                else{
                    targets.emplace(std::pair<std::string, int>(feature, 1));
                    levels++;
                }
                continue;
            }

            std::string t = dataset->getEntryFeatureAt(i, target);
            std::pair<std::string, std::string> condition(feature, t);
            std::map<std::pair<std::string, std::string>, int>::iterator it = counters.find(condition);
            if(it != counters.end())
                it->second+=1;
            else
                counters.emplace(std::pair<std::pair<std::string, std::string>, int>(condition, smoothing + 1));
        }
    }

    // Place 1 target corresponding to nothing
    targets.emplace(std::pair<std::string, int>("", 1));
    levels++;

    // Place 1 event corresponding to nothing
    std::pair<std::string, std::string> emptyevent("", "");
    for(std::map<std::string, int>::iterator it = targets.begin(); it!= targets.end(); it++){
        // Add an empty event
        std::pair<std::string, std::string> p("", it->first);
        counters.emplace(std::pair<std::pair<std::string, std::string>, int>(p , smoothing));
        it->second += smoothing;

        double prob = (1.0 * it->second) / (N + (levels) * (smoothing));
        targetprobabilities.emplace(it->first, prob);
    }

    // Update conditional probabilities
    for(std::map<std::pair<std::string, std::string>, int>::iterator it = counters.begin(); it != counters.end(); it++){
        // This corresponds to P(A | B) = N(A and B) / N(B)
        double p =  (1.0 *it->second) / (targets[it->first.second]);
        conditionalprobabilities.emplace(it->first, p);
    }
}

std::string NaiveBayes::evaluate(std::map<std::string, std::string> query){
    std::string answer;
    double best = -1;

    for(std::map<std::string, double>::iterator it = targetprobabilities.begin(); it != targetprobabilities.end(); it++){
        std::string result = it->first;
        double p = 1;

        for(int i = 0; i < dataset->getFeatureCount(); i++){
            if(dataset->getFeatureAt(i).first == target)
                continue;
            std::string feature = query[dataset->getFeatureAt(i).first];
            std::pair<std::string, std::string> condition(feature, result);
            double fc = conditionalprobabilities[condition];
            p *= fc;
        }

        p *= targetprobabilities[result];

        if(p > best){
            answer = result;
            best = p;
        }

    }

    return answer;
}

double NaiveBayes::test(DataSet* testData){
    int total = testData->getEntryCount();
    int errors = 0;

    for(int i = 0; i < total; i++){
        std::map<std::string, std::string> query = *(testData->makeQuery(i));
        std::string result = evaluate(query);
        std::string expected = testData->getEntryFeatureAt(i, target);

        if(result != query[target])
            errors ++;
    }

    return (1.0 * errors) / total;
}

void NaiveBayes::print(){
    std::cout<<"\n==CONDITIONAL PROBABILITIES==\n";
    for(std::map<std::pair<std::string, std::string>, double>::iterator it = conditionalprobabilities.begin();
        it != conditionalprobabilities.end(); it++){
        std::cout<<"P("<<it->first.first<<"|"<<it->first.second<<") == "<<it->second<<"\n";
    }

    std::cout<<"\n==TARGETS==\n";
    for(std::map<std::string, double>::iterator it = targetprobabilities.begin(); it != targetprobabilities.end(); it++){
        std::cout<<"P("<<it->first<<") == "<<it->second<<"\n";
    }
}

