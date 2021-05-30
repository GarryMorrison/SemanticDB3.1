//
// Created by Garry Morrison on 29/05/2021.
//

#ifndef SDB3_1_IFELSESTATEMENT_H
#define SDB3_1_IFELSESTATEMENT_H

// #define IFELSESTATEMENT 13

#include "BaseSequence.h"
#include "OperatorWithSequence.h"
#include "MultiLearnRule.h"


class IfElseStatement : public BaseSequence {
private:
    OperatorWithSequence& condition;
    MultiLearnRule& if_rules;
    MultiLearnRule& else_rules;

public:
    // IfElseStatement(OperatorWithSequence &our_condition, MultiLearnRule &if_rules) : condition(our_condition), if_vec_rules(if_rules) {
    //     else_vec_rules = MultiLearnRule();
    // };
    IfElseStatement(OperatorWithSequence &condition2, MultiLearnRule &if_rules2, MultiLearnRule &else_rules2) : condition(condition2), if_rules(if_rules2), else_rules(else_rules2) {};

    // void append(const LearnRule& learn_rule);
    // void append(const OperatorWithSequence& operator_with_sequence);

    // methods needed to support BaseSequence:
    const int type() const { return IFELSESTATEMENT; }
    const bool is_ket() const { return false; }
    const size_t size() const;
    const std::string to_string() const;
    const std::string to_string(const std::string &prefix) const;

    Ket to_ket() const { return Ket(); }  // Do something more interesting with these later!
    Superposition to_sp() const { return Superposition(); }
    Sequence to_seq() const { return Sequence(); }

    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq) { return bSeq; }  // Maybe change later!
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq) { return bSeq; }

    Sequence Compile(NewContext& context) const;  // Really should get rid of these two.
    Sequence Compile(NewContext& context, const ulong label_idx) const;

    Sequence Compile(ContextList& context) const;  // needs filling out
    // Sequence Compile(ContextList& context, const ulong label_idx) const;  // needs filling out
    // Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const;  // needs filling out
    // Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const; // needs filling out

    Sequence Compile(ContextList& context, const Ket& label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const Ket& multi_label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const;
};


#endif //SDB3_1_IFELSESTATEMENT_H
