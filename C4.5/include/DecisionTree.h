#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <DataSet.h>
#include <StatisticsManager.h>
#include <bits/stdc++.h>
#include <Predicate.h>
#include <Model.h>

class DecisionTree : public Model
{
    // Implement the C4.5 Algorithm
    private:
        DataSet* dataset;
        std::string targetFeature;
        DecisionTree* parent;
        Predicate predicate;
        StatisticsManager statistics;

        std::vector<DataSet*> partitionDataSet(std::string feature);
        std::vector<DataSet*> partitionDataSet(int index);

        std::pair<std::string, int> findBestFeature();

        std::vector<DecisionTree*> children;

        std::pair<double, int> getInformationGain(std::string feature, double h);
        std::pair<double, int>getWeightedVariance(std::string feature);
        std::map<std::string, std::string>* makeTestData(int index, DataSet* dataset);

        // Base cases
        bool isTargetUniform();
        bool isDataSetEmpty();
        bool isDataSetFeatureless();
        bool isPrunable();


        std::string majority = "\b";

    public:
        DecisionTree(DataSet* dataset, std::string targetFeature, DecisionTree* parent = nullptr);
        ~DecisionTree();
        DecisionTree(const DecisionTree& other);
        DecisionTree& operator= (const DecisionTree& other);

        void printDataSet();
        void grow();
        void grow(DataSet* training) override;

        void printTree();
        void printFullTree();
        std::string evaluate(std::map<std::string, std::string> query);
        std::string getMajorityTargetFeature();

        double test(DataSet *testdata) override;

        std::vector<DecisionTree*> getChildren();
        DecisionTree* getParent();

        void addChild(int i, DecisionTree* child);
        void removeChild(int index);
        void removeChild(DecisionTree* child);

        int getWeight();
        int getLeaves();
        bool isTerminal();
        std::string getTargetFeature();
};

#endif // DECISIONTREE_H
