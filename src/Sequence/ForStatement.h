//
// Created by Garry Morrison on 31/05/2021.
//

#ifndef SDB3_1_FORSTATEMENT_H
#define SDB3_1_FORSTATEMENT_H

#define FOR_TYPE 0
#define SFOR_TYPE 1

#include "BaseSequence.h"
#include "OperatorWithSequence.h"
#include "MultiLearnRule.h"

class ForStatement : public BaseSequence {
private:
    int for_type;
    ulong op_idx;
    ulong ket_idx;
    OperatorWithSequence& iterables;
    MultiLearnRule& for_rules;

public:
    ForStatement(int for_type2, ulong op_idx2, ulong ket_idx2, OperatorWithSequence &iterables2, MultiLearnRule &for_rules2)
        : for_type(for_type2), op_idx(op_idx2), ket_idx(ket_idx2), iterables(iterables2), for_rules(for_rules2) {}

    // methods needed to support BaseSequence:
    const int type() const { return FORSTATEMENT; }
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


#endif //SDB3_1_FORSTATEMENT_H
