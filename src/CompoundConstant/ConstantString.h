//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_CONSTANTSTRING_H
#define SDB3_1_CONSTANTSTRING_H

#include <string>
#include "CompoundConstant.h"
#include "../Operator/SimpleOperator.h"


class ConstantString : public CompoundConstant {
private:
    const std::string str;
public:
    ConstantString(const std::string &s) : str(s) { }
    const int type() const { return CSTRING; }
    const double get_float() const {return 0.0; }
    const int get_int() const { return 0; }
    const std::string get_string() const { return str; };
    const SimpleOperator get_operator() const { return SimpleOperator(""); } // Maybe change later.
    const std::string to_string() const { return "\"" + str + "\""; }

    void append(const std::shared_ptr<CompoundConstant> compound_constant) { return; }
};


#endif //SDB3_1_CONSTANTSTRING_H
