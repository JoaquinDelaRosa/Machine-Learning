#include "../include/DecisionTree.h"
#include "../../../Shared/include/DataSet/StatisticsManager.h"
#include "../../../Shared/include/Outputs/Model.h"
#include "bits/stdc++.h"
#include "../../../Shared/include/DataSet/Converter.h"

Converter* converter = new Converter();

DecisionTree::DecisionTree(DataSet* dataset, std::string targetFeature, DecisionTree* parent) : Model(){
    this->dataset = dataset;
    this->targetFeature = targetFeature;
    this->statistics = *(new StatisticsManager());
    this->predicate = *(new Predicate());
    this->children = *(new std::vector<DecisionTree*>());
    this->parent = parent;
}

DecisionTree::~DecisionTree(){
    delete this->dataset;
}

DecisionTree::DecisionTree(const DecisionTree& other){

    // Perform a recursive copy
    this->dataset = other.dataset;
    this->targetFeature = other.targetFeature;
    this->statistics = other.statistics;
    this->predicate = other.predicate;
    this->parent = other.parent;
    this->impurity = other.impurity;
    this->misclassificationRate = other.misclassificationRate;
    this->label = other.label;

    for(DecisionTree* child : other.children){
        if(child != nullptr){
            DecisionTree* newChild = new DecisionTree(*child);
            newChild->parent = this;
            this->children.push_back(newChild);
        }
    }
}

DecisionTree& DecisionTree::operator=(const DecisionTree& other){
    // Perform a recursive copy

    if(this == &other)
        return *this;

    this->dataset = other.dataset;
    this->targetFeature = other.targetFeature;
    this->statistics = other.statistics;
    this->predicate = other.predicate;
    this->parent = other.parent;
    this->impurity = other.impurity;
    this->misclassificationRate = other.misclassificationRate;
    this->label = other.label;

    for(DecisionTree* child : other.children){
        if(child != nullptr){
            DecisionTree* newChild = new DecisionTree(*child);
            newChild->parent = this;
            this->children.push_back(newChild);
        }
    }

    return *this;

}

std::pair<double, int> DecisionTree::getInformationGain(std::string feature, double h){
    double ig = 0;
    int best = -1;
    if(dataset->isFeatureCategorical(feature)){
        std::vector<DataSet*> p = this->dataset->partitionDataSet(feature);

        double rem = 0;
        for(DataSet* part : p ){
            double weight = (1.0* part->getEntryCount()) / (1.0 * dataset->getEntryCount());
            rem += weight * statistics.getEntropy(part, this->targetFeature);
        }
        ig = h - rem;
    }
    else{
        // Select the partition which will result in the highest gain
        dataset->sortByFeature(feature);
        int ct = dataset->getEntryCount();
        double rem = 0;
        int index = -1;
        double highestgain = -1;

        for(int i = 1; i < ct; i++){
            if(dataset->getEntryFeatureAt(i, targetFeature) != dataset->getEntryFeatureAt(i - 1, targetFeature)){
                //Select i
                rem = 0;
                std::vector<DataSet*> p = this->dataset->partitionDataSet(i);

                rem += ((1.0 * i ) / (1.0 *ct)) * statistics.getEntropy((p[0]), this->targetFeature) ;
                rem += ((1.0 * (ct - i )) / (1.0 *ct)) * statistics.getEntropy((p[1]), this->targetFeature) ;

                double temp = h - rem;
                if(temp > highestgain){
                    index = i;
                    highestgain = temp;
                }
            }
        }

        best = index;
        ig = highestgain;
    }
    return std::pair<double, int>(ig, best);
}

std::pair<double, int> DecisionTree::getWeightedVariance(std::string feature){
    double var = 0;
    int best = -1;

    if(dataset->isFeatureCategorical(feature)){
        std::vector<DataSet*> p = this->dataset->partitionDataSet(feature);
        for(DataSet* part: p){
            double weight = (1.0* part->getEntryCount()) / (1.0 * dataset->getEntryCount());
            double temp = statistics.getVariance(part, targetFeature);
            var += temp * weight;
        }
    }
    else{
        // Select the partition which will result in the highest gain
        dataset->sortByFeature(feature);
        int ct = dataset->getEntryCount();
        double rem = 0;
        int index = -1;
        double lowestgain = -1;

        for(int i = 1; i < ct; i++){

            if(dataset->getEntryFeatureAt(i, targetFeature) != dataset->getEntryFeatureAt(i - 1, targetFeature)){
                //Select i
                rem = 0;
                std::vector<DataSet*> p = this->dataset->partitionDataSet(i);

                rem += ((1.0 * i) / (1.0 *ct)) * statistics.getVariance((p[0]), this->targetFeature) ;
                rem += ((1.0 * (ct - i)) / (1.0 *ct)) * statistics.getVariance((p[1]), this->targetFeature) ;

                double temp = rem;
                if(lowestgain == -1 || temp < lowestgain){
                    index = i;
                    lowestgain = temp;
                }
            }
        }

        best = index;
        var = lowestgain;
    }

    return std::pair<double, int>(var, best);
}

