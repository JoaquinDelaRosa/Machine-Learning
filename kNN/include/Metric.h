#ifndef METRIC_H
#define METRIC_H

#include <bits/stdc++.h>

class Metric
{
    public:
        Metric();

        double euclidean(std::string a, std::string b);
        double levenshtein(std::string a, std::string b);
        double squareEuclidean(std::string a, std::string b);
        double getDistance(std::map<std::string, std::string> a, std::map<std::string, std::string> b);
};

#endif // METRIC_H
