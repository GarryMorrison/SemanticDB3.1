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

MultiLearnRule::MultiLearnRule(const IfElseStatement &if_else_statement) {
    vec_rules.push_back(std::make_shared<IfElseStatement>(if_else_statement));
}

MultiLearnRule::MultiLearnRule(const ForStatement &for_statement) {
    vec_rules.push_back(std::make_shared<ForStatement>(for_statement));
}


void MultiLearnRule::append(const LearnRule &learn_rule) {
    vec_rules.push_back(std::make_shared<LearnRule>(learn_rule));
}

void MultiLearnRule::append(const OperatorWithSequence &operator_with_sequence) {
    vec_rules.push_back(std::make_shared<OperatorWithSequence>(operator_with_sequence));
}

void MultiLearnRule::append(const IfElseStatement &if_else_statement) {
    vec_rules.push_back(std::make_shared<IfElseStatement>(if_else_statement));
}

void MultiLearnRule::append(const ForStatement &for_statement) {
    vec_rules.push_back(std::make_shared<ForStatement>(for_statement));
}


const std::string MultiLearnRule::to_string() const {
    std::string s = "\n";
    for (const auto& rule: vec_rules) {
        s += "    " + rule->to_string() + "\n";
    }
    return s;
}

const std::string MultiLearnRule::to_string(const std::string &prefix) const {
    std::string s = "\n";
    for (const auto& rule: vec_rules) {
        s += "    " + rule->to_string(prefix) + "\n";
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

Sequence MultiLearnRule::Compile(ContextList &context, const Ket& label_ket) const {
    Sequence result;
    for (const auto& rule: vec_rules) {
        result = rule->Compile(context, label_ket, label_ket);  // May want to change the middle label_ket to something else.
    }
    return result;
}

Sequence MultiLearnRule::Compile(ContextList &context, const Ket& label_ket, const Ket& multi_label_ket) const {
    Sequence result;
    for (const auto& rule: vec_rules) {
        result = rule->Compile(context, label_ket, multi_label_ket);
    }
    return result;
}

Sequence MultiLearnRule::Compile(ContextList &context, const Ket& label_ket, const std::vector<Sequence> &args) const {
    Sequence result;
    for (const auto& rule: vec_rules) {
        result = rule->Compile(context, label_ket, args);
    }
    return result;
}

