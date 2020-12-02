//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_BASESEQUENCE_H
#define SDB3_1_BASESEQUENCE_H

#include <memory>
#include "../KetMap/KetMap.h"


class Ket;
class Superposition;
class Sequence;
class ContextList;
class NewContext;

class BaseSequence {
private:

public:
    virtual const int type() const = 0;
    virtual const bool is_ket() const = 0;
    // virtual const ulong size() const = 0;
    virtual const size_t size() const = 0;
    virtual const std::string to_string() const = 0;

    virtual Ket to_ket() const = 0;
    virtual Superposition to_sp() const = 0;
    virtual Sequence to_seq() const = 0;

    // virtual BaseSequence* b_add(BaseSequence* bSeq) = 0;
    // virtual BaseSequence* b_append(BaseSequence* bSeq) = 0;
    virtual std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq) = 0;
    virtual std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq) = 0;

    virtual Sequence Compile(NewContext& context) const = 0;  // Really should get rid of these two.
    virtual Sequence Compile(NewContext& context, const ulong label_idx) const = 0;

    virtual Sequence Compile(ContextList& context) const = 0;
    // virtual Sequence Compile(ContextList& context, const ulong label_idx) const = 0;
    // virtual Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const = 0;
    // virtual Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const = 0;

    virtual Sequence Compile(ContextList& context, const Ket& label_ket) const = 0;
    virtual Sequence Compile(ContextList& context, const Ket& label_ket, const Ket& multi_label_ket) const = 0;
    virtual Sequence Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const = 0;

};

#endif //SDB3_1_BASESEQUENCE_H
