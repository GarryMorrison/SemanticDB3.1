//
// Created by Garry Morrison on 2/06/2021.
//

#ifndef SDB3_1_WHILESTATEMENT_H
#define SDB3_1_WHILESTATEMENT_H

#include "BaseSequence.h"
#include "OperatorWithSequence.h"
#include "MultiLearnRule.h"


class WhileStatement : public BaseSequence {
private:
    OperatorWithSequence& condition;
    MultiLearnRule& while_rules;

public:
    WhileStatement(OperatorWithSequence &condition2, MultiLearnRule &while_rules2) : condition(condition2), while_rules(while_rules2) {};

    // methods needed to support BaseSequence:
    const int type() const { return WHILESTATEMENT; }
    const bool is_ket() const { return false; }
    const size_t size() const;
    const std::string to_string() const;
    const std::string to_string(const std::string &prefix) const;

    Ket to_ket() const { return Ket(); }  // Do something more interesting with these later!
    Superposition to_sp() const { return Superposition(); }
    Sequence to_seq() const { return Sequence(); }

    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq) { return bSeq; }  // Maybe change later!
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq) { return bSeq; }

    Sequence Compile(ContextList& context) const;  // needs filling out
    // Sequence Compile(ContextList& context, const ulong label_idx) const;  // needs filling out
    // Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const;  // needs filling out
    // Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const; // needs filling out

    Sequence Compile(ContextList& context, const Ket& label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const Ket& multi_label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const;
};


#endif //SDB3_1_WHILESTATEMENT_H
