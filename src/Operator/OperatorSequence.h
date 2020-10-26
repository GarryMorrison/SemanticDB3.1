//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_OPERATORSEQUENCE_H
#define SDB3_1_OPERATORSEQUENCE_H

#define SPLUS 0
#define SMINUS 1
#define SSEQ 2
#define SMERGE 3
#define SMERGE2 4

#include <memory>
#include "BaseOperator.h"
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "../Context/NewContext.h"
#include "src/Sequence/LearnRule.h"


class OperatorSequence : public BaseOperator {
private:
    unsigned int op_symbol;
    // std::vector<BaseOperator*> op_seq;
    std::shared_ptr<BaseOperator> head_op;
    std::vector<std::shared_ptr<BaseOperator> > tail_op_seq;
    // friend class LearnRule;

public:
    OperatorSequence() { op_symbol = SPLUS; }
    OperatorSequence(unsigned int type) { op_symbol = type; }
    // OperatorSequence(BaseOperator* b_op);
    // OperatorSequence(unsigned int type, BaseOperator* b_op);
    OperatorSequence(std::shared_ptr<BaseOperator> b_op);
    OperatorSequence(unsigned int type, std::shared_ptr<BaseOperator> b_op);

    const unsigned int symbol_type() const { return op_symbol; }
    // void append(BaseOperator* b_op);
    void append(std::shared_ptr<BaseOperator> b_op);
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    Sequence Compile(NewContext& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const ulong multi_label_idx) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const std::vector<Sequence>& args) const;

    const int type() const { return OPSEQUENCE; }
    const ulong get_head_op_idx() const;
    const std::shared_ptr<BaseOperator>  get_tail_op() const;
    const std::string to_string() const;
};


#endif //SDB3_1_OPERATORSEQUENCE_H
