//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_SUPERPOSITION_H
#define SDB3_1_SUPERPOSITION_H

#define SUPERPOSITION 1

#include <unordered_map>
#include <string>
#include <functional>
#include "../KetMap/KetMap.h"
#include "Ket.h"
#include "SuperpositionIter.h"
#include "BaseSequence.h"
#include "../CompoundConstant/CompoundConstant.h"

class Ket;
class SuperpositionIter;
class Sequence;
class ContextList;


class Superposition : public BaseSequence {
private:
    std::unordered_map<ulong, double> sp;
    std::vector<ulong> sort_order;

public:
    Superposition() {};
    ~Superposition() {};
    Superposition(const ulong idx);
    Superposition(const std::string& s);
    Superposition(const std::string&s, const double v);
    Superposition(const ulong idx, const double v);
    Superposition(Ket k);
    Superposition(const Superposition &other_sp) : sp(other_sp.sp), sort_order(other_sp.sort_order) {};
    Superposition(const std::vector<ulong> &vec);

    const int type() const { return SUPERPOSITION; };
    const bool is_ket() const { if (sort_order.size() <= 1 ) { return true; } return false; }
    // const ulong size() const;
    const size_t size() const;
    const std::string to_string() const;

    Ket to_ket() const;
    Superposition to_sp() const;
    Sequence to_seq() const;

    // BaseSequence* b_add(BaseSequence* bSeq);
    // BaseSequence* b_append(BaseSequence* bSeq);
    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq);
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq);

    void add(const ulong idx);
    void add(const ulong idx, const double v);
    void add(const std::string& s);
    void add(const std::string& s, const double v);
    void add(Ket& a);
    void add(const Superposition& a);
    void add(const std::vector<ulong> &vec);
    void multiply(const double d);
    void coeff_sort();
    void ket_sort();
    void natural_sort();
    // void select(const ulong start, const ulong stop);

    void merge(const Superposition& sp2, const std::string& s);
    void merge(const Superposition& sp2);

    Superposition operator+(Ket& b);
    Superposition operator+(Superposition& sp);

    // SuperpositionIter:
    Ket get(ulong idx) const;
    SuperpositionIter begin () const;
    SuperpositionIter end () const;

    // A collection of built in operators:
    Superposition shuffle() const;
    double find_value(const Ket &k) const;
    Ket do_you_know() const;
    Ket how_many() const;
    Ket measure_currency() const;
    double find_min_coeff() const;
    double find_max_coeff() const;
    Superposition rescale(const double t) const;
    Superposition rescale() const;
    Superposition normalize(const double t) const;
    Superposition normalize() const;
    Superposition reverse() const;
    Superposition drop() const;
    Superposition drop_below(const double t) const;
    Superposition drop_above(const double t) const;
    Superposition pick(const ulong n) const;
    Ket pick_elt() const;
    Ket weighted_pick_elt() const;
    Superposition select(const int start, const int stop) const;

    Superposition apply_sigmoid(const std::function<double(double)> &sigmoid) const;
    Superposition apply_sigmoid(const std::function<double(double,double)> &sigmoid, const double t) const;
    Superposition apply_sigmoid(const std::function<double(double,double,double)> &sigmoid, const double t1, const double t2) const;
    Superposition apply_compound_sigmoid(const std::function<double(double, const std::vector<std::shared_ptr<CompoundConstant> >)> &fn, std::vector<std::shared_ptr<CompoundConstant> > parameters) const;

    // Nope! This produces "return type "class Sequence" is incomplete"
    // Sequence Compile(ContextList& context) const { return this->to_seq(); };
    // Sequence Compile(ContextList& context) const { Sequence tmp(*this); return tmp; };
    // Sequence Compile(ContextList& context, const ulong label_idx) const { return this->to_seq(); };
    // Sequence Compile(ContextList& context, const std::vector<Sequence>& args) const { return this->to_seq(); };

    Sequence Compile(NewContext& context) const;
    Sequence Compile(NewContext& context, const ulong label_idx) const;

    Sequence Compile(ContextList& context) const;
    Sequence Compile(ContextList& context, const ulong label_idx) const;
    Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const;
    Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const;

};


#endif //SDB3_1_SUPERPOSITION_H
