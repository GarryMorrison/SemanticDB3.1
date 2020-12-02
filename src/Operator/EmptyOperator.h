//
// Created by Garry Morrison on 20/09/2020.
//

#ifndef SDB3_1_EMPTYOPERATOR_H
#define SDB3_1_EMPTYOPERATOR_H

#include <string>
#include "BaseOperator.h"


class EmptyOperator : public BaseOperator {
private:
public:
    EmptyOperator() { }
    Sequence Compile(NewContext& context, const Sequence& seq) const { return seq; };
    Sequence Compile(ContextList& context, const Sequence& seq) const { return seq; };
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const { return seq; }
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const ulong multi_label_idx) const { return seq; }
    // Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const std::vector<Sequence>& args) const { return seq; }

    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket) const { return seq; }
    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket, const Ket& multi_label_ket) const { return seq; }
    Sequence Compile(ContextList& context, const Sequence& seq, const Ket& label_ket, const std::vector<Sequence>& args) const { return seq; }


    const int type() const { return OPEMPTY; }
    const ulong get_head_op_idx() const { return 0; }
    const std::shared_ptr<BaseOperator> get_tail_op() const { return std::make_shared<NumericOperator>(1); }
    const std::string to_string() const { return ""; };

};


#endif //SDB3_1_EMPTYOPERATOR_H
