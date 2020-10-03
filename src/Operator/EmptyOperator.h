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
    Sequence Compile(ContextList& context, const Sequence& seq, const ulong label_idx) const { return seq; }
    Sequence Compile(ContextList& context, const Sequence& seq, const std::vector<Sequence>& args) const { return seq; }
    const std::string to_string() const { return ""; };

};


#endif //SDB3_1_EMPTYOPERATOR_H
