#ifndef PREDICATE_H
#define PREDICATE_H

#include<bits/stdc++.h>

class Predicate
{
    private:
        std::string feature;
        std::string value;
        int mode;

    public:
        Predicate(std::string feature, std::string value, int mode);
        Predicate();
        enum Mode{Equals = 0, Greater  = 1, Lesser  = 2, GreaterEq = 3, LesserEq = 4};
        void print();
        bool evaluate(std::string value, bool numerical);

        std::string getValue();
        std::string getFeature();

};

#endif // PREDICATE_H
