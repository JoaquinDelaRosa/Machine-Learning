#include "../../Shared/include/DataSet/DataParser.h"
#include "../../Shared/include/DataSet/DataSet.h"
#include "../../Shared/include/Outputs/Validator.h"
#include <bits/stdc++.h>
#include <NaiveBayes.h>
// To-do:
// Fix a bug which causes non-nodes to be selected.

// Time taken: approx 8 hrs

int main()
{
    DataParser* parser = new DataParser();
    Validator* val= new Validator();
    NaiveBayes* bayes = new NaiveBayes("NAME");

    DataSet* d = parser->parseData("trainingdata.txt", "features.txt");
    DataSet* v = parser->parseData("testdata.txt", "features.txt");

    std::cout<<"Entries Retrieved: "<<d->getEntryCount()<<"\n";

    v->printEntries();

    bayes->grow(d);

    std::cout<<"Error Rate: "<<val->kFoldCrossValidation(v, bayes, 2)<<"\n";

    //d->printEntries();

    return 0;
}
