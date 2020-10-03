//
// Created by Garry Morrison on 13/09/2020.
//

#include <memory>
#include <string>
#include "OperatorSequence.h"

/*
OperatorSequence::OperatorSequence(BaseOperator* b_op) {
    op_symbol = SPLUS;
    op_seq.push_back(b_op);
}

OperatorSequence::OperatorSequence(unsigned int type, BaseOperator* b_op) {
    op_symbol = type;
    op_seq.push_back(b_op);
}
*/

OperatorSequence::OperatorSequence(std::shared_ptr<BaseOperator> b_op) {
    op_symbol = SPLUS;
    op_seq.push_back(b_op);
}

OperatorSequence::OperatorSequence(unsigned int type, std::shared_ptr<BaseOperator> b_op) {
    op_symbol = type;
    op_seq.push_back(b_op);
}


void OperatorSequence::append(std::shared_ptr<BaseOperator> b_op) {
    op_seq.push_back(b_op);
}

Sequence OperatorSequence::Compile(NewContext& context, const Sequence& seq) const {
    Sequence result(seq);
    for (auto it = op_seq.rbegin(); it != op_seq.rend(); ++it) {
        result = (*it)->Compile(context, result);
    }
    if (op_symbol == SMINUS) { result.multiply(-1); } // not 100% sure this should be here.
    return result;
}

Sequence OperatorSequence::Compile(ContextList& context, const Sequence& seq) const {
    Sequence result(seq);
    for (auto it = op_seq.rbegin(); it != op_seq.rend(); ++it) {
        result = (*it)->Compile(context, result);
    }
    if (op_symbol == SMINUS) { result.multiply(-1); } // not 100% sure this should be here.
    return result;
}

Sequence OperatorSequence::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    Sequence result(seq);
    for (auto it = op_seq.rbegin(); it != op_seq.rend(); ++it) {
        result = (*it)->Compile(context, result, label_idx);
    }
    if (op_symbol == SMINUS) { result.multiply(-1); } // not 100% sure this should be here.
    return result;
}

const std::string OperatorSequence::to_string() const {
    std::string s;
    switch(op_symbol) {
        case SPLUS: s = " + "; break;
        case SMINUS: s = " - "; break;
        case SSEQ: s = " . "; break;
        case SMERGE: s = " _ "; break;
        case SMERGE2: s = " __ "; break;
    }
    bool first_pass = true;
    for (const auto op: op_seq ) {
        if (first_pass) {
            s += op->to_string();
            first_pass = false;
        } else {
            s += " " + op->to_string();
        }
    }
    return s;
}
