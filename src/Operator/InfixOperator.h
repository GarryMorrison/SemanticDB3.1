//
// Created by Garry Morrison on 26/05/2021.
//

#ifndef SDB3_1_INFIXOPERATOR_H
#define SDB3_1_INFIXOPERATOR_H

#include "BaseOperator.h"
#include "NumericOperator.h"
#include "OperatorSequence.h"

#define OPEQUAL 100
#define OPNOTEQUAL 101
#define OPAND 102
#define OPOR 103
#define OPPLUS 104
#define OPMINUS 105
#define OPMULT 106
#define OPDIV 107
#define OPMOD 108


class InfixOperator : public BaseOperator {
private:
    int infix_type;
    std::shared_ptr<BaseSequence> one;
    std::shared_ptr<BaseSequence> two;

public:
    InfixOperator(const std::shared_ptr<BaseSequence> &param0, int infix_type2, const std::shared_ptr<BaseSequence> &param1)
    : infix_type(infix_type2), one(param0), two(param1) {}

    InfixOperator(const OperatorWithSequence& param0, int infix_type2, const OperatorWithSequence& param1);

    const int type() const { return OPINFIX; }
    const ulong get_head_op_idx() const { return 0; }
    const std::shared_ptr<BaseOperator> get_tail_op() const { return std::make_shared<NumericOperator>(1); }
    const std::string to_string() const;

    Sequence process_compile(const Sequence &seq_one, const Sequence &seq_two) const;

    Sequence Compile(NewContext& context, const Sequence& seq) const { return seq; };  // Deprecated.
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const ulong multi_label_idx) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const std::vector<Sequence>& args) const;

    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket, const Ket& multi_label_ket) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket, const std::vector<Sequence>& args) const;

};


#endif //SDB3_1_INFIXOPERATOR_H
