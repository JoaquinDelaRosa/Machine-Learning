#include <bits/stdc++.h>
#include <Dataset/DataSet.h>

#include <DecisionTree.h>
#include <Outputs/Validator.h>
#include <Pruner.h>
#include <Printer.h>
#include <Dataset/DataParser.h>

// TIme taken: approx. 24 hrs

int main()
{
    Validator* validator = new Validator();
    Printer* printer = new Printer();
    DataParser* parser = new DataParser();
    DataSet* d = parser->parseData("trainingdata.txt", "features.txt");
    DataSet* t = parser->parseData("testdata.txt", "features.txt");
    DataSet* v = parser->parseData("validationdata.txt", "features.txt");
    Pruner* pruner = new Pruner();

    DecisionTree* decisionTree = new DecisionTree(d, "GENDER");
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

    std::cout<<"\nEstimated Error Rate (By k-fold validation): "<<validator->kFoldCrossValidation(v, decisionTree, 10)<<"\n";

    return 0;
}
