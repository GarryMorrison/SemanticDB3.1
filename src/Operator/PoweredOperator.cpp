//
// Created by Garry Morrison on 13/09/2020.
//

#include "PoweredOperator.h"

Sequence PoweredOperator::Compile(NewContext& context, const Sequence& seq) const {
    Sequence result(seq);
    for (unsigned int i = 0; i < pow_; i++) {
        result = b_op->Compile(context, result);  // Memory leak??
    }
    return result;
}

Sequence PoweredOperator::Compile(ContextList& context, const Sequence& seq) const {
    Sequence result(seq);
    for (unsigned int i = 0; i < pow_; i++) {
        result = b_op->Compile(context, result);  // Memory leak??
    }
    return result;
}

Sequence PoweredOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    Sequence result(seq);
    for (unsigned int i = 0; i < pow_; i++) {
        result = b_op->Compile(context, result, label_idx);  // Memory leak??
    }
    return result;
}


const std::string PoweredOperator::to_string() const {
    std::string s;
    s = b_op->to_string() + "^" + std::to_string(pow_);
    return s;
}
