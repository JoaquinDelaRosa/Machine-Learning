#include "Pruner.h"
#include "Dataset/DataSet.h"
#include "DecisionTree.h"
#include "Dataset/StatisticsManager.h"
#include "Outputs/Validator.h"

#define INFINITESIMAL 1e-15

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

void updateMisclassificationError(DecisionTree* root){
    for(DecisionTree* child : root->getChildren()){
        child->getMisclassificationRate();
        updateMisclassificationError(child);
    }
}

void getMinimumTree(DecisionTree* tree){
    if(tree->isTerminal())
        return;

    for(DecisionTree* child : tree->getChildren())
        getMinimumTree(child);

    double error = 0;
    for(DecisionTree* child : tree->getChildren()){
        error += child->getMisclassificationRate();
    }

    // Since we are comparing doubles we just set it to be below some threshold to account
    // for floating point errors.

    if(abs(error - tree->getMisclassificationRate()) <= INFINITESIMAL){
        // Don't prune if the root node is all that is left.
        if(tree->getParent() != nullptr)
            tree->makeTerminal();
        else if(tree->getParent() == nullptr && tree->getChildren().size())
            tree = nullptr;
    }
}

std::pair<DecisionTree*, double> pruneWeakestLink(DecisionTree* tree){
    // Generate all subtrees
    if(tree == nullptr)
        return std::pair<DecisionTree*, double> (nullptr, -1);

    std::vector<DecisionTree*> *subtrees = new std::vector<DecisionTree*>();
    std::vector<DecisionTree*> *pruneset = new std::vector<DecisionTree*>();

    generateTreeSet(tree, tree, pruneset, subtrees);
    double best = 0;
    int index = -1;

    for(int i = 0 ; i < (int) pruneset->size(); i++){
        DecisionTree* s = (*subtrees)[i];

        // Get branch misclassification error:
        double rt = 0;
        for(DecisionTree* child : s->getChildren()){
            rt += child->getMisclassificationRate();
        }

        double r = s->getMisclassificationRate();
        double alpha = (r - rt) / (s->getLeaves() - 1);
        if(index == -1 || best > alpha){
            best = alpha;
            index = i;
        }
    }

    if(index == -1)
        return std::pair<DecisionTree*, double> (nullptr, -1);

    DecisionTree* choice = new DecisionTree(*((*pruneset)[index]));

    delete subtrees;
    delete pruneset;

    return std::pair<DecisionTree*, double>(choice , best);
}

DecisionTree* Pruner::costComplexityPrune(DataSet* testData, DecisionTree* root){
    // Generate the list of misclassification errors
    std::vector<DecisionTree*> *T = new std::vector<DecisionTree*>();
    std::vector<double> alpha;

    Validator * v= new Validator();
    updateMisclassificationError(root);

    // Initialize t1
    DecisionTree* tree = new DecisionTree(*root);
    getMinimumTree(tree);
    alpha.push_back(0);
    T->push_back(tree);

    while(tree != nullptr){
        // Prune the weakest link
        std::pair<DecisionTree*, double> result = pruneWeakestLink(tree);
        tree = result.first;

        if(tree == nullptr)
            break;

        T->push_back(tree);
        alpha.push_back(result.second);
    }

    // Now we select the desired alpha and the desired tree.
    DecisionTree* choice = nullptr;
    double bestError = 1;

    for(int i = 0; i < (int) T->size() - 1; i++){
        double threshold = sqrt(alpha[i] * alpha[i + 1]);

        // Find tree below this threshold
        int index = 0;
        while(alpha[index] < threshold && index < (int) T->size() - 1)
            index++;

        index = std::max(index -1 , 0);

        DecisionTree* candidate = (*T)[index];

        // Perform 10-fold validation
        double error = v->kFoldCrossValidation(testData, candidate, 10);
        error = error + candidate->getLeaves() * alpha[index];
        if(error < bestError || choice == nullptr){
            bestError = error;
            choice = candidate;
        }
    }

    return choice;
}
