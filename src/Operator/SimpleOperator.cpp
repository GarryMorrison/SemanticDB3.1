//
// Created by Garry Morrison on 13/09/2020.
//

#include <iostream>
#include "SimpleOperator.h"
#include "../Sequence/BaseSequence.h"
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "../FunctionOperatorMap/FunctionOperatorMap.h"


Sequence SimpleOperator::Compile(NewContext& context, const Sequence& seq) const {  // Really should delete this function!
    if (op_idx == ket_map.get_idx("")) { return seq; }  // Change later?
    if (fn_map.sigmoids.find(op_idx) != fn_map.sigmoids.end()) {
        auto sigmoid = fn_map.sigmoids[op_idx];
        return seq.apply_sigmoid(sigmoid);
    } else if (fn_map.built_in.find(op_idx) != fn_map.built_in.end()) {
        auto fn = fn_map.built_in[op_idx];
        return fn(&seq);
    } else if (fn_map.ket_fn.find(op_idx) != fn_map.ket_fn.end()) {
        auto fn = fn_map.ket_fn[op_idx];
        return seq.apply_ket_fn(fn);
    } else if (fn_map.sp_fn.find(op_idx) != fn_map.sp_fn.end()) {
        auto fn = fn_map.sp_fn[op_idx];
        return seq.apply_sp_fn(fn);
    }

    Sequence result;
    for (const auto &sp: seq) {
        Sequence seq2;
        for (const auto &k: sp) {
            //BaseSequence *b_rule = context.active_recall(op_idx, k.label_idx()); // Swap in later, when we have storedrules and memoizing rules
            std::shared_ptr<BaseSequence> b_rule = context.recall(op_idx, k.label_idx());
            // std::shared_ptr<BaseSequence> b_rule = context.active_recall(op_idx, k.label_idx());
            // std::cout << "b_rule: " << b_rule->to_string() << std::endl;
            // std::cout << "b_rule type: " << b_rule->type() << std::endl;
            Sequence seq3 = b_rule->to_seq();  // Need to test these three lines are right ...
            // seq3.multiply(k.value());
            seq2.add(seq3);
        }
        result.append(seq2);
    }
    return result;
}

