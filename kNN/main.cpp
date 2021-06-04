#include "../../Shared/include/DataSet/DataParser.h"
#include "../../Shared/include/DataSet/DataSet.h"
#include "../../Shared/include/Outputs/Validator.h"
#include <bits/stdc++.h>
#include <NearestNeighbor.h>
// To-do:
// Fix a bug which causes non-nodes to be selected.

// Time taken: approx 8 hrs

int main()
{
    DataParser* parser = new DataParser();
    NearestNeighbor* nn = new NearestNeighbor("ST", 1);
    Validator* val= new Validator();

    DataSet* d = parser->parseData("trainingdata.txt", "features.txt");
    DataSet* v = parser->parseData("validationdata.txt", "features.txt");

    d->printEntries();
    nn->grow(d);
    std::cout<<"Entries Retrieved: "<<d->getEntryCount()<<"\n";
    std::cout<<"Error Rate: "<<val->kFoldCrossValidation(v, nn, 2)<<"\n";

    //d->printEntries();

    return 0;
}
