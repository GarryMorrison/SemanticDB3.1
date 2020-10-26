//
// Created by Admin on 13/09/2020.
//

#include "CompoundOperator.h"
#include "../FunctionOperatorMap/FunctionOperatorMap.h"

/* Former, raw pointer version:
void CompoundOperator::append(CompoundConstant* const_op) {
    const_seq.push_back(const_op);
}
*/

void CompoundOperator::append(std::shared_ptr<CompoundConstant> constant) {
    const_vec.push_back(constant);
}

Sequence CompoundOperator::Compile(NewContext& context, const Sequence& seq) const {
    return seq;  // Fill out later!
}

Sequence CompoundOperator::Compile(ContextList& context, const Sequence& seq) const {
    if (fn_map.compound_built_in.find(op_idx) != fn_map.compound_built_in.end()) {
        auto fn = fn_map.compound_built_in[op_idx];
        return fn(&seq, const_vec);
    }else if (fn_map.compound_ket_fn.find(op_idx) != fn_map.compound_ket_fn.end()) {
        auto ket_fn = fn_map.compound_ket_fn[op_idx];
        return seq.apply_compound_ket_fn(ket_fn, const_vec);  // Change parameters to & later.
    } else if (fn_map.compound_sp_fn.find(op_idx) != fn_map.compound_sp_fn.end()) {
        auto sp_fn = fn_map.compound_sp_fn[op_idx];
        return seq.apply_compound_sp_fn(sp_fn, const_vec);  // Change parameters to & later.
    } else if (fn_map.compound_sigmoids.find(op_idx) != fn_map.compound_sigmoids.end()) {
        auto sigmoid = fn_map.compound_sigmoids[op_idx];
        return seq.apply_compound_sigmoid(sigmoid, const_vec); // Change parameters to & later.
    } else if (fn_map.compound_context_seq_fn.find(op_idx) != fn_map.compound_context_seq_fn.end()) {
        auto fn = fn_map.compound_context_seq_fn[op_idx];
        return fn(seq, context, const_vec);
    }
    return seq;
}

Sequence CompoundOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    return this->Compile(context, seq);
}

Sequence CompoundOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx, const ulong multi_label_idx) const {
    return this->Compile(context, seq);
}
Sequence CompoundOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx, const std::vector<Sequence> &args) const {
    return this->Compile(context, seq);
}

const std::string CompoundOperator::to_string() const {
    std::string s;
    s = ket_map.get_str(op_idx) + "[";
    bool first_pass = true;
    // for (const auto c: const_seq ) {
    for (const auto c: const_vec ) {
        if (first_pass) {
            s += c->to_string();
            first_pass = false;
        } else {
            s += ", " + c->to_string();
        }
    }
    return s + "]";
}