std::pair<std::string, int> DecisionTree::findBestFeature(){

    double entropy = 0;

    if(dataset->isFeatureCategorical(targetFeature))
        entropy = this->statistics.getEntropy(this->dataset, this->targetFeature);
    else{
        entropy = this->statistics.getVariance(this->dataset, this->targetFeature);
    }

    std::string splittingFeature = "";
    double maximum = 0;
    double minimum = INT_MAX;
    int splittingIndex = -1;

    for(int i = 0; i < this->dataset->getFeatureCount(); i++){
        std::pair<std::string, int> feature = this->dataset->getFeatureAt(i);

        if(feature.first != targetFeature){
            std::pair<double, int> ig ;
            if(dataset->isFeatureCategorical(targetFeature)){
                ig = this->getInformationGain(feature.first, entropy);
                if(ig.first > maximum || i == 0){
                    splittingFeature = feature.first;
                    maximum = ig.first;
                    splittingIndex = ig.second;
                }
           }
           else{
                ig = this->getWeightedVariance(feature.first);
                if(ig.first < minimum || i == 0){
                    splittingFeature = feature.first;
                    minimum = ig.first;
                    splittingIndex = ig.second;
                }
           }
        }
    }
    return std::pair<std::string, int>(splittingFeature, splittingIndex);
}

bool DecisionTree::isDataSetEmpty(){
    return this->dataset->getEntryCount() == 0;
}

bool DecisionTree::isDataSetFeatureless(){
    return this->dataset->getFeatureCount() <= 1;
}

bool DecisionTree::isTargetUniform(){
    if(this->dataset->isFeatureCategorical(this->targetFeature)){
        std::string f = this->dataset->getEntryFeatureAt(0, this->targetFeature);
        for(int i = 0 ; i < this->dataset->getEntryCount(); i++){
            if(f != this->dataset->getEntryFeatureAt(i, this->targetFeature))
                return false;
        }

        return true;
    }
    return false;
}

std::string DecisionTree::getMajorityTargetFeature(){
    if(majority != "\b")
        return majority;

    if(dataset->isFeatureCategorical(targetFeature))
        majority = this->statistics.getMode(this->dataset, this->targetFeature);
    else
        majority = converter->toString(statistics.getMean(this->dataset, targetFeature));

    return majority;
}

bool DecisionTree::isPrunable(){
    if(this->parent == nullptr)
        return false;

    // Prune when 5% of the data remains.
    if(this->dataset->getEntryCount() < 3)
        return true;

    return false;
}

void DecisionTree::grow(){
    if(this->isDataSetEmpty()){
        if(this->parent == nullptr){
            return;
        }
        label = this->parent->getMajorityTargetFeature();
        return;
    }

    if(!this->dataset->isFeatureCategorical(targetFeature)){
        // Stop when the pruning condition is met.
        if(this->isPrunable()){
            label = this->getMajorityTargetFeature();
            return;
        }
    }
    else if(this->isTargetUniform()){
        // Make the predicate be equal to the value of the target feature.
        label = dataset->getEntryFeatureAt(0, targetFeature);
        return;
    }

    if(this->isDataSetFeatureless()){
        // Make the predicate be equal to the value of this node's majority feature
        label = this->getMajorityTargetFeature();
        return;
    }

    // Split on the best feature
    std::pair<std::string, int> best = this->findBestFeature();

    // Ensure that the best feature is actually a feature.
    if(best.first == ""){
        return;
    }

    std::vector<DataSet*> p;


    if(dataset->isFeatureCategorical(best.first))
        p = this->dataset->partitionDataSet(best.first);
    else if(best.second != -1){
        dataset->sortByFeature(best.first);
        p = this->dataset->partitionDataSet(best.second);
    }
    else
        return;


   // Make children and grow them
    for(DataSet* part : p){
        std::string predicateLabel;
        int m;

        if(dataset->isFeatureCategorical(best.first)){
            predicateLabel = part->getEntryFeatureAt(0, best.first);
            m = Predicate::Equals;
        }
        else{
            predicateLabel = std::to_string(
                (converter->toDouble(dataset->getEntryFeatureAt(best.second - 1, best.first)) +
                 converter->toDouble(dataset->getEntryFeatureAt(best.second, best.first))) / (2.0)
            );

            if(children.size() == 0)
                m = Predicate::Lesser;
            else
                m = Predicate::GreaterEq;
        }


         // Remove the feature if it is categorical
        if(dataset->isFeatureCategorical(best.first)){
            part->removeFeature(best.first);
        }


        DecisionTree* child = new DecisionTree(part, this->targetFeature, this);
        child->predicate = *(new Predicate(best.first, predicateLabel, m));
        this->children.push_back(child);
        child->grow();
    }
}

