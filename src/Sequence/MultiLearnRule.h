//
// Created by Garry Morrison on 20/10/2020.
//

#ifndef SDB3_1_MULTILEARNRULE_H
#define SDB3_1_MULTILEARNRULE_H

#include <vector>
#include <variant>
#include "OperatorWithSequence.h"
#include "../LearnRule/LearnRule.h"


class MultiLearnRule {
private:
    std::vector<std::variant<LearnRule, OperatorWithSequence>> vec_rules;  // I'm not sure variant is the best choice here!
                                                                           // Maybe make LearnRule BaseSequence?
public:
    MultiLearnRule(const LearnRule& learn_rule);
    MultiLearnRule(const OperatorWithSequence& operator_with_sequence);
    void append(const LearnRule& learn_rule);
    void append(const OperatorWithSequence& operator_with_sequence);
    const std::string to_string() const;
};


#endif //SDB3_1_MULTILEARNRULE_H
