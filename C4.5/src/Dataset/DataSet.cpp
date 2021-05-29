#include "Dataset/DataSet.h"
#include<bits/stdc++.h>

DataSet::DataSet()
{
    this->entries = *(new std::vector<std::vector<std::string>>());
}
DataSet::DataSet(DataSet& other, bool featuresOnly ){
    if(!featuresOnly)
        this->entries = other.entries;

    this->featureList = other.featureList;
}

void DataSet::addEntry(std::vector<std::string> entry){
    if(entry.size() != this->featureList.size())
        return;
    this->entries.push_back(entry);
}

void DataSet::addFeature(std::string name, DataSet::Type type){
    this->featureList.push_back(std::pair<std::string, int> (name, (int) type));
}


void DataSet::printEntries(){
    for(std::vector<std::string> entry : this->entries){
        for(std::string field : entry){
            std::cout<<field<<"\t";
        }
        std::cout<<"\n";
    }
}

void DataSet::printFeatures(){
    for(auto& x: this->featureList ){
        std::cout<<x.first<<"\t";
    }
    std::cout<<"\n";
}

std::vector<std::string> DataSet::getEntryAt(int index){
    if(index >= 0 && index < (int) this->entries.size())
        return this->entries[index];

    return *(new std::vector<std::string>());
}

std::pair<std::string, int> DataSet::getFeatureAt(int index){
    if(index >= 0 && index < (int) this->featureList.size()){
        return this->featureList[index];
    }

    return *(new std::pair<std::string, int>("",-1));
}

std::string DataSet::getEntryFeatureAt(int index, int feature){
    if(index >= 0 && index < (int) this->entries.size()){
        if(feature >= 0 && feature < (int) this->featureList.size())
            return this->entries[index][feature];
    }

    return "";
}

std::string DataSet::getEntryFeatureAt(int index, std::string feature){
    int findex = this->getFeatureIndex(feature);

    if(index >= 0 && index < (int) this->entries.size()){
        if(findex >= 0 && findex < (int) this->featureList.size())
            return this->entries[index][findex];
    }

    return "";
}

void DataSet::removeEntry(int index){
    if(index >= 0 && index < (int) this->entries.size())
        this->entries.erase(this->entries.begin() + index);
    else
        std::cout<<"Entry cannot be deleted.\n";
}

void DataSet::removeFeature(std::string name){
    int index = 0;
    for(index = 0; index < (int) this->featureList.size(); index++){
        if(this->featureList[index].first == name)
            break;
    }
    if(index >=  (int) this->featureList.size())
        return;

    this->featureList.erase(this->featureList.begin() + index);
    // Remove feature from entries
    for(int i = 0; i < (int) this->entries.size(); i++){
        this->entries[i].erase(this->entries[i].begin() + index);
    }
}

int DataSet::getEntryCount(){
    return this->entries.size();
}

int DataSet::getFeatureCount(){
    return this->featureList.size();
}

bool DataSet::isFeatureCategorical(std::string feature){
    for(std::pair<std::string, int> f : this->featureList){
        if(f.first == feature){
            return f.second == this->Type::Categorical;
        }
    }
    return true;
}

int DataSet::getFeatureIndex(std::string feature){
    int index = -1;

    for(int i = 0; i < (int) this->featureList.size(); i++){
        if(featureList[i].first == feature){
            index = i;
            break;
        }
    }

    return index;
}

void DataSet::sortByFeature(std::string feature){
    int index = this->getFeatureIndex(feature);
    bool categorical = this->isFeatureCategorical(feature);
    if(index == -1){
        return;
    }

    std::sort(this->entries.begin(), this->entries.end(),
              [&index, &categorical](const std::vector<std::string>& a, const std::vector<std::string>& b){
                if(categorical)
                    return a[index] < b[index];
                return stod(a[index]) < stod(b[index]);
                }
              );
}


void DataSet::shuffle(){
    // Generate a new seed for shuffling.
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    std::shuffle(this->entries.begin(), this->entries.end(), e);
}

std::vector<DataSet*> DataSet::partitionDataSet(std::string feature){
    std::vector<DataSet*> subsets;
    std::map<std::string, int> levels;
    int levelctr = 0;

    for(int i = 0; i < this->getEntryCount(); i++){
        std::string f = this->getEntryFeatureAt(i, feature);

        auto itr = levels.find(f);
        // If a new label was not found, add it to the existing label
        if(itr != levels.end()){
            subsets[itr->second]->addEntry(this->getEntryAt(i));
        }

        // Otherwise make a new vector for that label.
        else{
            levels.emplace(std::pair<std::string, int>(f, levelctr));
            subsets.push_back(new DataSet(*this, true));
            subsets[levelctr]->addEntry(this->getEntryAt(i));
            levelctr++;
        }
    }
    return subsets;
}

std::vector<DataSet*> DataSet::partitionDataSet(int index){
    std::vector<DataSet*> subsets;
    subsets.push_back(new DataSet(*this, true));
    subsets.push_back(new DataSet(*this, true));

    for(int i = 0; i < index; i++)
        subsets[0]->addEntry(this->getEntryAt(i));

    for(int i = index; i < this->getEntryCount(); i++)
        subsets[1]->addEntry(this->getEntryAt(i));


    return subsets;
}

std::map<std::string, std::string>* DataSet::makeQuery(int index){
    std::map<std::string, std::string> *query = new std::map<std::string, std::string>();
    for(int i = 0; i < this->getFeatureCount(); i++){
        std::pair<std::string, int> feature = this->getFeatureAt(i);
        query->emplace(feature.first, this->getEntryFeatureAt(index, i));
    }

    return query;
}
