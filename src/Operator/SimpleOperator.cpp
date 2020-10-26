//
// Created by Garry Morrison on 13/09/2020.
//

#include "SimpleOperator.h"
#include "../Sequence/BaseSequence.h"
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "../FunctionOperatorMap/FunctionOperatorMap.h"


Sequence SimpleOperator::Compile(NewContext& context, const Sequence& seq) const {
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
            seq3.multiply(k.value());
            seq2.add(seq3);
        }
        result.append(seq2);
    }
    return result;
}

Sequence SimpleOperator::Compile(ContextList& context, const Sequence& seq) const {
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
    } else if (fn_map.seq_fn.find(op_idx) != fn_map.seq_fn.end()) {
        auto fn = fn_map.seq_fn[op_idx];
        return fn(seq);  // Is this correct? Do we instead need seq.apply_seq_fn(fn)?
    }

    Sequence result;
    for (const auto &sp: seq) {
        Sequence seq2;
        for (const auto &k: sp) {
            //BaseSequence *b_rule = context.active_recall(op_idx, k.label_idx()); // Swap in later, when we have storedrules and memoizing rules
            // std::shared_ptr<BaseSequence> b_rule = context.recall(op_idx, k.label_idx());
            // std::shared_ptr<BaseSequence> b_rule = context.active_recall(op_idx, k.label_idx());
            // std::cout << "b_rule: " << b_rule->to_string() << std::endl;
            // std::cout << "b_rule type: " << b_rule->type() << std::endl;
            // Sequence seq3 = b_rule->to_seq();  // Need to test these three lines are right ...
            Sequence seq3 = context.active_recall(op_idx, k.label_idx());
            seq3.multiply(k.value());
            seq2.add(seq3);
        }
        result.append(seq2);
    }
    // return Sequence();
    return result;
}

Sequence SimpleOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    return this->Compile(context, seq);  // I presume simple operator has no label_idx dependence.
}

Sequence SimpleOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx, const ulong multi_label_idx) const {
    return this->Compile(context, seq);
}

Sequence SimpleOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx, const std::vector<Sequence> &args) const {
    return this->Compile(context, seq);
}