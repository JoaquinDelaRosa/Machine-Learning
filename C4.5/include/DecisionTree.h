#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include <../../../Shared/include/DataSet/DataSet.h>
#include <../../../Shared/include/DataSet/StatisticsManager.h>
#include <bits/stdc++.h>
#include<Predicate.h>
#include <../../../Shared/include/Outputs/Model.h>

class DecisionTree : public Model
{
    // Implement the C4.5 Algorithm
    private:
        DataSet* dataset;
        std::string targetFeature;
        DecisionTree* parent;
        Predicate predicate;
        StatisticsManager statistics;

        std::vector<DecisionTree*> children;

        std::pair<std::string, int> findBestFeature();


        std::pair<double, int> getInformationGain(std::string feature, double h);
        std::pair<double, int>getWeightedVariance(std::string feature);

        // Base cases
        bool isTargetUniform();
        bool isDataSetEmpty();
        bool isDataSetFeatureless();
        bool isPrunable();

        std::string majority = "\b";
        std::string label = "\b";
        double impurity = -1;
        double misclassificationRate = -1;

    public:
        DecisionTree(DataSet* dataset, std::string targetFeature, DecisionTree* parent = nullptr);
        ~DecisionTree();
        DecisionTree(const DecisionTree& other);
        DecisionTree& operator= (const DecisionTree& other);

        void grow();
        void grow(DataSet* training) override;

        // Evaluation
        std::string evaluate(std::map<std::string, std::string> query);
        std::string getMajorityTargetFeature();
        double test(DataSet *testdata) override;

        // Child addition and removal
        void addChild(int i, DecisionTree* child);
        void removeChild(int index);
        void removeChild(DecisionTree* child);


        // Getters and Setters
        int getWeight();
        int getLeaves();
        DataSet* getDataSet();
        bool isTerminal();
        std::string getTargetFeature();

        double getImpurity();
        double getMisclassificationRate();
        Predicate getPredicate();
        std::string getLabel();
        void updateMisclassificationRate(DataSet* d);
        void setPredicate(Predicate* p);

        std::vector<DecisionTree*> getChildren();
        DecisionTree* getParent();
        DecisionTree* getRoot();

        void makeTerminal();
};

#endif // DECISIONTREE_H
