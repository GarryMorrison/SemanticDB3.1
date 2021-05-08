//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_SEQUENCE_H
#define SDB3_1_SEQUENCE_H

#define SEQUENCE 2

#include <vector>
#include <string>
#include <memory>
#include "Ket.h"
#include "Superposition.h"
#include "BaseSequence.h"
#include "../CompoundConstant/CompoundConstant.h"


class Sequence : public BaseSequence {
private:
    std::vector<Superposition> seq;

public:
    Sequence() {};
    ~Sequence() {};
    Sequence(const std::string& s);
    Sequence(const std::string& s, double d);
    Sequence(const Ket& k);
    Sequence(const Superposition& sp);
    Sequence(const Sequence& seq);
    Sequence(const std::vector<Superposition>& vec) : seq(vec) {};

    bool operator==(const Sequence& other) const;

    const int type() const { return SEQUENCE; };
    const bool is_ket() const;
    // const ulong size() const { return seq.size(); };  // Should this be in Sequence.cpp?
    // const size_t size() const { return seq.size(); };
    // const ulong size() const;
    const size_t size() const;
    const std::string to_string() const;
    const std::string readable_display() const;

    Ket to_ket() const;
    Superposition to_sp() const;
    Sequence to_seq() const;

    void add(const ulong idx);
    void add(const Ket& k);
    void add(const Superposition& sp);
    void add(const Sequence& seq);

    void multiply(const double d);

    void merge(const Sequence& seq2, const std::string& s);
    void merge(const Sequence& seq2);

    void append(const Ket& k);  // How about append(std::string, double) ?
    void append(const Superposition& sp);
    void append(const Sequence& seq);
    void append(const std::string& s);
    void append(const std::string& s, const double v);

    // BaseSequence* b_add(BaseSequence* bSeq);
    // BaseSequence* b_append(BaseSequence* bSeq);
    std::shared_ptr<BaseSequence> b_add(std::shared_ptr<BaseSequence> bSeq);
    std::shared_ptr<BaseSequence> b_append(std::shared_ptr<BaseSequence> bSeq);

    // define an iterator for our sequence class:
    Superposition get(ulong idx) const;
    typedef typename std::vector<Superposition>::iterator iterator;
    typedef typename std::vector<Superposition>::const_iterator const_iterator;

    iterator begin() {return seq.begin();}
    const_iterator begin() const {return seq.begin();}
    const_iterator cbegin() const {return seq.cbegin();}
    iterator end() {return seq.end();}
    const_iterator end() const {return seq.end();}
    const_iterator cend() const {return seq.cend();}

    void set(ulong idx, const Superposition &sp);

    Sequence apply_sigmoid(const std::function<double(double)> &sigmoid) const;
    Sequence apply_sigmoid(const std::function<double(double,double)> &sigmoid, const double t) const;
    Sequence apply_sigmoid(const std::function<double(double,double,double)> &sigmoid, const double t1, const double t2) const;

    Sequence apply_ket_fn(const std::function<Sequence(const Ket&)> &fn) const;
    Sequence apply_sp_fn(const std::function<Sequence(const Superposition&)> &fn) const;

    Sequence apply_context_ket_fn(const std::function<Sequence(const Ket&, ContextList&)> &fn, ContextList &context) const;

    Sequence apply_compound_sigmoid(std::function<double(double, const std::vector<std::shared_ptr<CompoundConstant> >&)> fn, std::vector<std::shared_ptr<CompoundConstant> > parameters) const;
    Sequence apply_compound_ket_fn(std::function<Sequence(Ket, const std::vector<std::shared_ptr<CompoundConstant> >&)> fn, std::vector<std::shared_ptr<CompoundConstant> > parameters) const;
    Sequence apply_compound_sp_fn(std::function<Sequence(const Superposition&, const std::vector<std::shared_ptr<CompoundConstant> >&)> fn, std::vector<std::shared_ptr<CompoundConstant> > parameters) const;

    // A collection of built in operators:
    Sequence shuffle() const;
    Sequence sshuffle() const;
    Sequence sreverse() const;
    Sequence sdrop() const;
    // double find_value(const Ket &k) const;
    Sequence do_you_know() const;
    Sequence how_many() const;
    Sequence measure_currency() const;
    Sequence show_many() const;
    Sequence sidx() const;
    // double find_min_coeff() const;
    // double find_max_coeff() const;
    Sequence rescale(const double t) const;
    Sequence rescale() const;
    Sequence op_rescale0() const;
    Sequence op_rescale1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence normalize(const double t) const;
    Sequence normalize() const;
    Sequence op_normalize0() const;
    Sequence op_normalize1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence reverse() const;
    Sequence drop() const;
    Sequence drop_below(const double t) const;
    Sequence op_drop_below1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence drop_above(const double t) const;
    Sequence op_drop_above1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence pick(const ulong n) const;
    Sequence op_pick1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence pick_elt() const;
    Sequence weighted_pick_elt() const;
    Sequence ket_sort() const;
    Sequence coeff_sort() const;
    Sequence natural_sort() const;
    Sequence sort_by(ContextList& context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence select(const int start, const int stop) const;
    Sequence op_select2(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence sselect(const int start, const int stop) const;
    Sequence op_sselect2(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence op_srotate_left() const;
    Sequence op_srotate_right() const;
    Sequence op_srotate_left1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence op_srotate_right1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence op_rotate_left() const;
    Sequence op_rotate_right() const;
    Sequence op_rotate_left1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;
    Sequence op_rotate_right1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const;


    void dummy1(std::shared_ptr<CompoundConstant> &constant);  // Delete these two later!
    void dummy2(std::vector<std::shared_ptr<CompoundConstant> > &const_vec);

    Sequence Compile(NewContext& context) const { return (*this); };
    Sequence Compile(NewContext& context, const ulong label_idx) const { return (*this); };

    Sequence Compile(ContextList& context) const { return (*this); };
    // Sequence Compile(ContextList& context, const ulong label_idx) const { return (*this); };
    // Sequence Compile(ContextList& context, const ulong label_idx, const ulong multi_label_idx) const { return *this; }
    // Sequence Compile(ContextList& context, const ulong label_idx, const std::vector<Sequence>& args) const { return (*this); };

    Sequence Compile(ContextList& context, const Ket& label_ket) const;
    Sequence Compile(ContextList& context, const Ket& label_ket, const Ket& multi_label_ket) const { return *this; };  // These two probably not correct now.
    Sequence Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const { return *this; }; // Need to take into account label_ket.value().
};


#endif //SDB3_1_SEQUENCE_H
