#ifndef METRIC_H
#define METRIC_H

#include <bits/stdc++.h>

class Metric
{
    public:
        Metric();

        double euclidean(double a, double b);
        double levenshtein(std::string a, std::string b);
};

#endif // METRIC_H
