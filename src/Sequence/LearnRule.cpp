//
// Created by Garry Morrison on 9/10/2020.
//

#include <memory>
#include "LearnRule.h"
#include "src/Operator/NumericOperator.h"

LearnRule::LearnRule(OperatorWithSequence &head, unsigned int rule_type, OperatorWithSequence &tail)  // Tidy this function!!!
    : _first_op_idx(0), _valid_learn_rule(false), _rule_type(rule_type) {

    if (head.op_vec.empty() || head.seq_vec.empty()) { return; }
    if ((head.op_vec[0]->type() != OPSIMPLE) && (head.op_vec[0]->type() != OPSEQUENCE)) { return; }

    _first_op_idx = head.op_vec[0]->get_head_op_idx();
    _ket_op = head.op_vec[0]->get_tail_op();
    _ket_like_seq = head.seq_vec[0];
    _RHS_seq = std::make_shared<OperatorWithSequence>(tail);
    _valid_learn_rule = true;
}

/*
LearnRule::LearnRule(class OperatorWithSequence &head, unsigned int rule_type, class MultiLearnRule &tail)
        : _first_op_idx(0), _valid_learn_rule(false), _rule_type(rule_type) {

    if (head.op_vec.empty() || head.seq_vec.empty()) { return; }
    if ((head.op_vec[0]->type() != OPSIMPLE) && (head.op_vec[0]->type() != OPSEQUENCE)) { return; }

    _first_op_idx = head.op_vec[0]->get_head_op_idx();
    _ket_op = head.op_vec[0]->get_tail_op();
    _ket_like_seq = head.seq_vec[0];
    _RHS_seq = std::make_shared<MultiLearnRule>(tail);
    _valid_learn_rule = true;
}
*/

const std::string LearnRule::to_string() const {
    if (!_valid_learn_rule) { return ""; }
    std::string rule_sym;
    switch (_rule_type) {
        case RULENORMAL : { rule_sym = " =>"; break; }
        case RULEADD : { rule_sym = " +=>"; break; }
        case RULESEQ : { rule_sym = " .=>"; break; }
        case RULESTORED : { rule_sym = " #=>"; break; }
        case RULEMEMOIZE : { rule_sym = " !=>"; break; }
        default: rule_sym = " ?? ";
    }
    if (_ket_op->type() == OPEMPTY) {
        return ket_map.get_str(_first_op_idx) + _ket_like_seq->to_string() + rule_sym + _RHS_seq->to_string();
    }
    return ket_map.get_str(_first_op_idx) + " ( " + _ket_op->to_string() + _ket_like_seq->to_string() + " )" + rule_sym + _RHS_seq->to_string();
}

Sequence LearnRule::Compile(NewContext &context) const {  // TODO: these two are almost certainly deprecated.
    return Sequence();
}

Sequence LearnRule::Compile(NewContext &context, const ulong label_idx) const {
    return Sequence();
}

Sequence LearnRule::Compile(ContextList &context) const {
    if (!_valid_learn_rule) { return Sequence(); }
    if (_ket_op->type() == OPEMPTY) {
        if (_ket_like_seq->is_ket()) {  // Primary / most common branch
            ulong label_idx = _ket_like_seq->to_ket().label_idx();    // Can we instead directly invoke .label_idx() ?
            switch (_rule_type) {
                case RULENORMAL : { context.learn(_first_op_idx, label_idx, _RHS_seq); break; }
                case RULEADD : { context.add_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                default: break;
            }
            return _RHS_seq->to_seq();
        } else {
            for (const auto k: _ket_like_seq->to_sp()) {  // Not sure this branch is ever triggered.
                ulong label_idx = k.label_idx();
                switch (_rule_type) {
                    case RULENORMAL : { context.learn(_first_op_idx, label_idx, _RHS_seq); break; }
                    case RULEADD : { context.add_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                    case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                    case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                    case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, _RHS_seq); break; }
                    default: break;
                }
            }
            return _RHS_seq->to_seq();
        }
    }
    Superposition indirect_sp = _ket_op->Compile(context, _ket_like_seq->to_seq()).to_sp(); // A question remains, when should we compile this section?
    for (const auto k: indirect_sp) {
        ulong label_idx = k.label_idx();
        switch (_rule_type) {
            case RULENORMAL : { context.learn(_first_op_idx, label_idx, _RHS_seq); break; }
            case RULEADD : { context.add_learn(_first_op_idx, label_idx, _RHS_seq); break; }
            case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, _RHS_seq); break; }
            case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, _RHS_seq); break; }
            case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, _RHS_seq); break; }
            default: break;
        }
    }
    return _RHS_seq->to_seq();
}

Sequence LearnRule::Compile(ContextList &context, const ulong label_idx) const {
    return this->Compile(context); // TODO: fill out later!
}

Sequence LearnRule::Compile(ContextList &context, const std::vector<Sequence> &args) const {
    return this->Compile(context);  // TODO: fill out later!
}

