//
// Created by Garry Morrison on 12/09/2020.
//

#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include "Sequence.h"


Sequence::Sequence(const std::string& s) {
    Superposition tmp(s);
    seq.push_back(tmp);
}

Sequence::Sequence(const std::string& s, const double d) {
    Superposition tmp(s, d);
    seq.push_back(tmp);
}

Sequence::Sequence(const Ket& k) {
    Superposition tmp;  // why not use constructor here?
    tmp.add(k);
    seq.push_back(tmp);
}

Sequence::Sequence(const Superposition& sp) {
    Superposition tmp;  // why not use constructor here?
    tmp.add(sp);
    seq.push_back(tmp);
}

Sequence::Sequence(const Sequence& seq2) {
    // for (const auto &sp: seq2.seq) {
    //     seq.push_back(sp);
    // }
    seq.assign(seq2.seq.begin(), seq2.seq.end());  // Is this faster or slower than the for loop version??
}

bool Sequence::operator==(const Sequence &other) const {
    return seq == other.seq;
    /*if (seq.size() != other.seq.size()) { return false; }
    auto seq_iter = seq.cbegin();
    auto other_seq_iter = other.seq.cbegin();
    for (; seq_iter != seq.cend() && other_seq_iter != other.seq.cend(); ++seq_iter, ++other_seq_iter) {
        if (!(*seq_iter == *other_seq_iter)) {
            return false;
        }
    }
    return true;*/
}

const bool Sequence::is_ket() const {
    if (seq.empty()) { return true; }
    if (seq.size() == 1 ) {
        if (seq[0].is_ket()) { return true; }
    }
    return false;
}

const size_t Sequence::size() const {
    // if (seq.empty()) { return 0; }
    if (seq.size() == 1) {
        if (seq[0].size() == 0) {  // Do we need a similar check in Superposition::size() ?
            return 0;
        }
    }
    return seq.size();
}

const std::string Sequence::to_string() const {
    if (seq.empty()) { return "|>"; }
    std::string s;
    bool first_pass = true;
    for (const auto &sp: seq) {
        if (first_pass) {
            s += sp.to_string();
            first_pass = false;
        }
        else {
            s += " . " + sp.to_string();
        }
    }
    return s;
}

const std::string Sequence::to_string(const std::string &prefix) const {
    return prefix + this->to_string();
}

const std::string Sequence::readable_display() const {
    if (seq.empty()) { return ""; }
    std::string s;
    bool first_pass = true;
    for (const auto &sp: seq) {
        if (first_pass) {
            s += sp.readable_display();
            first_pass = false;
        }
        else {
            s += " . " + sp.readable_display();
        }
    }
    return s;
}


Ket Sequence::to_ket() const {
    if (seq.empty()) { return Ket(); }
    return seq[0].to_ket();
}

Superposition Sequence::to_sp() const {
    if (seq.empty() ) { return Superposition(); }
    // Superposition tmp = seq[0];  // I think we want instead for it to add all the superpositions together.
    Superposition tmp;
    for (const auto &sp: seq) {
        tmp.add(sp);
    }
    return tmp;
}

Sequence Sequence::to_seq() const {
    // Sequence tmp(*this);
    // return tmp;
    return *this;
}

void Sequence::add(const ulong idx) {
    if ( seq.empty() ) {
        Superposition tmp(idx);
        seq.push_back(tmp);
    }
    else {
        seq.back().add(idx);
    }
}

void Sequence::add(const Ket& k) {
    if ( seq.empty() ) {
        Superposition tmp(k);
        seq.push_back(tmp);
    }
    else {
        seq.back().add(k);
    }
}

void Sequence::add(const Superposition& sp) {
    if ( seq.empty() ) {
        seq.push_back(sp);
    }
    else {
        seq.back().add(sp);
    }
}

void Sequence::add(const Sequence& seq2) {
    if (seq2.seq.empty()) { return; }
    if (seq.empty() ) {
        seq = seq2.seq;
    } else {
        Sequence tail(seq2);
        Superposition head = tail.seq.front();
        tail.seq.erase(tail.seq.begin());
        seq.back().add(head);
        this->append(tail);
    }
}

void Sequence::multiply(const double d) {
    if (seq.empty()) { return; }
    for (auto &sp: seq) {
        sp.multiply(d);  // Yup. Seems to work.
    }
    // for (ulong idx = 0; idx < seq.size(); idx++) {  // tidy!
    //     seq[idx].multiply(d);
    // }
}

