//
// Created by Garry Morrison on 9/10/2020.
//

#include <iostream>
#include <memory>
#include "LearnRule.h"
#include "src/Operator/NumericOperator.h"
#include "MultiLearnRule.h"
#include "MultiSelfKet.h"


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


LearnRule::LearnRule(OperatorWithSequence &head, unsigned int rule_type, MultiLearnRule &tail)
        : _first_op_idx(0), _valid_learn_rule(false), _rule_type(rule_type) {

    if (head.op_vec.empty() || head.seq_vec.empty()) { return; }
    if ((head.op_vec[0]->type() != OPSIMPLE) && (head.op_vec[0]->type() != OPSEQUENCE)) { return; }

    _first_op_idx = head.op_vec[0]->get_head_op_idx();
    _ket_op = head.op_vec[0]->get_tail_op();
    _ket_like_seq = head.seq_vec[0];
    _RHS_seq = std::make_shared<MultiLearnRule>(tail);
    _valid_learn_rule = true;
}


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

Sequence LearnRule::Compile(ContextList &context) const {  // Maybe we should compile _RHS_seq before passing to context.learn() ??
    if (!_valid_learn_rule) { return Sequence(); }
    // std::cout << "LearnRule::Compile(context):" << std::endl;

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
            case RULENORMAL : { context.learn(_first_op_idx, k, _RHS_seq); break; }
            case RULEADD : { context.add_learn(_first_op_idx, k, _RHS_seq); break; }
            case RULESEQ : { context.seq_learn(_first_op_idx, k, _RHS_seq); break; }
            case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, _RHS_seq); break; }
            case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, _RHS_seq); break; }
            default: break;
        }
    }
    return _RHS_seq->to_seq();
}

Sequence LearnRule::Compile(ContextList &context, const Ket& label_ket) const {
    // std::cout << "LearnRule::Compile(context, label_ket)" << std::endl;
    return this->Compile(context); // TODO: fill out later!
}

Sequence LearnRule::Compile(ContextList &context, const Ket& label_ket1, const Ket& multi_label_ket) const {
    if (!_valid_learn_rule) { return Sequence(); }

    // std::cout << "LearnRule:" << std::endl;
    // std::cout << "    rule_type: " << _rule_type << std::endl;
    // std::cout << "    RHS_seq: " << _RHS_seq->to_string() << std::endl;
    // std::cout << "    RHS_seq.type: " << _RHS_seq->type() << std::endl;
    // std::cout << "    label_ket1: " << label_ket1.to_string() << std::endl;
    // std::cout << "    multi_label_ket: " << multi_label_ket.to_string() << std::endl;

    std::shared_ptr<BaseSequence> bSeq2 = _RHS_seq;

    if (_ket_op->type() == OPEMPTY) {
        if (_ket_like_seq->is_ket()) {  // Primary / most common branch
            // ulong label_idx = _ket_like_seq->to_ket().label_idx();    // Can we instead directly invoke .label_idx() ?
            // Ket label_ket = _ket_like_seq->to_ket();
            Ket label_ket = _ket_like_seq->Compile(context, label_ket1, multi_label_ket).to_ket();
            ulong label_idx = label_ket.label_idx();
            if (_rule_type == RULENORMAL || _rule_type == RULEADD || _rule_type == RULESEQ) {
                Sequence RHS = _RHS_seq->Compile(context, label_ket, multi_label_ket);
                bSeq2 = std::make_shared<Sequence>(RHS);
            }

            switch (_rule_type) {
                case RULENORMAL : { context.learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULEADD : { context.add_learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, bSeq2); break; }
                default: break;
            }
            return bSeq2->to_seq();
        } else {
            for (const auto k: _ket_like_seq->to_sp()) {  // Not sure this branch is ever triggered.
                ulong label_idx = k.label_idx();
                if (_rule_type == RULENORMAL || _rule_type == RULEADD || _rule_type == RULESEQ) {
                    Sequence RHS = _RHS_seq->Compile(context, k, multi_label_ket);
                    bSeq2 = std::make_shared<Sequence>(RHS);
                }

                switch (_rule_type) {
                    case RULENORMAL : { context.learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULEADD : { context.add_learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, bSeq2); break; }
                    default: break;
                }
            }
            return bSeq2->to_seq();
        }
    }
    // Superposition indirect_sp = _ket_op->Compile(context, _ket_like_seq->to_seq()).to_sp(); // A question remains, when should we compile this section?
    // Superposition indirect_sp = _ket_op->Compile(context, _ket_like_seq->to_seq(), label_ket1, multi_label_ket).to_sp();
    Sequence compiled_ket = _ket_like_seq->Compile(context, label_ket1, multi_label_ket);
    Superposition indirect_sp = _ket_op->Compile(context, compiled_ket).to_sp();
    // std::cout << "    indirect sp: " << indirect_sp.to_string() << std::endl;
    for (const auto k: indirect_sp) {
        ulong label_idx = k.label_idx();
        if (_rule_type == RULENORMAL || _rule_type == RULEADD || _rule_type == RULESEQ) {
            Sequence RHS = _RHS_seq->Compile(context, k, multi_label_ket);
            bSeq2 = std::make_shared<Sequence>(RHS);
        }

        switch (_rule_type) {
            case RULENORMAL : { context.learn(_first_op_idx, k, bSeq2); break; }
            case RULEADD : { context.add_learn(_first_op_idx, k, bSeq2); break; }
            case RULESEQ : { context.seq_learn(_first_op_idx, k, bSeq2); break; }
            case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, bSeq2); break; }
            case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, bSeq2); break; }
            default: break;
        }
    }
    return bSeq2->to_seq();
}

