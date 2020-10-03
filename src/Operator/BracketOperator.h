//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_BRACKETOPERATOR_H
#define SDB3_1_BRACKETOPERATOR_H

#include <string>
#include "OperatorSequence.h"
#include "BaseOperator.h"

class BracketOperator : public BaseOperator {
private:
    std::vector<OperatorSequence> op_seq_vec;

public:
    BracketOperator(OperatorSequence &op_seq) { op_seq_vec.push_back(op_seq); }
    BracketOperator(BracketOperator &b_operator) { op_seq_vec = b_operator.op_seq_vec; }
    void append(OperatorSequence &op_seq) { op_seq_vec.push_back(op_seq); } // do we want to call this append instead of push??

    Sequence Compile(NewContext& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const std::vector<Sequence>& args) const;
    const std::string to_string() const;

};


#endif //SDB3_1_BRACKETOPERATOR_H