void Sequence::merge(const Sequence& seq2, const std::string& s) {
    if (seq2.seq.empty() ) { return; }             // buggy. Eg: |> __ |> Maybe we want this behaviour.
    if (seq.empty()) { this->add(seq2); return; }  // buggy. Eg: |> __ |>
    Superposition head = seq.back();
    Superposition tail = seq2.seq.front();
    head.merge(tail, s);
    seq.pop_back();
    seq.push_back(head);

    bool first_pass = true;
    for (auto const &sp : seq2.seq) {
        if (!first_pass) {
            seq.push_back(sp);
        }
        first_pass = false;
    }
}

void Sequence::merge(const Sequence& seq2) {
    this->merge(seq2, "");
}

void Sequence::process_infix(unsigned int infix_type, const Sequence &seq2) {
    // if (seq2.seq.empty() ) { return; }             // buggy. Eg: |> __ |> Maybe we want this behaviour.
    // if (seq.empty()) { this->add(seq2); return; }  // buggy. Eg: |> __ |>
    if (seq.empty() || seq2.seq.empty()) { return; }  // I don't think this is correct!
    Superposition head = seq.back();
    Superposition tail = seq2.seq.front();
    head.process_infix(infix_type, tail);
    seq.pop_back();
    seq.push_back(head);

    bool first_pass = true;
    for (auto const &sp : seq2.seq) {
        if (!first_pass) {
            seq.push_back(sp);
        }
        first_pass = false;
    }
}

void Sequence::insert_range(const Sequence &seq2) {
    // if (seq2.seq.empty() ) { return; }             // buggy. Eg: |> __ |> Maybe we want this behaviour.
    // if (seq.empty()) { this->add(seq2); return; }  // buggy. Eg: |> __ |>
    if (seq.empty() || seq2.seq.empty()) { return; }

    Superposition head = seq.back();
    Superposition tail = seq2.seq.front();
    head.insert_range(tail);
    seq.pop_back();
    seq.push_back(head);

    bool first_pass = true;
    for (auto const &sp : seq2.seq) {
        if (!first_pass) {
            seq.push_back(sp);
        }
        first_pass = false;
    }
}

void Sequence::append(const Ket& k) {
    Superposition tmp(k);
    seq.push_back(tmp);
}

void Sequence::append(const Superposition& sp) {
    // Superposition tmp(sp);
    seq.push_back(sp);
}

void Sequence::append(const Sequence& seq2) {
    for (const auto &sp: seq2.seq) {
        seq.push_back(sp);
    }
}

void Sequence::append(const std::string& s) {
    Superposition tmp(s);
    seq.push_back(tmp);  // Use std::move here?
}

void Sequence::append(const std::string& s, const double v) {
    Superposition tmp(s, v);
    seq.push_back(tmp);  // Use std::move here?
}


Superposition Sequence::get(ulong idx) const {
    if (idx >= seq.size() ) { return Superposition(); }
    return seq[idx];
}

void Sequence::set(ulong idx, const Superposition &sp) {
    if (idx >= seq.size()) { return; }
    seq[idx] = sp;
}

/*
BaseSequence* Sequence::b_add(BaseSequence* bSeq) {
    Sequence *seq = new Sequence(*this);
    seq->add(bSeq->to_seq());
    return seq;
}

BaseSequence* Sequence::b_append(BaseSequence* bSeq) {
    Sequence *seq = new Sequence(*this);
    seq->append(bSeq->to_seq());
    return seq;
}
*/

std::shared_ptr<BaseSequence> Sequence::b_add(std::shared_ptr<BaseSequence> bSeq) {
    Sequence tmp(*this);
    tmp.add(bSeq->to_seq());
    return std::make_shared<Sequence>(tmp);
}

std::shared_ptr<BaseSequence> Sequence::b_append(std::shared_ptr<BaseSequence> bSeq) {
    Sequence tmp(*this);
    tmp.append(bSeq->to_seq());
    return std::make_shared<Sequence>(tmp);
}


Sequence Sequence::shuffle() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.shuffle();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::sshuffle() const {
    Sequence result(*this);

    std::random_device rd; // code from here: https://en.cppreference.com/w/cpp/algorithm/random_shuffle
    std::mt19937 g(rd());
    std::shuffle(result.seq.begin(), result.seq.end(), g);

    return result;
}

Sequence Sequence::sreverse() const {
    Sequence result;
    for (auto it = seq.rbegin(); it != seq.rend(); ++it) {
        result.seq.push_back(*it);
    }
    return result;
}

