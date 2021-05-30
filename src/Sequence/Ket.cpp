//
// Created by Garry Morrison on 12/09/2020.
//

#include <cmath>
#include "../Function/misc.h"
#include "Ket.h"

const ulong Ket::label_idx() const {
    return ket_label_idx;
}

const std::string Ket::label() const {
    std::string result = ket_map.get_str(ket_label_idx);
    return result;
}

const double Ket::value() const {
    return ket_value;
}

const size_t Ket::size() const {
    size_t result;
    if (ket_map.get_idx("") == ket_label_idx) {  // Hard wire in 0 for get_idx("").
        result = 0;
    }
    else {
        result = 1;
    }
    return result;
}

const std::string Ket::to_string() const {
    std::string s;
    if ( double_eq(ket_value, 1.0) ) {
        s = "|" + ket_map.get_str(ket_label_idx) + ">";
    } else if ( double_eq(ket_value, -1.0) ) {
        s = "- |" + ket_map.get_str(ket_label_idx) + ">";
    } else if ( double_eq(ket_value, round(ket_value)) ) {
        s = std::to_string((int)round(ket_value)) + "|" + ket_map.get_str(ket_label_idx) + ">";
    }
    else {
        s = std::to_string(ket_value) + "|" + ket_map.get_str(ket_label_idx) + ">";
    }
    return s;
}

const std::string Ket::to_string(const std::string &prefix) const {
    return prefix + this->to_string();
}

const std::vector<ulong> Ket::label_split_idx() const {
    return ket_map.get_split_idx(ket_label_idx);
}

void Ket::multiply(const double d) {
    ket_value *= d;
}

void Ket::merge(const Ket k) {
    ket_value *= k.ket_value;
    std::string s = this->label() + k.label();
    ket_label_idx = ket_map.get_idx(s);
}

void Ket::merge(const Ket k, const std::string& s) {
    ket_value *= k.ket_value;
    std::string s2 = this->label() + s + k.label();
    ket_label_idx = ket_map.get_idx(s2);
}

Superposition Ket::operator+(Ket& a) {
    Superposition tmp;
    tmp.add(*this);
    tmp.add(a);
    return tmp;
}

Ket Ket::to_ket() const {
    // Ket tmp(*this);
    // return tmp;
    return *this;
}

Superposition Ket::to_sp() const {
    Superposition tmp(this->label_idx(), this->value());
    return tmp;
}

Sequence Ket::to_seq() const {
    Sequence tmp(*this);
    return tmp;
}

/*
BaseSequence* Ket::b_add(BaseSequence* bSeq) {
    Sequence *seq = new Sequence(*this);
    seq->add(bSeq->to_seq());
    return seq;
}

BaseSequence* Ket::b_append(BaseSequence* bSeq) {
    Sequence *seq = new Sequence(*this);
    seq->append(bSeq->to_seq());
    return seq;
}
*/

std::shared_ptr<BaseSequence> Ket::b_add(std::shared_ptr<BaseSequence> bSeq) {
    Sequence tmp(*this);
    tmp.add(bSeq->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(tmp);
    return result;
}

std::shared_ptr<BaseSequence> Ket::b_append(std::shared_ptr<BaseSequence> bSeq) {
    Sequence tmp(*this);
    tmp.append(bSeq->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(tmp);
    return result;
}

Sequence Ket::Compile(NewContext& context) const {
    return this->to_seq();
}

Sequence Ket::Compile(NewContext& context, const ulong label_idx) const {
    return this->to_seq();
}


Sequence Ket::Compile(ContextList& context) const {
    return this->to_seq();
}

Sequence Ket::Compile(ContextList& context, const Ket& label_ket) const {
    // return this->to_seq();
    Sequence tmp(*this);
    tmp.multiply(label_ket.value());
    return tmp;
}

Sequence Ket::Compile(ContextList &context, const Ket& label_ket, const Ket& multi_label_ket) const {
    return this->to_seq();
}

Sequence Ket::Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const {
    return this->to_seq();
}
