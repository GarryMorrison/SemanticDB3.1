//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_BASEOPERATOR_H
#define SDB3_1_BASEOPERATOR_H

#include <string>
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"
#include "../Context/NewContext.h"

class BaseOperator {
private:

public:
    virtual const std::string to_string() const = 0;
    virtual Sequence Compile(NewContext& context, const Sequence& seq) const = 0;  // will probably deprecate this one.
    virtual Sequence Compile(ContextList& context, const Sequence& seq) const = 0;
    virtual Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const = 0;
    // virtual Sequence Compile(ContextList& context, const Sequence& seq, const std::vector<Sequence>& args) const = 0;
};

#endif //SDB3_1_BASEOPERATOR_H
