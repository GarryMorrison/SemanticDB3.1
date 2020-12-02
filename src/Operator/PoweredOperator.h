//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_POWEREDOPERATOR_H
#define SDB3_1_POWEREDOPERATOR_H

#include <string>
#include <memory>
#include "BaseOperator.h"
#include "NumericOperator.h"

class PoweredOperator : public BaseOperator {
private:
    // BaseOperator* b_op;
    std::shared_ptr<BaseOperator> b_op;
    unsigned int pow_;

public:
    // PoweredOperator(BaseOperator* base_op, const unsigned int power) {b_op = base_op; pow_ = power; };
    PoweredOperator(std::shared_ptr<BaseOperator> base_op, const unsigned int power) {b_op = base_op; pow_ = power; };
    Sequence Compile(NewContext& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const ulong multi_label_idx) const;
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const std::vector<Sequence>& args) const;

    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket, const Ket& multi_label_ket) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket, const std::vector<Sequence>& args) const;

    const int type() const { return OPPOWER; }
    const ulong get_head_op_idx() const { return 0; }
    const std::shared_ptr<BaseOperator> get_tail_op() const { return std::make_shared<NumericOperator>(1); }
    const std::string to_string() const;
};


#endif //SDB3_1_POWEREDOPERATOR_H
