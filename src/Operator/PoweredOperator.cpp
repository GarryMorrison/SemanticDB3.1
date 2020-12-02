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

Sequence PoweredOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket) const {
    Sequence result(seq);
    for (unsigned int i = 0; i < pow_; i++) {
        result = b_op->Compile(context, result, label_ket);  // Memory leak??
    }
    return result;
}

Sequence PoweredOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket, const Ket& multi_label_ket) const {
    Sequence result(seq);
    for (unsigned int i = 0; i < pow_; i++) {
        result = b_op->Compile(context, result, label_ket, multi_label_ket);
    }
    return result;
}

Sequence PoweredOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket, const std::vector<Sequence> &args) const {
    Sequence result(seq);
    for (unsigned int i = 0; i < pow_; i++) {
        result = b_op->Compile(context, result, label_ket, args);
    }
    return result;
}

const std::string PoweredOperator::to_string() const {
    std::string s;
    s = b_op->to_string() + "^" + std::to_string(pow_);
    return s;
}
