#include "../../include/DataSet/Converter.h"
#include "bits/stdc++.h"

Converter::Converter()
{

}

std::string Converter::toString(double d){
    std::ostringstream strs;
    strs << d;
    return strs.str();
}

double Converter::toDouble(std::string s){
    s = "0" + s;
    return stod(s);
}
