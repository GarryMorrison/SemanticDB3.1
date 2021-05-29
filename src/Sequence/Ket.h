//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_KET_H
#define SDB3_1_KET_H

// #define KET 0

#include "../KetMap/KetMap.h"
#include "Superposition.h"
#include "Sequence.h"
#include "BaseSequence.h"
// #include "../Context/NewContext.h"

class Superposition;
class NewContext;


class Ket : public BaseSequence {
private:
    ulong ket_label_idx;
    double ket_value;

public:
    Ket() {
        ket_label_idx = ket_map.get_idx("");
        ket_value = 1;
    }
    Ket(const ulong idx) {
        ket_label_idx = idx;
        ket_value = 1;
    }
    Ket(const ulong idx, const double v) {
        ket_label_idx = idx;
        ket_value = v;
    }
    Ket(const std::string& s) {
        ket_label_idx = ket_map.get_idx(s);
        ket_value = 1;
    }
    Ket(const std::string& s, const double v) {
        ket_label_idx = ket_map.get_idx(s);
        ket_value = v;
    }
    const int type() const { return KET; };
    const bool is_ket() const { return true; }
    // const ulong size() const;
    const size_t size() const;
    const ulong label_idx() const;
    const std::string label() const;
    const double value() const;
    const std::string to_string() const;
    const std::vector<ulong> label_split_idx() const;

    void multiply(const double d);
    void merge(const Ket k);
    void merge(const Ket k, const std::string& s);
    Superposition operator+(Ket& a);

    Ket to_ket() const;
    Superposition to_sp() const;
    Sequence to_seq() const;

    // BaseSequence* b_add(BaseSequence* bSeq);
    // BaseSequence* b_append(BaseSequence* bSeq);
    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq);
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq);

    // Nope! This produces "return type "class Sequence" is incomplete"
    // Sequence Compile(ContextList& context) const { return this->to_seq(); };
    // Sequence Compile(ContextList& context, const ulong label_idx) const { return this->to_seq(); };
    // Sequence Compile(ContextList& context, const std::vector<Sequence>& args) const { return this->to_seq(); };

    Sequence Compile(NewContext& context) const;
    Sequence Compile(NewContext& context, const ulong label_idx) const;

    Sequence Compile(ContextList& context) const;
    // Sequence Compile(ContextList& context, const ulong label_idx) const;
    // Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const;
    // Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const;

    Sequence Compile(ContextList& context, const Ket& label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const Ket& multi_label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const;

};


#endif //SDB3_1_KET_H