Sequence LearnRule::Compile(ContextList &context, const Ket& label_ket1, const std::vector<Sequence> &args) const {
    if (!_valid_learn_rule) { return Sequence(); }

    // std::cout << "LearnRule::Compile(context, label_ket1, args):" << std::endl;
    // std::cout << "    rule_type: " << _rule_type << std::endl;
    // std::cout << "    RHS_seq: " << _RHS_seq->to_string() << std::endl;
    // std::cout << "    RHS_seq.type: " << _RHS_seq->type() << std::endl;
    // std::cout << "    label_ket1: " << label_ket1.to_string() << std::endl;
    // std::cout << "    args: ... " << std::endl;

    std::shared_ptr<BaseSequence> bSeq2 = _RHS_seq;

    if (_ket_op->type() == OPEMPTY && (_ket_like_seq->type() != MULTISELFKET)) {  // MultiSelfKets can expand to Sequence's so we can't handle it on this branch.
        if (_ket_like_seq->is_ket()) {  // Primary / most common branch
            Ket label_ket = _ket_like_seq->to_ket();
            ulong label_idx = label_ket.label_idx();    // Can we instead directly invoke .label_idx() ?
            if (_rule_type == RULENORMAL || _rule_type == RULEADD || _rule_type == RULESEQ) {
                Sequence RHS = _RHS_seq->Compile(context, label_ket, args);
                bSeq2 = std::make_shared<Sequence>(RHS);
            }

            switch (_rule_type) {
                case RULENORMAL : { context.learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULEADD : { context.add_learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, bSeq2); break; }
                case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, bSeq2); break; }
                default: break;
            }
            return bSeq2->to_seq();
        } else {
            for (const auto k: _ket_like_seq->to_sp()) {  // Not sure this branch is ever triggered.
                ulong label_idx = k.label_idx();
                if (_rule_type == RULENORMAL || _rule_type == RULEADD || _rule_type == RULESEQ) {
                    Sequence RHS = _RHS_seq->Compile(context, k, args);
                    bSeq2 = std::make_shared<Sequence>(RHS);
                }

                switch (_rule_type) {
                    case RULENORMAL : { context.learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULEADD : { context.add_learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULESEQ : { context.seq_learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, bSeq2); break; }
                    case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, bSeq2); break; }
                    default: break;
                }
            }
            return bSeq2->to_seq();
        }
    }
    // Superposition indirect_sp = _ket_op->Compile(context, _ket_like_seq->to_seq()).to_sp(); // A question remains, when should we compile this section?
    Sequence compiled_ket = _ket_like_seq->Compile(context, label_ket1, args);
    Superposition indirect_sp = _ket_op->Compile(context, compiled_ket).to_sp();
    // std::cout << "    indirect sp: " << indirect_sp.to_string() << std::endl;
    for (const auto k: indirect_sp) {
        ulong label_idx = k.label_idx();
        if (_rule_type == RULENORMAL || _rule_type == RULEADD || _rule_type == RULESEQ) {
            Sequence RHS = _RHS_seq->Compile(context, k, args);
            bSeq2 = std::make_shared<Sequence>(RHS);
        }

        switch (_rule_type) {
            case RULENORMAL : { context.learn(_first_op_idx, k, bSeq2); break; }
            case RULEADD : { context.add_learn(_first_op_idx, k, bSeq2); break; }
            case RULESEQ : { context.seq_learn(_first_op_idx, k, bSeq2); break; }
            case RULESTORED : { context.stored_learn(_first_op_idx, label_idx, bSeq2); break; }
            case RULEMEMOIZE : { context.memoize_learn(_first_op_idx, label_idx, bSeq2); break; }
            default: break;
        }
    }
    return bSeq2->to_seq();
}

