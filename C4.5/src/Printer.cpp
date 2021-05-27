#include "Printer.h"
#include "DecisionTree.h"

Printer::Printer()
{
    //ctor
}

void Printer::printTree(DecisionTree* root){
    if(!root->isTerminal()){
        for(DecisionTree* child: root->getChildren()){
            if(child != nullptr)
                printTree(child);
        }
    }
    else{
        std::cout<<root->getLabel()<<" :";
        DecisionTree* curr = root->getParent();
        root->getPredicate().print();
        while(curr != nullptr){
            curr->getPredicate().print();
            curr = curr->getParent();
        }

        std::cout<<"\n";
    }
}

void Printer::printFullTree(DecisionTree* root, DecisionTree* current){
    if(!current->isTerminal()){
        for(DecisionTree* child: current->getChildren()){
            if(child != nullptr)
                printFullTree(root, child);
        }

        // Print the exception
        if(current != root){
            std::cout<<current->getMajorityTargetFeature()<<": ";
            std::cout<<current->getPredicate().getFeature()<<" == ??? ";
            DecisionTree* curr = current->getParent();
            while(curr != nullptr){
                curr->getPredicate().print();
                curr = curr->getParent();
            }
            std::cout<<"\n";
        }
    }
    else{
        std::cout<<current->getLabel()<<" :";
        DecisionTree* curr = current->getParent();
        current->getPredicate().print();
        while(curr != nullptr){
            curr->getPredicate().print();
            curr = curr->getParent();
        }

        std::cout<<"\n";
    }
}

void Printer::printDataSet(DecisionTree* tree){
    tree->getDataSet()->printFeatures();
    tree->getDataSet()->printEntries();
}
