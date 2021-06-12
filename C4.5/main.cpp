#include <bits/stdc++.h>
#include <DecisionTree.h>
#include "../../Shared/include/DataSet/DataSet.h"
#include "../../Shared/include/DataSet/DataParser.h"
#include "../../Shared/include/Outputs/Validator.h"
#include <Pruner.h>
#include <Printer.h>

// TIme taken: approx. 24 hrs
// Version 1.3

int main()
{
    Validator* validator = new Validator();
    Printer* printer = new Printer();
    DataParser* parser = new DataParser();
    DataSet* d = parser->parseData("trainingdata.txt", "features.txt");
    DataSet* t = parser->parseData("testdata.txt", "features.txt");
    DataSet* v = parser->parseData("validationdata.txt", "features.txt");
    Pruner* pruner = new Pruner();

    DecisionTree* decisionTree = new DecisionTree(d, "D");
    decisionTree->grow();
    printer->printTree(decisionTree);

    std::cout<<"\nError rate: "<<decisionTree->test(v)<<"\n";
    std::cout<<"Test Samples: "<<v->getEntryCount()<<"\n";
    std::cout<<"Training Samples: "<<d->getEntryCount()<<"\n";
    std::cout<<"Pruning Samples: "<<t->getEntryCount()<<"\n";

    std::cout<<"\n=====PRUNED=====\n";
    decisionTree = pruner->costComplexityPrune(t, decisionTree);
    printer->printTree(decisionTree);

    std::cout<<"\nError rate: "<<decisionTree->test(v)<<"\n";
    std::cout<<"Test Samples: "<<v->getEntryCount()<<"\n";
    std::cout<<"Training Samples: "<<d->getEntryCount()<<"\n";

    std::cout<<"Estimated Error Rate (By k-fold validation): "<<validator->kFoldCrossValidation(v, decisionTree, 10)<<"\n";

    return 0;
}
