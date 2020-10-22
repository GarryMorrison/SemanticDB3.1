//
// Created by Garry Morrison on 20/10/2020.
//

#include "MultiLearnRule.h"

MultiLearnRule::MultiLearnRule(const LearnRule &learn_rule) {
    vec_rules.push_back(std::make_shared<LearnRule>(learn_rule));
}

MultiLearnRule::MultiLearnRule(const OperatorWithSequence &operator_with_sequence) {
    vec_rules.push_back(std::make_shared<OperatorWithSequence>(operator_with_sequence));
}

void MultiLearnRule::append(const LearnRule &learn_rule) {
    vec_rules.push_back(std::make_shared<LearnRule>(learn_rule));
}

void MultiLearnRule::append(const OperatorWithSequence &operator_with_sequence) {
    vec_rules.push_back(std::make_shared<OperatorWithSequence>(operator_with_sequence));
}

const std::string MultiLearnRule::to_string() const {
    std::string s = "\n";
    for (const auto& rule: vec_rules) {
        s += "    " + rule->to_string() + "\n";
    }
    return s;
}

Sequence MultiLearnRule::Compile(NewContext &context) const {
    return Sequence();
}

Sequence MultiLearnRule::Compile(NewContext &context, const ulong label_idx) const {
    return Sequence();
}


Sequence MultiLearnRule::Compile(ContextList &context) const {
    Sequence result;
    for (const auto& rule: vec_rules) {
        result = rule->Compile(context);
    }
    return result;
}

Sequence MultiLearnRule::Compile(ContextList &context, const ulong label_idx) const {
    Sequence result;
    for (const auto& rule: vec_rules) {
        result = rule->Compile(context, label_idx, label_idx);
    }
    return result;
}

Sequence MultiLearnRule::Compile(ContextList &context, const ulong label_idx, const ulong multi_label_idx) const {
    return this->Compile(context, label_idx);
}

Sequence MultiLearnRule::Compile(ContextList &context, const std::vector<Sequence> &args) const {
    return Sequence();
}

