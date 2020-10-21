//
// Created by Garry Morrison on 9/10/2020.
//

#ifndef SDB3_1_LEARNRULE_H
#define SDB3_1_LEARNRULE_H

#define LEARNRULE 11

#include "src/KetMap/KetMap.h"
#include "src/Context/Frame.h"
#include "src/Operator/OperatorSequence.h"
#include "src/Sequence/Sequence.h"
#include "src/Context/ContextList.h"
#include "src/Sequence/OperatorWithSequence.h"
#include "BaseSequence.h"
// #include "src/Sequence/MultiLearnRule.h"


class LearnRule : public BaseSequence {
private:
    bool _valid_learn_rule;
    ulong _first_op_idx;
    std::shared_ptr<BaseOperator> _ket_op;
    std::shared_ptr<BaseSequence> _ket_like_seq;
    unsigned int _rule_type;
    std::shared_ptr<BaseSequence> _RHS_seq;

public:
    LearnRule(class OperatorWithSequence& head, unsigned int rule_type, class OperatorWithSequence& tail);
    LearnRule(class OperatorWithSequence& head, unsigned int rule_type, class MultiLearnRule& tail);
    bool is_valid_learn_rule() { return _valid_learn_rule; }

    // void Compile(ContextList &context); We need it to return Sequence instead.

    // methods needed to support BaseSequence:
    const int type() const { return LEARNRULE; }
    const bool is_ket() const { return false; }
    const size_t size() const { return 1; }
    const std::string to_string() const;

    Ket to_ket() const { return Ket(); }  // Do something more interesting with these later!
    Superposition to_sp() const { return Superposition(); }
    Sequence to_seq() const { return Sequence(); }

    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq) { return bSeq; }  // Maybe change later!
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq) { return bSeq; }

    Sequence Compile(NewContext& context) const;  // Really should get rid of these two.
    Sequence Compile(NewContext& context, const ulong label_idx) const;

    Sequence Compile(ContextList& context) const;
    Sequence Compile(ContextList& context, const ulong label_idx) const;  // needs filling out
    Sequence Compile(ContextList& context, const std::vector<Sequence>& args) const; // needs filling out
};


#endif //SDB3_1_LEARNRULE_H
