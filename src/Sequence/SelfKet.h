//
// Created by Garry Morrison on 17/09/2020.
//

#ifndef SDB3_1_SELFKET_H
#define SDB3_1_SELFKET_H

#define SELFKET 3

#include "BaseSequence.h"
#include "Ket.h"
#include "Superposition.h"
#include "Sequence.h"
// #include "../Context/NewContext.h"


class SelfKet : public BaseSequence {
private:
    unsigned int idx;
    double value;

public:
    SelfKet() { idx = 1; value = 1;}
    SelfKet(const unsigned int idx2) { idx = idx2; value = 1;}
    SelfKet(const unsigned int idx2, const double d) { idx = idx2; value = d; }
    const int type() const { return SELFKET; }
    const bool is_ket() const { return true; }
    const std::string to_string() const;
    // const ulong size() const { return 1; }
    const size_t size() const { return 1; }
    void multiply(const double d) { value *= d; }
    Ket to_ket() const;
    Superposition to_sp() const;
    Sequence to_seq() const;
    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq);
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq);

    Sequence Compile(NewContext& context) const;
    Sequence Compile(NewContext& context, const ulong label_idx) const;

    Sequence Compile(ContextList& context) const;
    Sequence Compile(ContextList& context, const ulong label_idx) const;
    Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const;
    Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const;
};


#endif //SDB3_1_SELFKET_H