Sequence Sequence::sdrop() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.drop();
        if (r.size() > 0) {
            result.seq.push_back(r);
        }
    }
    return result;
}

/*
Sequence Sequence::do_you_know() const {
    if (seq.empty()) { return Sequence("no"); }
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.do_you_know();
        result.seq.push_back(r);
    }
    return result;
}
*/

Sequence Sequence::do_you_know() const {
    if (seq.empty()) { return Sequence("no"); }
    if (seq.size() == 1 && seq[0].size() == 0) { return Sequence("no"); }
    return Sequence("yes");
}

Sequence Sequence::how_many() const {
    if (seq.empty()) { return Sequence("number: 0"); }  // What about case where seq[0].size() == 0?
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.how_many();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::measure_currency() const {
    if (seq.empty()) { return Sequence("number: 0"); }  // What about case where seq[0].size() == 0?
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.measure_currency();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::show_many() const {
    return Sequence("number: " + std::to_string(seq.size()));
}

Sequence Sequence::sidx() const {
    Sequence result;
    for (ulong idx = 0; idx < seq.size(); idx++) {
        result.append(std::to_string(idx + 1));
    }
    return result;
}

Sequence Sequence::rescale(const double t) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.rescale(t);
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::rescale() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.rescale();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::op_rescale0() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.rescale();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::op_rescale1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const {
    if (parameters.empty()) { return this->rescale(); }
    double t = parameters[0]->get_float();
    return this->rescale(t);
}

Sequence Sequence::normalize(const double t) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.normalize(t);
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::normalize() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.normalize();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::op_normalize0() const {
    return this->normalize();
}

Sequence Sequence::op_normalize1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const {
    if (parameters.empty()) { return this->normalize(); }
    double t = parameters[0]->get_float();
    return this->normalize(t);
}

Sequence Sequence::reverse() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.reverse();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::drop() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.drop();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::drop_below(const double t) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.drop_below(t);
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::op_drop_below1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const {
    if (parameters.empty()) { return *this; }
    double t = parameters[0]->get_float();
    return this->drop_below(t);
}

Sequence Sequence::op_drop_above1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const {
    if (parameters.empty()) { return *this; }
    double t = parameters[0]->get_float();
    return this->drop_above(t);
}

Sequence Sequence::op_pick1(const std::vector<std::shared_ptr<CompoundConstant> > &parameters) const {
    if (parameters.empty()) { return *this; }
    int n = parameters[0]->get_int();
    if (n < 0) { return this->pick(0); }
    return this->pick(n);
}


Sequence Sequence::drop_above(const double t) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.drop_above(t);
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::pick(const ulong n) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.pick(n);
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::pick_elt() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.pick_elt();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::weighted_pick_elt() const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition r = sp.weighted_pick_elt();
        result.seq.push_back(r);
    }
    return result;
}

Sequence Sequence::ket_sort() const {
    Sequence result;
    for (auto sp: seq) {
        sp.ket_sort();
        result.seq.push_back(std::move(sp));  // use std::move here? Is this correct?
    }
    return result;
}

Sequence Sequence::coeff_sort() const {
    Sequence result;
    for (auto sp: seq) {
        sp.coeff_sort();
        result.seq.push_back(std::move(sp));  // use std::move here? Is this correct?
    }
    return result;
}

Sequence Sequence::natural_sort() const {
    Sequence result;
    for (auto sp: seq) {
        sp.natural_sort();
        result.seq.push_back(std::move(sp));  // use std::move here? Is this correct?
    }
    return result;
}

Sequence Sequence::sort_by(ContextList &context, const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    Sequence result;
    for (auto sp: seq) {   // Should we use transform here instead?
        sp.sort_by(context, parameters);
        result.seq.push_back(std::move(sp));  // use std::move here? Is this correct?
    }
    return result;
}

Sequence Sequence::select(const int start, const int stop) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition tmp = sp.select(start, stop);
        result.seq.push_back(std::move(tmp));  // use std::move here? Is this correct?
    }
    return result;
}


// Need to test which is faster, for loop, vs std::transform.
/*
Sequence Sequence::select(const int start, const int stop) const {
    Sequence result;
    std::transform(seq.begin(), seq.end(), std::back_inserter(result.seq),
                   [=] (const Superposition &sp) { return sp.select(start, stop); });
    return result;
}
*/

