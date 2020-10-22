//
// Created by Garry Morrison on 28/09/2020.
//

#ifndef SDB3_1_FUNCTIONOPERATOR_H
#define SDB3_1_FUNCTIONOPERATOR_H

#include <string>
#include "BaseOperator.h"
#include "../Sequence/BaseSequence.h"
#include "NumericOperator.h"
#include "../Sequence/OperatorWithSequence.h"


class FunctionOperator : public BaseOperator {
private:
    ulong idx;
    std::vector<std::shared_ptr<BaseSequence> > seq_vec;

public:
    FunctionOperator(const ulong idx2, const std::vector<std::shared_ptr<BaseSequence> > &seq_vec2) : idx(idx2), seq_vec(seq_vec2) {}
    FunctionOperator(const ulong idx2,
                     const std::shared_ptr<BaseSequence> &param0) : idx(idx2) {  // &param or just param?
        seq_vec.push_back(param0);
    }
    FunctionOperator(const ulong idx2,
                     const std::shared_ptr<BaseSequence> &param0,
                     const std::shared_ptr<BaseSequence> &param1) : idx(idx2) {
        seq_vec.push_back(param0);
        seq_vec.push_back(param1);
    }
    FunctionOperator(const ulong idx2,
                     const std::shared_ptr<BaseSequence> &param0,
                     const std::shared_ptr<BaseSequence> &param1,
                     const std::shared_ptr<BaseSequence> &param2) : idx(idx2) {
        seq_vec.push_back(param0);
        seq_vec.push_back(param1);
        seq_vec.push_back(param2);
    }
    FunctionOperator(const ulong idx2,
                     const std::shared_ptr<BaseSequence> &param0,
                     const std::shared_ptr<BaseSequence> &param1,
                     const std::shared_ptr<BaseSequence> &param2,
                     const std::shared_ptr<BaseSequence> &param3) : idx(idx2) {
        seq_vec.push_back(param0);
        seq_vec.push_back(param1);
        seq_vec.push_back(param2);
        seq_vec.push_back(param3);
    }
    FunctionOperator(const ulong idx2, const OperatorWithSequence& param0);
    FunctionOperator(const ulong idx2, const OperatorWithSequence& param0, const OperatorWithSequence& param1);
    FunctionOperator(const ulong idx2, const OperatorWithSequence& param0, const OperatorWithSequence& param1, const OperatorWithSequence& param2);
    FunctionOperator(const ulong idx2, const OperatorWithSequence& param0, const OperatorWithSequence& param1, const OperatorWithSequence& param2, const OperatorWithSequence& param3);
    FunctionOperator(const std::string &s,
                     const std::shared_ptr<BaseSequence> &param0) {
        idx = ket_map.get_idx(s);
        seq_vec.push_back(param0);
    }
    FunctionOperator(const std::string &s,
                     const std::shared_ptr<BaseSequence> &param0,
                     const std::shared_ptr<BaseSequence> &param1) {
        idx = ket_map.get_idx(s);
        seq_vec.push_back(param0);
        seq_vec.push_back(param1);
    }
    FunctionOperator(const std::string &s,
                     const std::shared_ptr<BaseSequence> &param0,
                     const std::shared_ptr<BaseSequence> &param1,
                     const std::shared_ptr<BaseSequence> &param2) {
        idx = ket_map.get_idx(s);
        seq_vec.push_back(param0);
        seq_vec.push_back(param1);
        seq_vec.push_back(param2);
    }
    FunctionOperator(const std::string &s,
                     const std::shared_ptr<BaseSequence> &param0,
                     const std::shared_ptr<BaseSequence> &param1,
                     const std::shared_ptr<BaseSequence> &param2,
                     const std::shared_ptr<BaseSequence> &param3) {
        idx = ket_map.get_idx(s);
        seq_vec.push_back(param0);
        seq_vec.push_back(param1);
        seq_vec.push_back(param2);
        seq_vec.push_back(param3);
    }

    FunctionOperator(const std::string &s, const std::vector<std::shared_ptr<BaseSequence> > &seq_vec2) : seq_vec(seq_vec2) { idx = ket_map.get_idx(s); }

    const int type() const { return OPFUNCTION; }
    const ulong get_head_op_idx() const { return 0; }
    const std::shared_ptr<BaseOperator> get_tail_op() const { return std::make_shared<NumericOperator>(1); }
    const std::string to_string() const;
    Sequence Compile(NewContext& context, const Sequence& seq) const { return seq; };
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx, const ulong multi_label_idx) const; // Put in BaseOperator too.
};


#endif //SDB3_1_FUNCTIONOPERATOR_H
