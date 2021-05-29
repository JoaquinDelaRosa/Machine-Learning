#include "Dataset/StatisticsManager.h"
#include "Dataset/DataSet.h"
#include "bits/stdc++.h"

#define SQRT1_2 sqrt(1.0/2)
#define PI 3.1415
#define INV_ERR_CONST 0.15449436008930206298828125

StatisticsManager::StatisticsManager()
{
    //ctor
}

std::string StatisticsManager::getMode(DataSet* d, std::string feature){
    std::map<std::string, int> counters;

    for(int i = 0; i < d->getEntryCount(); i++){
        std::string f = d->getEntryFeatureAt(i, feature);
        std::map<std::string, int>::iterator it = counters.find(f);
        if(it != counters.end())
            it->second+=1;
        else if(f != "")
            counters.emplace(std::pair<std::string, int>(f, 1));
    }

    // Return the mode by iterating over map
    int maximum = counters.begin()->second;
    std::string mode = "";

    for(auto it = counters.begin(); it != counters.end(); ++it){
        if(it->second >= maximum){
            mode = it->first;
            maximum = it->second;
        }
    }

    return mode;
}

int StatisticsManager::getFrequency(DataSet* d, std::string feature, std::string label){
    int ctr = 0;
    for(int i = 0; i < d->getEntryCount(); i++){
        if(d->getEntryFeatureAt(i, feature) == label)
            ctr++;
    }

    return ctr;
}

double StatisticsManager::getEntropy(DataSet* d, std::string target){
    std::map<std::string, int> counters;
    int total = d->getEntryCount();

    for(int i = 0; i < d->getEntryCount(); i++){
        std::string f = d->getEntryFeatureAt(i, target);
        std::map<std::string, int>::iterator it = counters.find(f);
        if(it != counters.end())
            it->second+=1;
        else if(f != "")
            counters.emplace(std::pair<std::string, int>(f, 1));
    }

    // Compute entropy
    double entropy = 0;
    for(auto it = counters.begin(); it != counters.end(); ++it){
        double p = (1.0* it->second) / (1.0 * total);
        entropy += p * log2(p);
    }

    return -1 * entropy;
}

double StatisticsManager::getMean(DataSet* d, std::string feature){
    double mean = 0;
    if(d->isFeatureCategorical(feature))
        return 0;

    int total = d->getEntryCount();
    double sum = 0;
    for(int i = 0; i < total; i++){
        sum += stod(d->getEntryFeatureAt(i, feature));
    }

    mean = sum / (1.0* total);
    return mean;
}

double StatisticsManager::getVariance(DataSet* d, std::string feature){
    double var = 0;
    if(d->isFeatureCategorical(feature))
        return 0;

    int total = d->getEntryCount();
    double sum = 0;
    double mean = this->getMean(d, feature);

    for(int i = 0; i < total; i ++){
        double val = stod(d->getEntryFeatureAt(i, feature));

        sum += (val - mean) * (val - mean);
    }

    var = sum / (total - 1);
    return var;
}

double StatisticsManager::getRE(double error, double alpha, int N){
    double z = getERFInv(1 - 2 * alpha) / SQRT1_2;
    double wt = (1.0 + (z * z) / N);
    double f = error;

    double f1 = f;
    double f2 = (z * z) / (2 * N);
    double f3 = z * sqrt(f / N - (f * f) / N + (z * z) / (4 * N * N));

    double p = (f1 + f2 + f3) / wt;
    return p;
}

double StatisticsManager::getERFInv(double c){
    // Given the value of alpha in the z-table, this function returns the corresponding z-value of the one-sided z-test.
   double tt1, tt2, lnx, sgn;
   sgn = (c < 0) ? -1.0f : 1.0f;

   c = (1 - c)*(1 + c);        // x = 1 - x*x;
   lnx = logf(c);

   tt1 = 2/(PI*INV_ERR_CONST) + 0.5f * lnx;
   tt2 = 1/(INV_ERR_CONST) * lnx;

   return(sgn*sqrtf(-tt1 + sqrtf(tt1*tt1 - tt2)));
}
