#ifndef VALIDATOR_H
#define VALIDATOR_H
#include "../DataSet/DataSet.h"
#include "Model.h"

class Validator
{
    public:
        Validator();
        double kFoldCrossValidation(DataSet* d, Model* m, int k);
        DataSet* generateTestingData(DataSet* d, int k, int index);
};

#endif // VALIDATOR_H
