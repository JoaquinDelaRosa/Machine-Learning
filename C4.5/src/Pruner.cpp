#include "Pruner.h"
#include "DataSet.h"
#include "DecisionTree.h"
#include "StatisticsManager.h"
#include "Validator.h"
#include "Predicate.h"

Pruner::Pruner()
{
    this->statistics = *(new StatisticsManager());
}

void Pruner::reducedErrorPrune(DataSet* testData, DecisionTree* root, DecisionTree* current){
    if(current->getChildren().size() <= 0)
        return;

//     Assume the current subtree stays
    double included = statistics.getRE(root->test(testData), 0.2, testData->getEntryCount());
    double excluded = 0;


    for(int i = 0; i < (int) current->getChildren().size(); i++){
        reducedErrorPrune(testData, root, current->getChildren()[i]);
    }

    if(current->getParent() != nullptr){
        for(int i = 0; i < (int) current->getParent()->getChildren().size(); i++){
            DecisionTree* child = current->getParent()->getChildren()[i];
            if(child == current){
                current->getParent()->removeChild(i);
                excluded = statistics.getRE(root->test(testData), 0.2, testData->getEntryCount());
                current->getParent()->addChild(i, current);
                break;
            }
        }
    }

    // Prune if including the current node gives more error.
    if(excluded <= included){
        // Detach the subtree
        if(current->getParent() == nullptr)
            return;

        current->getParent()->removeChild(current);

        // Delete the subtreee
        delete current;
    }
}

DecisionTree* Pruner::reducedErrorPrune(DataSet* testData, DecisionTree* root){
    reducedErrorPrune(testData, root, root);
    return root;
}

void generateTreeSet(DecisionTree* root, DecisionTree* current, std::vector<DecisionTree*> *trees, std::vector<DecisionTree*> *rem){
    // Traverse the tree by DFS
    if(current->isTerminal())
        return;
    if(current == nullptr)
        return;

    // Traverse
    for(int i = 0; i < (int) current->getChildren().size(); i++){
        DecisionTree* child = current->getChildren()[i];
        generateTreeSet(root, child, trees, rem);

        if(child->isTerminal())
            continue;

        // Generate the subtree
        current->removeChild(child);

        DecisionTree* subtree = new DecisionTree(*root);
        DecisionTree* childcopy = new DecisionTree(*child);

        if(trees != nullptr)
            trees->push_back(subtree);
        if(rem != nullptr)
            rem->push_back(childcopy);
        current->addChild(i, child);
    }

}

DecisionTree* getRoot(DecisionTree* root){
    DecisionTree* subtree = new DecisionTree(*root);

    for(DecisionTree* child : subtree->getChildren()){
        subtree->removeChild(child);
    }

    return subtree;
}

void updateMisclassificationError(DecisionTree* root, DataSet* valData){
    for(DecisionTree* child : root->getChildren()){
        child->updateMisclassificationRate(valData);
        updateMisclassificationError(child, valData);
    }
}

DecisionTree* Pruner::costComplexityPrune(DataSet* testData, DataSet* validationData, DecisionTree* root){
    // Generate the list of misclassification errors
    std::vector<DecisionTree*> *T = new std::vector<DecisionTree*>();

    Validator * v= new Validator();
    updateMisclassificationError(root,validationData);

    return root;
}
