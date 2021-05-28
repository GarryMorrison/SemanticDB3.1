//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_COMPOUNDCONSTANT_H
#define SDB3_1_COMPOUNDCONSTANT_H

#include <string>
#include <memory>
// #include "../Operator/SimpleOperator.h"
class SimpleOperator;

#define CSTRING 30
#define CFLOAT 31
#define CINT 32
#define COPERATOR 33


class CompoundConstant {
private:

public:
    virtual const int type() const = 0;
    virtual const std::string to_string() const = 0;
    virtual const double get_float() const = 0;
    virtual const int get_int() const = 0;
    virtual const SimpleOperator get_operator() const = 0; // should this be SimpleOperator or BaseOperator*?
    virtual const std::string get_string() const = 0;
    virtual void append(const std::shared_ptr<CompoundConstant> compound_constant) = 0;
};


#endif //SDB3_1_COMPOUNDCONSTANT_H
