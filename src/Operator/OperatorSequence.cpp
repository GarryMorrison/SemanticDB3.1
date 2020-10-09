//
// Created by Garry Morrison on 13/09/2020.
//

#include <memory>
#include <string>
#include "OperatorSequence.h"
#include "SimpleOperator.h"
#include "EmptyOperator.h"

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

OperatorSequence::OperatorSequence(std::shared_ptr<BaseOperator> b_op) : op_symbol(SPLUS), head_op(b_op) {}

OperatorSequence::OperatorSequence(unsigned int type, std::shared_ptr<BaseOperator> b_op) : op_symbol(type), head_op(b_op) {}

void OperatorSequence::append(std::shared_ptr<BaseOperator> b_op) {
    tail_op_seq.push_back(b_op);
}

Sequence OperatorSequence::Compile(NewContext& context, const Sequence& seq) const {
    Sequence result(seq);
    for (auto it = tail_op_seq.rbegin(); it != tail_op_seq.rend(); ++it) {
        result = (*it)->Compile(context, result);
    }
    result = head_op->Compile(context, result);
    if (op_symbol == SMINUS) { result.multiply(-1); } // not 100% sure this should be here.
    return result;
}

Sequence OperatorSequence::Compile(ContextList& context, const Sequence& seq) const {
    Sequence result(seq);
    for (auto it = tail_op_seq.rbegin(); it != tail_op_seq.rend(); ++it) {
        result = (*it)->Compile(context, result);
    }
    result = head_op->Compile(context, result);
    if (op_symbol == SMINUS) { result.multiply(-1); } // not 100% sure this should be here.
    return result;
}

Sequence OperatorSequence::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    Sequence result(seq);
    for (auto it = tail_op_seq.rbegin(); it != tail_op_seq.rend(); ++it) {
        result = (*it)->Compile(context, result, label_idx);
    }
    result = head_op->Compile(context, result, label_idx);
    if (op_symbol == SMINUS) { result.multiply(-1); } // not 100% sure this should be here.
    return result;
}

const ulong OperatorSequence::get_head_op_idx() const {
    if (head_op->type() == OPSIMPLE) { return head_op->get_head_op_idx(); }
    return 0;
}

const std::shared_ptr<BaseOperator>  OperatorSequence::get_tail_op() const {
    if (tail_op_seq.empty()) { return std::make_shared<EmptyOperator>(); }
    if (tail_op_seq.size() == 1) { return tail_op_seq[0]; }
    OperatorSequence result(tail_op_seq[0]);
    for (auto it = tail_op_seq.cbegin() + 1; it != tail_op_seq.cend(); ++it) {
        result.append(*it);
    }
    return std::make_shared<OperatorSequence>(result);  // use std::move here?
}

const std::string OperatorSequence::to_string() const {
    std::string s;
    switch(op_symbol) {  // disable op_symbol later!
        case SPLUS: s = " + "; break;
        case SMINUS: s = " - "; break;
        case SSEQ: s = " . "; break;
        case SMERGE: s = " _ "; break;
        case SMERGE2: s = " __ "; break;
    }
    s = head_op->to_string();
    for (const auto &op: tail_op_seq ) {
        s += " " + op->to_string();
    }
    return s;
}
