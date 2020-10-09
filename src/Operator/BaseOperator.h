//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_BASEOPERATOR_H
#define SDB3_1_BASEOPERATOR_H

#include <string>
#include <memory>
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "../Context/NewContext.h"

#define OPEMPTY 40
#define OPSIMPLE 41
#define OPNUMERIC 42
#define OPBRACKET 43
#define OPCOMPOUND 44
#define OPPOWER 45
#define OPSEQUENCE 46
#define OPFUNCTION 47

class BaseOperator {
private:

public:
    virtual const int type() const = 0;
    virtual const ulong get_head_op_idx() const = 0;
    virtual const std::shared_ptr<BaseOperator> get_tail_op() const = 0;
    virtual const std::string to_string() const = 0;
    virtual Sequence Compile(NewContext& context, const Sequence& seq) const = 0;  // will probably deprecate this one.
    virtual Sequence Compile(ContextList& context, const Sequence& seq) const = 0;
    virtual Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const = 0;
    // virtual Sequence Compile(ContextList& context, const Sequence& seq, const std::vector<Sequence>& args) const = 0;
};

#endif //SDB3_1_BASEOPERATOR_H
