#include "Dataset/DataParser.h"
#include "Dataset/DataSet.h"
#include "bits/stdc++.h"


DataParser::DataParser()
{
    //ctor
}

void addFeatures(DataSet* d, std::string features){
    std::ifstream file;
    file.open(features);

    if(file.is_open()){
        std::string temp;
        while(std::getline(file, temp)){
            // Delimit data.
            std::istringstream iss(temp);
            std::string token;
            std::vector<std::string> entry;

            while(std::getline(iss, token, '\t')){
                entry.push_back(token);
            }

            if(entry.size() < 2)
                continue;

            DataSet::Type type = DataSet::Categorical;
            if(entry[1] == "NUMERICAL"){
                type = DataSet::Numerical;
            }

            d->addFeature(entry[0], type);
        }

        file.close();
    }
    else{
        std::cout<<"FILE NOT FOUND.";
    }
}

DataSet* DataParser::parseData(std::string path, std::string features){
    std::ifstream file;
    file.open(path);
    DataSet* d =new DataSet();

    addFeatures(d, features);

    if(file.is_open()){
        std::string temp;
        while(std::getline(file, temp)){
            // Delimit data.
            std::istringstream iss(temp);
            std::string token;
            std::vector<std::string> entry;

            while(std::getline(iss, token, '\t')){
                entry.push_back(token);
            }

            d->addEntry(entry);
        }

        file.close();
    }
    else{
        std::cout<<"FILE NOT FOUND.";
    }

    return d;
}
