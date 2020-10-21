//
// Created by Garry Morrison on 21/10/2020.
//

#include "../Function/misc.h"
#include "MultiSelfKet.h"


const std::string MultiSelfKet::to_string() const {
    std::string coeff;
    if ( !double_eq(value, 1.0) ) {  // handle value == -1 too!
        coeff = std::to_string(value);
    }
    std::string s;
    if (idx == 1 ) {
        s = "|__self>";
    } else {
        s = "|__self" + std::to_string(idx) + ">";
    }
    return coeff + s;
}

Ket MultiSelfKet::to_ket() const {
    if (idx == 1) { return Ket("__self", value); }
    return Ket("__self" + std::to_string(idx), value);
}

Superposition MultiSelfKet::to_sp() const {
    if (idx == 1) { return Superposition("__self", value); }
    return Superposition("__self" + std::to_string(idx), value);
}

Sequence MultiSelfKet::to_seq() const {
    if (idx == 1) { return Sequence("__self", value); }
    return Sequence("__self" + std::to_string(idx), value);
}


std::shared_ptr<BaseSequence> MultiSelfKet::b_add(std::shared_ptr<BaseSequence> bSeq) {  // Finish later!
    // return std::make_shared<Sequence>(this->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(this->to_seq());
    return result;
}

std::shared_ptr<BaseSequence> MultiSelfKet::b_append(std::shared_ptr<BaseSequence> bSeq) {
    // return std::make_shared<Sequence>(this->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(this->to_seq());
    return result;
}

Sequence MultiSelfKet::Compile(NewContext& context) const {
    return this->to_seq();
}

Sequence MultiSelfKet::Compile(NewContext& context, const ulong label_idx) const {
//     if (idx == 1) {
//         Ket tmp(label_idx, value);
//         return tmp.to_seq();
//     }
    return this->to_seq();
}

Sequence MultiSelfKet::Compile(ContextList& context) const {
    return this->to_seq();
}

Sequence MultiSelfKet::Compile(ContextList& context, const ulong label_idx) const {
//     if (idx == 1) {
//         Ket tmp(label_idx, value);
//         return tmp.to_seq();
//     }
    return this->to_seq();
}

Sequence MultiSelfKet::Compile(ContextList& context, const std::vector<Sequence>& args) const {
//     if (idx < args.size()) {  // Need to handle |_self0> too.
//         return args.at(idx);
//     }
    return this->to_seq();
}
