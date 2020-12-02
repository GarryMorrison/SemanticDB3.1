//
// Created by Garry Morrison on 17/09/2020.
//

#include <memory>
#include "SelfKet.h"
#include "../Function/misc.h"
#include "../Context/NewContext.h"

const std::string SelfKet::to_string() const {
    std::string coeff = "";
    if ( !double_eq(value, 1.0) ) {  // handle value == -1 too!
        coeff = std::to_string(value);
    }
    std::string s;
    if (idx == 1 ) {
        s = "|_self>";
    } else {
        s = "|_self" + std::to_string(idx) + ">";
    }
    return coeff + s;
}

Ket SelfKet::to_ket() const {
    Ket tmp("_self", value); // Should this be "_self" + std::to_string(idx)?
    return tmp;
}

Superposition SelfKet::to_sp() const {
    Superposition tmp("_self", value); // Should this be "_self" + std::to_string(idx)?
    return tmp;
}

Sequence SelfKet::to_seq() const {
    // Ket tmp("_self", value);
    Sequence seq("_self", value); // Should this be "_self" + std::to_string(idx)?
    return seq;
}

std::shared_ptr<BaseSequence> SelfKet::b_add(std::shared_ptr<BaseSequence> bSeq) {  // Finish later!
    // return std::make_shared<Sequence>(this->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(this->to_seq());
    return result;
}

std::shared_ptr<BaseSequence> SelfKet::b_append(std::shared_ptr<BaseSequence> bSeq) {
    // return std::make_shared<Sequence>(this->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(this->to_seq());
    return result;
}

Sequence SelfKet::Compile(NewContext& context) const {
    return this->to_seq();
}

Sequence SelfKet::Compile(NewContext& context, const ulong label_idx) const {
    if (idx == 1) {
        Ket tmp(label_idx, value);
        return tmp.to_seq();
    }
    return this->to_seq();
}

Sequence SelfKet::Compile(ContextList& context) const {
    return this->to_seq();
}

Sequence SelfKet::Compile(ContextList& context, const Ket& label_ket) const {
    if (idx == 1) {
        // Ket tmp(label_idx, value);
        // return tmp.to_seq();
        return label_ket.to_seq();
    }
    return this->to_seq();
}

Sequence SelfKet::Compile(ContextList &context, const Ket& label_ket, const Ket& multi_label_ket) const {
    if (idx == 1) {
        // Ket tmp(label_idx, value);
        // return tmp.to_seq();
        return label_ket.to_seq();
    }
    return this->to_seq();
}

Sequence SelfKet::Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const {
    if (idx == 1) {
        // Ket tmp(label_idx, value);
        // return tmp.to_seq();
        return label_ket.to_seq();
    }
    return this->to_seq();
}
