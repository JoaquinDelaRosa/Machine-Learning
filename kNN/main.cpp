#include <bits/stdc++.h>
#include <Dataset/DataParser.h>
#include <Dataset/DataSet.h>
#include <NearestNeighbor.h>
#include <Outputs/Validator.h>

#include <Dataset/StatisticsManager.h>

// To-do:
// Implement searching for k-Nearest Neighbors, rather than just 1 neighbor.
// Implement weighted neighbor search.

// Time taken: approx 4 hrs

int main()
{
    DataParser* parser = new DataParser();
    NearestNeighbor* nn = new NearestNeighbor("ST");
    Validator* val= new Validator();

    DataSet* d = parser->parseData("trainingdata.txt", "features.txt");
    DataSet* t = parser->parseData("testdata.txt", "features.txt");
    DataSet* v = parser->parseData("validationdata.txt", "features.txt");

    //d->printEntries();
    nn->grow(d);
    std::cout<<"Entries Retrieved: "<<d->getEntryCount()<<"\n";
    std::cout<<"Error Rate: "<<val->kFoldCrossValidation(v, nn, 10)<<"\n";

    //d->printEntries();

    return 0;
}
