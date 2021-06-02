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

std::string naiveMedian(DataSet* d, std::string feature){
    // To optimize this we should implement median of medians
    if(d->getEntryCount() == 0){
        return "";
    }

    DataSet* s = new DataSet(*d);
    s->sortByFeature(feature);
    int lm  = (floor((d->getEntryCount() + 1) / 2.0 ) - 1.0);
    int um  = (ceil((d->getEntryCount() + 1) / 2.0 ) - 1.0);

    if(s->isFeatureCategorical(feature)){
        return s->getEntryFeatureAt(lm , feature);
    }
    double x = stod(s->getEntryFeatureAt(lm, feature));
    double y = stod(s->getEntryFeatureAt(um, feature));


    double median = (x + y) / 2;
    return std::to_string(median);
}


std::string StatisticsManager::getNth(DataSet* d, std::string feature, unsigned long long int n){
    // Partition the data set into sets of 5
    std::vector<DataSet*> subsets;
    int index = 0;
    int N = d->getEntryCount();
    int M = 5;

    // Feature
    std::pair<std::string, int> f = d->getFeatureAt(d->getFeatureIndex(feature));

    // Partition into subsets
    while(index < N){
        DataSet *div = new DataSet();
        div->addFeature(f.first, f.second);

        for(int i = 0; i <  M&& index < N; i++){
            std::vector<std::string> s;
            s.push_back(d->getEntryFeatureAt(index, feature));
            div->addEntry(s);
            index++;
        }

        subsets.push_back(div);
    }

    // Get the medians on each subset
    // Temp acts as a buffer.
    std::vector<std::string> temp;
    DataSet* median = new DataSet();
    median->addFeature(f.first, f.second);

    for(DataSet* subs: subsets){
        temp.push_back(naiveMedian(subs, feature));
        median->addEntry(temp);
        temp.clear();
    }

    std::string pivot = "";
    if(median->getEntryCount() <= M){
        pivot = naiveMedian(median, feature);
    }
    else{
        pivot = getNth(median, feature, median->getEntryCount() / 2);
    }


    // Partition
    DataSet* low = new DataSet(*median, true);
    DataSet* high = new DataSet(*median, true);

    for(int i = 0; i < d->getEntryCount(); i++){
        std::string entryFeature = d->getEntryFeatureAt(i, feature);
        temp.push_back(entryFeature);

        if(d->isFeatureCategorical(feature)){
            if(entryFeature < pivot){
                low->addEntry(temp);
            }
            else{
                high->addEntry(temp);
            }
        }
        else{
            if(stod(entryFeature) < stod(pivot)){
                low->addEntry(temp);
            }
            else{
                high->addEntry(temp);
            }
        }

        temp.clear();
    }
    unsigned long long int k = low->getEntryCount();
    if(k == n)
        return pivot;
    else if(k < n){
        return this->getNth(high, feature, n - k -1);
    }
    else{
        return this->getNth(low, feature, n);
    }
}

std::string StatisticsManager::getMedian(DataSet* d, std::string feature){
    return naiveMedian(d, feature);
    //return this->getNth(s, feature, s->getEntryCount() / 2);
}
