#include "Metric.h"
#include "bits/stdc++.h"

Metric::Metric()
{
    //ctor
}

double Metric::euclidean(double a, double b){
    return sqrt(a * a + b * b);
}

double Metric::levenshtein(std::string a, std::string b){
    int table[a.size()][b.size()];

    for(int i = 0; i < (int) a.size(); i++){
        table[i][0] = i;
    }

    for(int i = 0; i < (int) b.size(); i++){
        table[0][i] = i;
    }


    for(int i = 1; i < (int) a.size(); i++){
        for(int j = 1; j < (int) b.size(); j++){
            int cost = (a[i] == b[j]) ? 0 : 1;

            table[i][j] = std::min(std::min(table[i - 1][j] + 1,
                                            table[i][j - 1] + 1),
                                            table[i - 1][j - 1] + cost);
        }
    }


    return table[a.size() - 1][b.size() -1];
}
