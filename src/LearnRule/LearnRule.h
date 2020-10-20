//
// Created by Garry Morrison on 9/10/2020.
//

#ifndef SDB3_1_LEARNRULE_H
#define SDB3_1_LEARNRULE_H

#include "src/KetMap/KetMap.h"
#include "src/Context/Frame.h"
#include "src/Operator/OperatorSequence.h"
#include "src/Sequence/Sequence.h"
#include "src/Context/ContextList.h"
#include "src/Sequence/OperatorWithSequence.h"
// #include "src/Sequence/MultiLearnRule.h"


class LearnRule {
private:
    bool _valid_learn_rule;
    ulong _first_op_idx;
    std::shared_ptr<BaseOperator> _ket_op;
    std::shared_ptr<BaseSequence> _ket_like_seq;
    unsigned int _rule_type;
    std::shared_ptr<BaseSequence> _RHS_seq;

public:
    LearnRule(class OperatorWithSequence& head, unsigned int rule_type, class OperatorWithSequence& tail);
    // LearnRule(class OperatorWithSequence& head, unsigned int rule_type, class MultiLearnRule& tail);
    bool is_valid_learn_rule() { return _valid_learn_rule; }
    const std::string to_string() const;
    void Compile(ContextList &context);
};


#endif //SDB3_1_LEARNRULE_H
