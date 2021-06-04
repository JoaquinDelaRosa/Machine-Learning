#ifndef DATASET_H
#define DATASET_H

#include<bits/stdc++.h>

class DataSet
{
    private:
        std::vector<std::vector<std::string>> entries;
        std::vector<std::pair<std::string, int>> featureList;

    public:
        DataSet();
        DataSet(DataSet& other, bool featureOnly = false);

        enum Type{Categorical = 0, Numerical = 1};
        void addFeature(std::string name, DataSet::Type type);
        void addFeature(std::string name, int type);
        void addEntry(std::vector<std::string> entry);
        void removeEntry(int index);
        void removeFeature(std::string name);

        // getters
        std::vector<std::string> getEntryAt(int index);
        std::pair<std::string, int> getFeatureAt(int index);
        int getFeatureIndex(std::string feature);
        std::string getEntryFeatureAt(int index, int feature);
        std::string getEntryFeatureAt(int index, std::string feature);

        // Get counters
        int getEntryCount();
        int getFeatureCount();

        // Printing
        void printEntries();
        void printFeatures();

        bool isFeatureCategorical(std::string feature);

        // Sorting
        void sortByFeature(std::string feature);

        // Shuffling
        void shuffle();

        //Partitioning
        std::vector<DataSet*> partitionDataSet(int index);
        std::vector<DataSet*> partitionDataSet(std::string feature);

        // Querying
        std::map<std::string, std::string>* makeQuery(int index);
};

#endif // DATASET_H
