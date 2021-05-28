//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_CONSTANTOPERATOR_H
#define SDB3_1_CONSTANTOPERATOR_H

#include <string>
#include "CompoundConstant.h"
#include "../KetMap/KetMap.h"
#include "../Operator/SimpleOperator.h"

class ConstantOperator : public CompoundConstant {
private:
    ulong idx;
public:
    ConstantOperator(const ulong idx2) : idx(idx2) {}
    ConstantOperator(const std::string &s) { idx = ket_map.get_idx(s); }
    const int type() const { return COPERATOR; }
    const double get_float() const {return 0.0; }
    const int get_int() const { return 0; }
    const SimpleOperator get_operator() const { return SimpleOperator(idx); }
    const std::string get_string() const { return ""; }  // Maybe change later.
    const std::string to_string() const { return ket_map.get_str(idx); }

    void append(const std::shared_ptr<CompoundConstant> compound_constant) { return; }
};


#endif //SDB3_1_CONSTANTOPERATOR_H