Sequence Sequence::op_select2(const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    if (parameters.size() < 2) { return *this; }  // Return *this or Sequence()?
    int start = parameters[0]->get_int();
    int stop = parameters[1]->get_int();
    // std::cout << "Sequence::op_select2:" << std::endl;
    // std::cout << "    start: " << start << std::endl;
    // std::cout << "    stop: " << stop << std::endl;
    return this->select(start, stop);
}

Sequence Sequence::sselect(const int a, const int b) const {
    int start = a;
    int stop = b;
    if (start > 0) { start--; }
    // if (stop > 0) { stop--;}
    if (start < 0) { start += (int)seq.size(); }
    if (stop < 0 ) { stop += (int)seq.size() + 1; }
    start = std::max((int)0, start);  // For style reasons, use static_cast<int> instead?
    stop = std::max((int)0, stop);
    start = std::min((int)seq.size(), start);
    stop = std::min((int)seq.size(), stop);
    // std::cout << "sselect start: " << start << std::endl;
    // std::cout << "sselect stop: " << stop << std::endl;

    auto start_iter = seq.begin() + (size_t)start;
    auto stop_iter = seq.begin() + (size_t)stop;
    Sequence result;
    result.seq.insert(result.seq.begin(), start_iter, stop_iter);
    return result;
}

Sequence Sequence::op_sselect2(const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    if (parameters.size() < 2) { return *this; }  // Return *this or Sequence()?
    int start = parameters[0]->get_int();
    int stop = parameters[1]->get_int();
    return this->sselect(start, stop);
}

Sequence Sequence::op_srotate_left() const {
    Sequence result(*this);
    std::rotate(result.seq.begin(), result.seq.begin() + 1, result.seq.end());
    return result;
}

Sequence Sequence::op_srotate_right() const {
    Sequence result(*this);
    std::rotate(result.seq.rbegin(), result.seq.rbegin() + 1, result.seq.rend());
    return result;
}

Sequence Sequence::op_srotate_left1(const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    Sequence result(*this);
    if (parameters.empty()) { return result; }
    int pos = parameters[0]->get_int();
    // if (pos < 0 || pos >= result.seq.size()) { return result; }  // Should we instead return an empty ket?
    // if (pos < 0) { pos = 0; }
    pos %= result.seq.size();
    std::rotate(result.seq.begin(), result.seq.begin() + pos, result.seq.end());
    return result;
}

Sequence Sequence::op_srotate_right1(const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    Sequence result(*this);
    if (parameters.empty()) { return result; }
    int pos = parameters[0]->get_int();
    // if (pos < 0 || pos >= result.seq.size()) { return result; }  // Should we instead return an empty ket?
    // if (pos < 0) { pos = 0; }
    pos %= result.seq.size();
    std::rotate(result.seq.rbegin(), result.seq.rbegin() + pos, result.seq.rend());
    return result;
}

Sequence Sequence::op_rotate_left() const {
    Sequence result(*this);
    std::transform(result.seq.begin(), result.seq.end(), result.seq.begin(),
                   [](const Superposition &sp) -> Superposition { return sp.rotate_left(1); });  // Testing a new way to transforming sequences. Instead of using for loops and push_back.
    return result;
}

Sequence Sequence::op_rotate_right() const {
    Sequence result(*this);
    std::transform(result.seq.begin(), result.seq.end(), result.seq.begin(),
                   [](const Superposition &sp) -> Superposition { return sp.rotate_right(1); });  // Testing a new way to transforming sequences.
    return result;
}

Sequence Sequence::op_rotate_left1(const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    Sequence result(*this);
    if (parameters.empty()) { return result; }
    int n = parameters[0]->get_int();
    std::transform(result.seq.begin(), result.seq.end(), result.seq.begin(),
                   [n](const Superposition &sp) -> Superposition { return sp.rotate_left(n); });  // Testing a new way to transforming sequences.
    return result;
}

Sequence Sequence::op_rotate_right1(const std::vector<std::shared_ptr<CompoundConstant>> &parameters) const {
    Sequence result(*this);
    if (parameters.empty()) { return result; }
    int n = parameters[0]->get_int();
    std::transform(result.seq.begin(), result.seq.end(), result.seq.begin(),
                   [n](const Superposition &sp) -> Superposition { return sp.rotate_right(n); });  // Testing a new way to transforming sequences.
    return result;
}