void DecisionTree::grow(DataSet* training){
    this->dataset = training;
    this->grow();
}

std::string DecisionTree::evaluate(std::map<std::string, std::string> query){
    if(!this->isTerminal()){
        for(DecisionTree* child : children){
            if(child == nullptr)
                continue;

            if(query.find(child->predicate.getFeature()) != query.end()){
                std::string fval = query[child->predicate.getFeature()];
                if(child->predicate.evaluate(fval, !dataset->isFeatureCategorical(fval)))
                    return child->evaluate(query);
            }
        }

        // Return the majority feature of this node if the answer was not found

        return this->getMajorityTargetFeature();
    }
    return this->label;;
}

double DecisionTree::test(DataSet* testData){
    // Use an evaluation function to determine whether or not to prune a subtree
    int wrong = 0;
    double error = 0;
    int total = testData->getEntryCount();
    for(int i = 0; i < total; i++){
        std::map<std::string, std::string>* query = testData->makeQuery(i);
        std::string evaluation = this->evaluate(*query);
        std::string testValue = testData->getEntryFeatureAt(i, targetFeature);
        // For categorical data
        if(testData->isFeatureCategorical(targetFeature) &&  testValue != evaluation){
            wrong++;
        }

        // For numerical data
        if(!testData->isFeatureCategorical(targetFeature)){
            double val = converter->toDouble(testValue);
            double e = converter->toDouble(evaluation);
            error += (val - e) * (val - e);
        }
        delete(query);
    }

    if(dataset->isFeatureCategorical(targetFeature))
        error = (1.0 * wrong) / total;

    return error;
}

std::vector<DecisionTree*> DecisionTree::getChildren(){
    return this->children;
}

DecisionTree* DecisionTree::getParent(){
    return this->parent;
}

void DecisionTree::removeChild(int index){
    children.erase(children.begin() + index);
}

void DecisionTree::addChild(int i, DecisionTree* child){
    children.insert(children.begin() + i, child);
}

void DecisionTree::removeChild(DecisionTree* child){
    for(int i = 0; i < (int) children.size(); i++){
        DecisionTree* ch = children[i];
        if(ch == child){
            this->removeChild(i);
        }
        if(ch != nullptr)
        ch->removeChild(child);
    }
}

int DecisionTree::getLeaves(){
    if(this->isTerminal())
        return 1;

    int sum = 0;
    for(DecisionTree* child : children){
        sum += child->getLeaves() + 1;
    }

    return sum ;
}

bool DecisionTree::isTerminal(){
    return label != "\b";
}

std::string DecisionTree::getTargetFeature(){
    return this->targetFeature;
}

double DecisionTree::getImpurity(){
    if(impurity >= 0)
        return impurity;

    // Calculate impurity using a specific strategy specified in the statistics manager. In this case we use Entropy, but we can use whatever
    // startegy we wish.

    if(dataset->isFeatureCategorical(targetFeature))
        impurity = statistics.getEntropy(dataset, targetFeature);
    else{
        impurity = statistics.getVariance(dataset, targetFeature);
    }

    return impurity;
}

double DecisionTree::getMisclassificationRate(){
    if (this->misclassificationRate >= 0)
        return this->misclassificationRate;

    // Let r be the misclassification estimate (equal to 1 - p(label))
    double r = 1 - ((1.0 * statistics.getFrequency(dataset, targetFeature, this->getMajorityTargetFeature())) / this->dataset->getEntryCount());
    // Let p be the posterior probability estimate. In this case we use N(t) / N, for N(t) being the size of this node;
    DecisionTree* root = this->getRoot();
    double p = (1.0 * this->dataset->getEntryCount()) / root->dataset->getEntryCount();

    this->misclassificationRate = r * p;
    return this->misclassificationRate;
}

DataSet* DecisionTree::getDataSet(){
    return this->dataset;
}

void DecisionTree::setPredicate(Predicate* p){
    this->predicate = *(new Predicate(*p));
}

std::string DecisionTree::getLabel(){
    return label;
}

Predicate DecisionTree::getPredicate(){
    return this->predicate;
}

DecisionTree* DecisionTree::getRoot(){
    DecisionTree* curr = this;
    while(curr->parent != nullptr)
        curr = curr->parent;

    return curr;
}

void DecisionTree::makeTerminal(){
    this->label = this->getMajorityTargetFeature();
    // Prune the children
    for(DecisionTree* child: children){
        this->removeChild(child);
    }
}
