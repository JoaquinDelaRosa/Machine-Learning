#include "../../include/Outputs/Validator.h"
#include "../../include/DataSet/DataSet.h"
#include "../../include/Outputs/Model.h"

Validator::Validator()
{

}

double Validator::kFoldCrossValidation(DataSet* d, Model* m, int k){
    if(k == 0)
        return 0;
    if(m == nullptr || d == nullptr)
        return 0;

    // shuffle the data set and get samples
    d->shuffle();

    double cerror = 0;

    for(int i = 0; i < k; i ++){
        // Make training and testing data
        DataSet* testing = generateTestingData(d, k, i);
        if(testing == nullptr)
            continue;

        double error = m->test(testing);
        cerror += error;

        delete testing;
    }


    return cerror / k;;
}

DataSet* Validator::generateTestingData(DataSet* d, int k, int index){
    if(k == 0)
        return nullptr;
    if(index >= k || index < 0)
        return nullptr;

    DataSet* testing = new DataSet(*d, true);
    int N = d->getEntryCount();
    int samples = N / k;

    for(int j = 0; j < N; j++){
        if(! (index * (samples) <= j && j < (index + 1) *(samples))){
            testing->addEntry(d->getEntryAt(j));
        }
    }

    return testing;

}
