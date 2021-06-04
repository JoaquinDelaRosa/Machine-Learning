#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "DataSet.h"
#include <bits/stdc++.h>
class DataParser
{
    public:
        DataParser();
        DataSet* parseData(std::string path, std::string features);
};

#endif // DATAPARSER_H