Sequence SimpleOperator::Compile(ContextList& context, const Sequence& seq) const {
    if (op_idx == ket_map.get_idx("")) { return seq; }  // Change later?

    // std::cout << "SimpleOperator::Compile(context, seq):" << std::endl;  // Later, if in debug mode, print this out.
    // std::cout << "    seq: " << seq.to_string() << std::endl;
    // std::cout << "    seq.size(): " << seq.size() << std::endl;
    // std::cout << "    seq.type(): " << seq.type() << std::endl;

    if (fn_map.sigmoids.find(op_idx) != fn_map.sigmoids.end()) {
        auto sigmoid = fn_map.sigmoids[op_idx];
        return seq.apply_sigmoid(sigmoid);
    } else if (fn_map.built_in.find(op_idx) != fn_map.built_in.end()) {
        // std::cout << "    built_in_fn: " << ket_map.get_str(op_idx) << std::endl;
        auto fn = fn_map.built_in[op_idx];
        return fn(&seq);
    } else if (fn_map.ket_fn.find(op_idx) != fn_map.ket_fn.end()) {
        auto fn = fn_map.ket_fn[op_idx];
        return seq.apply_ket_fn(fn);
    } else if (fn_map.context_ket_fn.find(op_idx) != fn_map.context_ket_fn.end()) {
        auto fn = fn_map.context_ket_fn[op_idx];
        return seq.apply_context_ket_fn(fn, context);
    } else if (fn_map.sp_fn.find(op_idx) != fn_map.sp_fn.end()) {
        auto fn = fn_map.sp_fn[op_idx];
        return seq.apply_sp_fn(fn);
    } else if (fn_map.seq_fn.find(op_idx) != fn_map.seq_fn.end()) {
        auto fn = fn_map.seq_fn[op_idx];
        return fn(seq);  // Is this correct? Do we instead need seq.apply_seq_fn(fn)?
    } else if (fn_map.context_seq_fn.find(op_idx) != fn_map.context_seq_fn.end()) {
        auto fn = fn_map.context_seq_fn[op_idx];
        return fn(seq, context);
    } else if (context.fn_recall_type(op_idx, 1) == RULESTORED) {  // What about other rule types?
        auto rule = context.fn_recall(op_idx, 1);
        std::vector<Sequence> args; // specify size of args here?
        // Sequence empty("");
        Sequence empty;
        args.push_back(empty);
        args.push_back(seq);
        // Ket empty_ket("");
        Ket empty_ket;
        return rule->Compile(context, empty_ket, args);  // Currently set label_idx to 0. Not sure what else we can do.
    } else if (context.bound_fn_recall_type(op_idx, 1) == RULESTORED) {
        ulong input_idx = ket_map.get_idx("input");  // learn input|seq>. A little heavier and slower than I would like!
        ulong seq_idx = ket_map.get_idx("seq");      // Also, we have hard wired in the op/ket labels here. Is there a better way?
        unsigned int input_rule_type = context.recall_type(input_idx, seq_idx);
        std::shared_ptr<BaseSequence> input_bSeq;
        if (input_rule_type != RULEUNDEFINED) {
            input_bSeq = context.recall(input_idx, seq_idx);
        }
        std::shared_ptr<BaseSequence> bSeq_empty = std::make_shared<Ket>();
        context.learn(input_idx, seq_idx, bSeq_empty);
        std::vector<ulong> params = context.bound_fn_params_recall(op_idx, 1);
        ulong param_op_idx = params[0];
        ulong param_ket_idx = params[1];
        unsigned int param_rule_type = context.recall_type(param_op_idx, param_ket_idx);
        std::shared_ptr<BaseSequence> param_bSeq = context.recall(param_op_idx, param_ket_idx);
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(seq);
        context.learn(param_op_idx, param_ket_idx, bSeq);
        auto rule = context.bound_fn_body_recall(op_idx, 1);
        std::vector<Sequence> args; // specify size of args here?
        Sequence empty;
        args.push_back(empty);
        args.push_back(seq);
        Ket empty_ket;
        // return rule->Compile(context, empty_ket, args);
        Sequence result = rule->Compile(context, empty_ket, args);

        // Now restore the variables:
        switch (input_rule_type) {
            case RULENORMAL: context.learn(input_idx, seq_idx, input_bSeq); break;
            case RULESTORED: context.stored_learn(input_idx, seq_idx, input_bSeq); break;
            case RULEMEMOIZE: context.memoize_learn(input_idx, seq_idx, input_bSeq); break;
            case RULEUNDEFINED: context.unlearn(input_idx, seq_idx); break;
            default: break;
        }
        switch (param_rule_type) {
            case RULENORMAL: context.learn(param_op_idx, param_ket_idx, param_bSeq); break;
            case RULESTORED: context.stored_learn(param_op_idx, param_ket_idx, param_bSeq); break;
            case RULEMEMOIZE: context.memoize_learn(param_op_idx, param_ket_idx, param_bSeq); break;
            case RULEUNDEFINED: context.unlearn(param_op_idx, param_ket_idx); break;
            default: break;
        }
        return result;
    }

    Sequence result;
    if (seq.size() == 0) {
        ulong empty_idx = ket_map.get_idx("");
        return context.active_recall(op_idx, empty_idx);
    }
    for (const auto &sp: seq) {
        Sequence seq2;
        if (sp.size() == 0) {
            ulong empty_idx = ket_map.get_idx("");
            seq2 = context.active_recall(op_idx, empty_idx);
            // std::cout << "seq2: " << seq2.to_string() << std::endl;
        } else {
            for (const auto &k: sp) {
                //BaseSequence *b_rule = context.active_recall(op_idx, k.label_idx()); // Swap in later, when we have storedrules and memoizing rules
                // std::shared_ptr<BaseSequence> b_rule = context.recall(op_idx, k.label_idx());
                // std::shared_ptr<BaseSequence> b_rule = context.active_recall(op_idx, k.label_idx());
                // std::cout << "b_rule: " << b_rule->to_string() << std::endl;
                // std::cout << "b_rule type: " << b_rule->type() << std::endl;
                // Sequence seq3 = b_rule->to_seq();  // Need to test these three lines are right ...
                Sequence seq3 = context.active_recall(op_idx, k);
                seq2.add(seq3);
            }
        }
        result.append(seq2);
    }
    // return Sequence();
    return result;
}

Sequence SimpleOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket) const {
    return this->Compile(context, seq);  // I presume simple operator has no label_idx dependence.
}

Sequence SimpleOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket, const Ket& multi_label_ket) const {
    return this->Compile(context, seq);
}

Sequence SimpleOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket, const std::vector<Sequence> &args) const {
    return this->Compile(context, seq);
}
