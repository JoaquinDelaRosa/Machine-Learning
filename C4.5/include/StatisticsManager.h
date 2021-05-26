#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H
#include <DataSet.h>
#include <bits/stdc++.h>

class StatisticsManager
{
    public:
        StatisticsManager();


        std::string getMode(DataSet* d, std::string feature, int start, int finish);
        double getEntropy(DataSet* d, std::string target);
        double getMean(DataSet* d, std::string feature);
        double getVariance(DataSet* d, std::string feature);

        double getRE(double error, double alpha, int N);
        double getERFInv(double confidence);

};

#endif // STATISTICSMANAGER_H
