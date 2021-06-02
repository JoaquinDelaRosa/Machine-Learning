#include "Outputs/Predicate.h"
#include "bits/stdc++.h"

Predicate::Predicate(){
    this->feature = "";
    this->value = "";
    this->mode = -1;
}

Predicate::Predicate(std::string feature, std::string value, int mode)
{
    this->feature = feature;
    this->value = value;
    this->mode = mode;
}

void Predicate::print(){
    std::string operation = "";
    switch(this->mode){
        case Equals: operation = "=="; break;
        case Greater: operation = ">"; break;
        case Lesser: operation = "<"; break;
        case GreaterEq: operation = ">="; break;
        case LesserEq: operation = "<="; break;
        default: operation = ""; break;
    }

    std::cout<<this->feature<<" "<<operation<<" "<<this->value<<" ";
}

bool Predicate::evaluate(std::string value, bool numeric){
    if(numeric){
        switch(this->mode){
            case Equals: return value == this->value;
            case Greater:  return value > this->value;
            case Lesser:  return value < this->value;
            case GreaterEq:  return value >= this->value;
            case LesserEq: return value <= this->value;
            default: return false;
        }
    }
    else{
        return value == this->value;
    }
}

std::string Predicate::getValue(){
    return this->value;
}

std::string Predicate::getFeature(){
    return this->feature;
}