Sequence Sequence::apply_sigmoid(const std::function<double(double)> &sigmoid) const {
    Sequence result;
    for (const auto &sp: seq) {
        // Superposition tmp = sp.apply_sigmoid(sigmoid); // not sure which approach is better.
        // result.seq.push_back(tmp);
        result.seq.push_back(sp.apply_sigmoid(sigmoid));
    }
    return result;
}

Sequence Sequence::apply_sigmoid(const std::function<double(double,double)> &sigmoid, const double t) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition tmp = sp.apply_sigmoid(sigmoid, t);
        result.seq.push_back(tmp);
    }
    return result;
}

Sequence Sequence::apply_sigmoid(const std::function<double(double,double,double)> &sigmoid, const double t1, const double t2) const {
    Sequence result;
    for (const auto &sp: seq) {
        Superposition tmp = sp.apply_sigmoid(sigmoid, t1, t2);
        result.seq.push_back(tmp);
    }
    return result;
}

Sequence Sequence::apply_ket_fn(const std::function<Sequence(const Ket&)> &fn) const {
    if (seq.empty() ) {
        Ket k;
        return fn(k);
    }
    Sequence result;
    for (const auto &sp: seq) {
        Sequence tmp;
        if (sp.size() == 0 ) {
            Ket k;
            tmp.add(fn(k));
        } else {
            for (const auto k: sp) {
                Sequence tmp2 = fn(k);
                tmp.add(tmp2);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence Sequence::apply_sp_fn(const std::function<Sequence(const Superposition&)> &fn) const {
    Sequence result;
    for (const auto &sp: seq) {
        Sequence tmp = fn(sp);
        // result.append(tmp);  // not sure if it should be result.add(tmp). Probably.
        result.add(tmp);
    }
    return result;
}

Sequence Sequence::apply_context_ket_fn(const std::function<Sequence(const Ket &, ContextList &)> &fn, ContextList &context) const {  // Test this code!
    if (seq.empty() ) {
        Ket k;
        return fn(k, context);
    }
    Sequence result;
    for (const auto &sp: seq) {
        Sequence tmp;
        if (sp.size() == 0 ) {
            Ket k;
            tmp.add(fn(k, context));
        } else {
            for (const auto k: sp) {
                Sequence tmp2 = fn(k, context);
                tmp.add(tmp2);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence Sequence::apply_compound_sigmoid(const std::function<double(double, const std::vector<std::shared_ptr<CompoundConstant> >&)> sigmoid, std::vector<std::shared_ptr<CompoundConstant> > parameters) const {
    Sequence result;
    for (const auto &sp: seq) {
        //Superposition tmp = sp.apply_sigmoid(sigmoid, t);
        Superposition tmp = sp.apply_compound_sigmoid(sigmoid, parameters);
        result.seq.push_back(tmp);
    }
    return result;
}

Sequence Sequence::apply_compound_ket_fn(const std::function<Sequence(Ket, const std::vector<std::shared_ptr<CompoundConstant> >&)> fn, std::vector<std::shared_ptr<CompoundConstant> > parameters) const {
    if (seq.empty() ) {
        Ket k;
        return fn(k, parameters);
    }
    Sequence result;
    for (const auto &sp: seq) {
        Sequence tmp;
        if (sp.size() == 0 ) {
            Ket k;
            tmp.add(fn(k, parameters));
        } else {
            for (const auto k: sp) {
                Sequence tmp2 = fn(k, parameters);
                tmp.add(tmp2);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence Sequence::apply_compound_sp_fn(const std::function<Sequence(const Superposition&, const std::vector<std::shared_ptr<CompoundConstant> >&)> fn, std::vector<std::shared_ptr<CompoundConstant> > parameters) const {
    Sequence result;
    for (const auto &sp: seq) {  // What do we want to happen when seq is empty?
        Sequence tmp = fn(sp, parameters);
        // result.append(tmp);  // not sure if it should be result.add(tmp). Probably.
        result.add(tmp);
    }
    return result;
}


void Sequence::dummy1(std::shared_ptr<CompoundConstant> &constant) {
    std::cout << "dummy1 constant: " << constant->to_string() << std::endl;
    return;
}

void Sequence::dummy2(std::vector<std::shared_ptr<CompoundConstant> > &const_vec) {
    for (const auto c: const_vec) {
        std::cout << "dummy2 constant: " << c->to_string() << std::endl;
    }
    return;
}

Sequence Sequence::Compile(ContextList &context, const Ket &label_ket) const {
    Sequence tmp(*this);
    tmp.multiply(label_ket.value());  // Is this correct?
    return tmp;
}
