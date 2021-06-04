#ifndef PRINTER_H
#define PRINTER_H

#include "DecisionTree.h"

class Printer
{
    public:
        Printer();
        void printTree(DecisionTree* root);
        void printFullTree(DecisionTree* root, DecisionTree* current);
        void printDataSet(DecisionTree* tree);
};

#endif // PRINTER_H
