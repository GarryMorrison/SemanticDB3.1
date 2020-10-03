//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_NUMERICOPERATOR_H
#define SDB3_1_NUMERICOPERATOR_H

#include <string>
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "BaseOperator.h"


class NumericOperator : public BaseOperator {
private:
    double value;

public:
    NumericOperator(const double d) { value = d; };
    Sequence Compile(NewContext& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const std::vector<Sequence>& args) const;
    const std::string to_string() const;
};


#endif //SDB3_1_NUMERICOPERATOR_H
