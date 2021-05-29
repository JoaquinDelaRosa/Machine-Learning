#ifndef MODEL_H
#define MODEL_H
#include <Dataset/DataSet.h>

class Model
{
    public:
        Model();
        virtual~Model();
        virtual void grow(DataSet* trainingdata);        // Grows the model
        virtual double test(DataSet* testdata);      // Testing function returns the error rate
};

#endif // MODEL_H
