#include <iostream>
#include <DataSet.h>
#include <bits/stdc++.h>
#include <DecisionTree.h>
#include <Predicate.h>
#include <Validator.h>
#include <Pruner.h>
#include <Printer.h>

// TIme taken: approx. 24 hrs

void addFeatures(DataSet* d){
    d->addFeature("CT", DataSet::Categorical);
    d->addFeature("ST", DataSet::Categorical);
    d->addFeature("G", DataSet::Categorical);
    d->addFeature("DECISION", DataSet::Categorical);
}

void constructDataSet(DataSet* d, std::string filename){
    std::ifstream trainingData;
    trainingData.open(filename);
    if(trainingData.is_open()){
        std::string temp;
        while(std::getline(trainingData, temp)){
            // Delimit data.
            std::istringstream iss(temp);
            std::string token;
            std::vector<std::string> entry;
            while(std::getline(iss, token, '\t')){
                entry.push_back(token);
            }
            d->addEntry(entry);
        }

        trainingData.close();
    }
    else{
        std::cout<<"FILE NOT FOUND.";
    }

}
int main()
{
    Validator* validator = new Validator();
    Printer* printer = new Printer();
    DataSet* d = new DataSet();
    DataSet* t = new DataSet();
    DataSet* v = new DataSet();
    Pruner* pruner = new Pruner();

    addFeatures(d);
    addFeatures(t);
    addFeatures(v);
    constructDataSet(d, "trainingdata.txt");
    constructDataSet(t, "testdata.txt");
    constructDataSet(v, "validationdata.txt");
    // Begin processing the data here.

    DecisionTree* decisionTree = new DecisionTree(d, "DECISION");
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
