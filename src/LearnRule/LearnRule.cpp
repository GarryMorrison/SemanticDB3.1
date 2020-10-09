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

const std::string LearnRule::to_string() const {
    std::string rule_sym;
    switch (_rule_type) {
        case RULENORMAL : { rule_sym = " => "; break; }
        case RULEADD : { rule_sym = " +=> "; break; }
        case RULESEQ : { rule_sym = " .=> "; break; }
        case RULESTORED : { rule_sym = " #=> "; break; }
        case RULEMEMOIZE : { rule_sym = " !=> "; break; }
        default: rule_sym = " ?? ";
    }
    if (_ket_op->type() == OPEMPTY) {
        return ket_map.get_str(_first_op_idx) + _ket_like_seq->to_string() + rule_sym + _RHS_seq->to_string();
    }
    return ket_map.get_str(_first_op_idx) + " ( " + _ket_op->to_string() + _ket_like_seq->to_string() + " )" + rule_sym + _RHS_seq->to_string();
}

void LearnRule::Compile(ContextList &context) {
    return;
}
