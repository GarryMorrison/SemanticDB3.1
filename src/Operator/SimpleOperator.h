//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_SIMPLEOPERATOR_H
#define SDB3_1_SIMPLEOPERATOR_H

#include <string>
#include "../KetMap/KetMap.h"
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "BaseOperator.h"

class SimpleOperator : public BaseOperator {
private:
    ulong op_idx;

public:
    SimpleOperator(const ulong idx) : op_idx(idx) { };
    SimpleOperator(const std::string& s) { op_idx = ket_map.get_idx(s); };
    SimpleOperator(const Ket k) {
        auto split_idx = ket_map.get_split_idx(k.label_idx());
        if (split_idx.size() != 2) { op_idx = ket_map.get_idx(""); }
        if (split_idx[0] != ket_map.get_idx("op")) { op_idx = ket_map.get_idx(""); }
        op_idx = split_idx[1];
    }
    const std::string to_string() const { return ket_map.get_str(op_idx); };

    ulong get_idx() const { return op_idx; }
    std::string get_str() const { return ket_map.get_str(op_idx); }

    Sequence Compile(NewContext& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq) const;
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const;
};


#endif //SDB3_1_SIMPLEOPERATOR_H
