//
// Created by Garry Morrison on 12/09/2020.
//

#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <chrono>
#include "Superposition.h"
#include "../Function/misc.h"
#include "../Function/NaturalSort.h"
#include "../Operator/SimpleOperator.h"
#include "../OperatorLibrary/FunctionOperatorLibrary.h"
#include "../Sequence/OperatorWithSequence.h"

Superposition::Superposition(const ulong idx) {
    if (ket_map.get_idx("") == idx) {return; }

    sp[idx] = 1.0;
    sort_order.push_back(idx);
}

Superposition::Superposition(const std::string& s) {
    if (s.empty()) {return; }

    ulong idx = ket_map.get_idx(s);
    sp[idx] = 1.0;
    sort_order.push_back(idx);
}

Superposition::Superposition(const std::string& s, const double v) {
    if (s.empty()) {return; }

    ulong idx = ket_map.get_idx(s);
    sp[idx] = v;
    sort_order.push_back(idx);
}

Superposition::Superposition(const ulong idx, const double v) {
    if (ket_map.get_idx("") == idx) {return; }

    sp[idx] = v;
    sort_order.push_back(idx);
}

Superposition::Superposition(Ket k) {
    ulong idx = k.label_idx();
    if (ket_map.get_idx("") == idx) {return; }

    sp[idx] = k.value();
    sort_order.push_back(idx);
}

/*
Superposition::Superposition(const Superposition &sp1) {
    for (const auto idx : sp1.sort_order) {
        double value = sp1.sp.at(idx);
        sp[idx] = value;
        sort_order.push_back(idx);
    }
}
*/

Superposition::Superposition(const std::vector<ulong> &vec) {
    ulong identity_idx = ket_map.get_idx("");
    for (const ulong idx: vec) {
        if (idx == identity_idx) { continue; }
        if (sp.find(idx) != sp.end()) {
            // sp[idx]++;
            sp[idx] += 1.0f;
        } else {
            sp[idx] = 1.0;
            sort_order.push_back(idx);
        }
    }
}

bool Superposition::operator==(const Superposition &other) const {
    if (sort_order.size() != other.sort_order.size()) { return false; }
    for (const ulong &idx: sort_order) {
        auto other_iter = other.sp.find(idx);
        if (other_iter == other.sp.end()) {
            return false;
        }
        double value = sp.at(idx);  // We can't use sp[idx] because of 'const'.
        if (!double_eq(value, other_iter->second)) {
            return false;
        }
    }
    return true;
}


const size_t Superposition::size() const {
    return sort_order.size();
}

const std::string Superposition::to_string() const {
    if (sp.empty()) { return "|>"; }

    std::string s;
    std::string label;
    std::string value_string;
    std::string sign;
    bool first_pass = true;
    for (const auto idx: sort_order) {
        auto label = ket_map.get_str(idx);
        auto value = sp.at(idx);
        sign = " + ";
        if (double_eq(fabs(value), 1.0)) {
            value_string = "";
            if (value < 0) { sign = " - "; }
        }
        else {
            if (double_eq(value, round(value))) {
                value_string = std::to_string((int)round(fabs(value)));
            }
            else {
                value_string = std::to_string(fabs(value));
            }
            if (value < 0) { sign = " - "; }
        }

        if (first_pass && value >= 0) {
            s += value_string + "|" + label + ">";
            first_pass = false;
        }
        else if (first_pass && value < 0) {
            s += "- " + value_string + "|" + label + ">";
            first_pass = false;
        }
        else {
            s += sign + value_string + "|" + label + ">";
        }
    }
    return s;
}

const std::string Superposition::to_string(const std::string &prefix) const {
    return prefix + this->to_string();
}

const std::string Superposition::readable_display() const {
    if (sp.empty()) { return ""; }

    std::string s;
    std::string label;
    std::string value_string;
    std::string sign;
    bool first_pass = true;
    for (const auto idx: sort_order) {
        auto label = ket_map.get_str(idx);
        auto value = sp.at(idx);
        sign = " + ";
        if (double_eq(fabs(value), 1.0)) {
            value_string = "";
            if (value < 0) { sign = " - "; }
        }
        else {
            // if (double_eq(value, round(value))) {
            //     value_string = std::to_string((int)round(fabs(value))) + " ";
            // }
            // else {
            //     value_string = std::to_string(fabs(value)) + " ";
            // }
            value_string = float_to_int(fabs(value), default_decimal_places) + " "; // improve!
            if (value < 0) { sign = " - "; }
        }

        if (first_pass && value >= 0) {
            s += value_string + label;
            first_pass = false;
        }
        else if (first_pass && value < 0) {
            s += "- " + value_string + label;
            first_pass = false;
        }
        else {
            s += sign + value_string + label;
        }
    }
    return s;
}


Ket Superposition::to_ket() const {
    if (sp.empty()) { return Ket(); }
    ulong op_idx = sort_order[0];
    double value = sp.at(op_idx);
    return Ket(op_idx, value);
}

Superposition Superposition::to_sp() const {
    return Superposition(*this);
}

Sequence Superposition::to_seq() const {
    return Sequence(*this);
}

/*
BaseSequence* Superposition::b_add(BaseSequence* bSeq) {
    Sequence *seq = new Sequence(*this);
    seq->add(bSeq->to_seq());
    return seq;
}

BaseSequence* Superposition::b_append(BaseSequence* bSeq) {
    Sequence *seq = new Sequence(*this);
    seq->append(bSeq->to_seq());
    return seq;
}
*/

std::shared_ptr<BaseSequence> Superposition::b_add(std::shared_ptr<BaseSequence> bSeq) {
    Sequence tmp(*this);
    tmp.add(bSeq->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(tmp);
    return result;
}

std::shared_ptr<BaseSequence> Superposition::b_append(std::shared_ptr<BaseSequence> bSeq) {
    Sequence tmp(*this);
    tmp.append(bSeq->to_seq());
    std::shared_ptr<BaseSequence> result = std::make_shared<Sequence>(tmp);
    return result;
}


void Superposition::add(const ulong idx) {
    if (ket_map.get_idx("") == idx) {return; }  // maybe we should make idx for "" = 0, as an optimization
    // especially since we use Superposition::add() a lot!
    if (sp.find(idx) != sp.end()) {
        sp[idx] += 1.0;  // Maybe use: sp[idx]++ ?
    }
    else {
        sp[idx] = 1.0;
        sort_order.push_back(idx);
    }
    return;
}

void Superposition::add(const ulong idx, const double v) {
    if (ket_map.get_idx("") == idx) {return; }

    if (sp.find(idx) != sp.end()) {
        sp[idx] += v;
    }
    else {
        sp[idx] = v;
        sort_order.push_back(idx);
    }
    return;
}


void Superposition::add(const std::string& s) {
    ulong idx = ket_map.get_idx(s);
    if (ket_map.get_idx("") == idx) {return; }

    if (sp.find(idx) != sp.end()) {
        sp[idx] += 1.0;
    }
    else {
        sp[idx] = 1.0;
        sort_order.push_back(idx);
    }
    return;
}

void Superposition::add(const std::string& s, const double v) {
    ulong idx = ket_map.get_idx(s);
    if (ket_map.get_idx("") == idx) {return; }

    if (sp.find(idx) != sp.end()) {
        sp[idx] += v;
    }
    else {
        sp[idx] = v;
        sort_order.push_back(idx);
    }
    return;
}

// const bugs out for now:
// void Superposition::add(const Ket& a) {
void Superposition::add(Ket& a) {
    ulong idx = a.label_idx();
    double v = a.value();

    if (ket_map.get_idx("") == idx) {return; }  // |> is the identity element for superpositions. sp + |> == |> + sp == sp

    if (sp.find(idx) != sp.end()) {
        sp[idx] += v;
    }
    else {
        sp[idx] = v;
        sort_order.push_back(idx);
    }
    return;
}

void Superposition::add(const Superposition& a) {
    ulong identity_idx = ket_map.get_idx("");

    for (const ulong idx: a.sort_order) {
        if (idx == identity_idx) { continue; }
        if (sp.find(idx) != sp.end()) {
            sp[idx] += a.sp.at(idx);
        }
        else {
            sp[idx] = a.sp.at(idx);
            sort_order.push_back(idx);
        }
    }
}

void Superposition::add(const std::vector<ulong> &vec) {
    ulong identity_idx = ket_map.get_idx("");
    for (const ulong idx: vec) {
        if (idx == identity_idx) { continue; }
        if (sp.find(idx) != sp.end()) {
            // sp[idx]++; // Not certain this is correct for doubles.
            sp[idx] += 1.0;
        } else {
            sp[idx] = 1.0;
            sort_order.push_back(idx);
        }
    }
}

void Superposition::multiply(const double d) {
    for (ulong idx: sort_order) {
        sp[idx] *= d;
    }
}

struct CompareCoeffStruct {
    CompareCoeffStruct(std::unordered_map<ulong, double> *sp) { this->sp = sp; }
//    bool operator () (ulong a, ulong b) { return sp->at(a) < sp->at(b); }
    bool operator () (ulong a, ulong b) { return sp->at(a) > sp->at(b); }
    std::unordered_map<ulong, double> *sp;
};

void Superposition::coeff_sort() {
    std::sort(sort_order.begin(), sort_order.end(), [this] (ulong a, ulong b) { return this->sp[a] > this->sp[b]; });
}

void Superposition::ket_sort() {
    std::sort(sort_order.begin(), sort_order.end(), [] (ulong a, ulong b) { return ket_map.get_str(a) < ket_map.get_str(b); });
}

void Superposition::natural_sort() {
    std::sort(sort_order.begin(), sort_order.end(), [] (ulong a, ulong b) {
        if (strnatcasecmp(ket_map.get_str(a).c_str(), ket_map.get_str(b).c_str()) == -1) { return true; }
        return false;
    });
}

void Superposition::sort_by(ContextList &context, const std::vector<std::shared_ptr<CompoundConstant>> &parameters) {
    if (parameters.empty()) { return; }
    SimpleOperator op = parameters[0]->get_operator();
    std::unordered_map<ulong, std::string> sort_by_map;
    // std::map<ulong, std::string> sort_by_map;  // Slower than unordered_map.
    for (const ulong idx: sort_order) {
        Ket k(idx, sp[idx]);
        std::string sort_by_str = op.Compile(context, k.to_seq()).to_ket().label();
        sort_by_map[idx] = sort_by_str;
    }
    std::sort(sort_order.begin(), sort_order.end(), [&sort_by_map] (ulong a, ulong b) {
        if (strnatcasecmp(sort_by_map[a].c_str(), sort_by_map[b].c_str()) == -1) { return true; } // Work out why we can't use [a] instead of .at(a)
        return false;
    });
}

Superposition Superposition::select(const int a, const int b) const {
    int start = a;
    int stop = b;
    if (start > 0) { start--; }
    // if (stop > 0) { stop--;}
    if (start < 0) { start += (int)sort_order.size(); }
    if (stop < 0 ) { stop += (int)sort_order.size() + 1; }
    start = std::max((int)0, start);  // For style reasons, use static_cast<int> instead?
    stop = std::max((int)0, stop);
    start = std::min((int)sort_order.size(), start);
    stop = std::min((int)sort_order.size(), stop);
    // std::cout << "Superposition::select:" << std::endl;
    // std::cout << "    start: " << start << std::endl;
    // std::cout << "    stop: " << stop << std::endl;

    auto start_iter = sort_order.cbegin() + (size_t)start;
    auto stop_iter = sort_order.cbegin() + (size_t)stop;
    Superposition result;
    result.sort_order.insert(result.sort_order.begin(), start_iter, stop_iter);
    for (auto idx: result.sort_order) {
        // result.sp[idx] = sp[idx];  // If we make this function const, then need to use sp.at(idx) instead.
        result.sp[idx] = sp.at(idx);  // Currently bugs out if stop is too large. Fix!
    }
    return result;
}

void Superposition::merge(const Superposition& sp2, const std::string& s) {  // Can we tidy this function up?
    if (sp2.sort_order.empty() ) { return; }
    if (sort_order.empty() ) { this->add(sp2); return; }

    ulong head_idx = sort_order.back();
    double head_value = sp[head_idx];
    sp.erase(head_idx);
    sort_order.pop_back();
    ulong tail_idx = sp2.sort_order.front();
    double tail_value = sp2.sp.at(tail_idx); // does this work: sp2.sp[tail_idx] ?
    std::string s2 = ket_map.get_str(head_idx) + s + ket_map.get_str(tail_idx);
    ulong new_idx = ket_map.get_idx(s2);
    double new_value = head_value * tail_value;
    this->add(new_idx, new_value);

    bool first_pass = true;
    for (const auto idx: sp2.sort_order) {
        if (!first_pass) {
            this->add(idx, sp2.sp.at(idx));
        }
        first_pass = false;
    }
}

void Superposition::merge(const Superposition& sp2) {
    this->merge(sp2, "");
}

void Superposition::process_infix(unsigned int infix_type, const Superposition &sp2) {
    // if (sort_order.empty() || sp2.sort_order.empty()) { return; }
    if (sort_order.empty()) { return; }
    // if (sp2.sort_order.empty()) { sort_order.clear(); sp.clear(); }

    if (sp2.sort_order.empty()) {  // Not quite correct. Eg, Sequence() vs Sequence("").
        ulong head_idx = sort_order.back();
        sp.erase(head_idx);
        sort_order.pop_back();
    } else {
        ulong head_idx = sort_order.back();
        double head_value = sp[head_idx];
        sp.erase(head_idx);
        sort_order.pop_back();
        ulong tail_idx = sp2.sort_order.front();
        double tail_value = sp2.sp.at(tail_idx); // does this work: sp2.sp[tail_idx] ?
        // std::string s2 = ket_map.get_str(head_idx) + s + ket_map.get_str(tail_idx);
        // ulong new_idx = ket_map.get_idx(s2);

        ulong new_idx = process_infix_compile(head_idx, infix_type, tail_idx);
        double new_value = head_value * tail_value;
        this->add(new_idx, new_value);

        bool first_pass = true;
        for (const auto idx: sp2.sort_order) {
            if (!first_pass) {
                this->add(idx, sp2.sp.at(idx));
            }
            first_pass = false;
        }
    }
}

void Superposition::insert_range(const Superposition &sp2) {
    // if (sp2.sort_order.empty() ) { return; }
    // if (sort_order.empty() ) { this->add(sp2); return; }  // Not 100% sure on what we want to do here.
    if (sort_order.empty()) { return; }

    if (sp2.sort_order.empty()) {
        ulong head_idx = sort_order.back();
        sp.erase(head_idx);
        sort_order.pop_back();
    } else {
        ulong head_idx = sort_order.back();
        double head_value = sp[head_idx];
        sp.erase(head_idx);
        sort_order.pop_back();
        ulong tail_idx = sp2.sort_order.front();
        double tail_value = sp2.sp.at(tail_idx); // does this work: sp2.sp[tail_idx] ?

        Superposition our_range = range2(head_idx, tail_idx);

        double new_value = head_value * tail_value;
        our_range.multiply(new_value);
        this->add(our_range);

        bool first_pass = true;
        for (const auto idx: sp2.sort_order) {
            if (!first_pass) {
                this->add(idx, sp2.sp.at(idx));
            }
            first_pass = false;
        }
    }
}

Superposition Superposition::operator+(Ket& b) {
    Superposition tmp;
    tmp.add(*this);
    tmp.add(b);
    return tmp;
}

Superposition Superposition::operator+(Superposition& sp) {
    Superposition tmp;
    tmp.add(*this);
    tmp.add(sp);
    return tmp;
}

// SuperpositionIter:
Ket Superposition::get(ulong idx) const {
    if (idx >= sort_order.size() ) { return Ket(); }
    ulong sp_idx = sort_order[idx];
    double value = sp.at(sp_idx);
    return Ket(sp_idx, value);
}

SuperpositionIter Superposition::begin () const {
    return SuperpositionIter(this, 0);
}

SuperpositionIter Superposition::end () const {
    return SuperpositionIter(this, this->sort_order.size());
}


// maybe more efficient to use:
// https://codereview.stackexchange.com/questions/39001/fisher-yates-modern-shuffle-algorithm
Superposition Superposition::shuffle() const {
    Superposition result(*this);

    std::random_device rd; // code from here: https://en.cppreference.com/w/cpp/algorithm/random_shuffle
    std::mt19937 g(rd());
    std::shuffle(result.sort_order.begin(), result.sort_order.end(), g);

    return result;
}

double Superposition::find_value(const Ket &k) const {
    ulong idx = k.label_idx();
    if (sp.find(idx) == sp.end()) { return 0; }
    return sp.at(idx);  // swap to sp[idx]? Nope. Doesn't seem to work.
    // return sp[idx];
}

Ket Superposition::do_you_know() const {
    if (sort_order.empty()) { return Ket("no"); }
    return Ket("yes");
}

Ket Superposition::how_many() const {
    ulong count = sort_order.size();
    Ket result("number: " + std::to_string(count));
    return result;
}


Ket Superposition::measure_currency() const {
    double sum = 0;
    for (const auto idx : sort_order) {
        sum += sp.at(idx);
    }
    return Ket("number: " + float_to_int(sum, default_decimal_places));
}

bool compare(const std::pair<ulong, double>&a, const std::pair<ulong, double>&b) {
    return a.second < b.second;
}

double Superposition::find_min_coeff() const {
    return std::min_element(sp.begin(), sp.end(), compare)->second;
}

double Superposition::find_max_coeff() const { // what happens if sp is empty?
    return std::max_element(sp.begin(), sp.end(), compare)->second;
}

Superposition Superposition::rescale(const double t) const {
    if (sort_order.empty()) { return Superposition(); }
    double the_max = this->find_max_coeff();
    if (the_max == 0) { return *this; }
    Superposition result;
    for (const auto idx : sort_order) {
        double value = sp.at(idx);
        result.sp[idx] = t * value / the_max;
        result.sort_order.push_back(idx);
    }
    return result;
}

Superposition Superposition::rescale() const {
    return this->rescale(1);
}

Superposition Superposition::normalize() const {
    return this->normalize(1);
}

Superposition Superposition::normalize(const double t) const {
    if (sort_order.empty()) { return Superposition(); }
    double sum = 0;
    for (const auto idx : sort_order) {
        double value = sp.at(idx);
        sum += value;
    }
    if (sum == 0) { return *this; }
    Superposition result;
    for (const auto idx : sort_order) {
        double value = sp.at(idx);
        result.sp[idx] = t * value / sum;
        result.sort_order.push_back(idx);
    }
    return result;
}

Superposition Superposition::reverse() const {
    Superposition result;
    result.sort_order.reserve(sort_order.size());
    for ( auto it = sort_order.rbegin(); it != sort_order.rend(); ++it) {
        double value = sp.at(*it);
        result.sp[*it] = value;
        result.sort_order.push_back(*it);
    }
    return result;
}

Superposition Superposition::drop() const {
    Superposition result;
    for (const auto idx : sort_order) {
        double value = sp.at(idx);
        if (value > 0) {
            result.sp[idx] = value;
            result.sort_order.push_back(idx);
        }
    }
    return result;
}

Superposition Superposition::drop_below(const double t) const {
    Superposition result;
    for (const auto idx : sort_order) {
        double value = sp.at(idx);
        if (value >= t) {
            result.sp[idx] = value;
            result.sort_order.push_back(idx);
        }
    }
    return result;
}

Superposition Superposition::drop_above(const double t) const {
    Superposition result;
    for (const auto idx : sort_order) {
        double value = sp.at(idx);
        if (value <= t) {
            result.sp[idx] = value;
            result.sort_order.push_back(idx);
        }
    }
    return result;
}

// code from here:
// https://ideone.com/3A3cv
// https://stackoverflow.com/questions/9345087/choose-m-elements-randomly-from-a-vector-containing-n-elements
// https://codereview.stackexchange.com/questions/39001/fisher-yates-modern-shuffle-algorithm
template<class bidiiter>
bidiiter random_unique(bidiiter begin, bidiiter end, size_t num_random) {
    // seed the RNG:
    std::random_device rd;
    std::mt19937 mt(rd());

    size_t left = std::distance(begin, end);
    while (num_random--) {
        bidiiter r = begin;
        std::uniform_int_distribution<> dis(0, left - 1);
        const int randomIndex = dis(mt);
        std::advance(r, randomIndex);
//        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

Superposition Superposition::pick(const ulong n) const {
    std::vector<ulong> sort_order1 = sort_order;
    size_t min_size = std::min(sort_order.size(), (size_t)n);
    random_unique(sort_order1.begin(), sort_order1.end(), min_size);
    Superposition result;
    for (ulong k = 0; k < min_size; k++) {
        ulong idx = sort_order1[k];
        double value = sp.at(idx);
        result.add(idx, value);
    }
    return result;
}

Ket Superposition::pick_elt() const {
    if (this->size() == 0) { return Ket(); }
    if (this->sort_order[0] == ket_map.get_idx("")) { return Ket(); }
    std::random_device rd;  // is this correct to re-seed on every invoke?
    std::mt19937 eng(rd()); // code from here: https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    std::uniform_int_distribution<> distr(0, sort_order.size() - 1);
    ulong pos = distr(eng);
    ulong idx = sort_order[pos];
    double value = sp.at(idx);
    return Ket(idx, value);
}

Ket Superposition::weighted_pick_elt() const {  // Not happy with the results on this. I suspect there is a bug in here somewhere!
    Superposition sp1 = this->drop();
    if (sp1.size() == 0) { return Ket(); }
    if (sp1.sort_order[0] == ket_map.get_idx("")) { return Ket(); }
    double sum = 0;
    for (const auto idx : sp1.sort_order) {
        double value = sp1.sp.at(idx);
        sum += value;
    }  // sum should be > 0
    std::random_device rd;  // is this correct to re-seed on every invoke?
    std::mt19937 gen(rd()); // code from here: https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    // int m_seed {static_cast<int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};  // Testing a better seed method than std::random_device.
    // std::mt19937 gen(m_seed);                                                                             // Nope, seems a little worse.
    // std::uniform_int_distribution<> distr(0, sum);
    std::uniform_real_distribution<double> distr(0, sum);
    double r = distr(gen);
    double upto = 0;
    for (const auto k : sp1 ) { // maybe iterate using sort_order instead as an optimization?
        double w = k.value();
        if (upto + w > r) { return k; }
        upto += w;
    }
    // assert(false); // we shouldn't get here if everything is working correctly
    return Ket();  // We keep hitting assert(false), not sure yet why. Fix!
}

Superposition Superposition::rotate_left(const int n) const {  // We use int for n instead of ulong because parameters[0] returns int not ulong.
    Superposition result(*this);
    ulong pos = n % result.sort_order.size();
    std::rotate(result.sort_order.begin(), result.sort_order.begin() + pos, result.sort_order.end());
    return result;
}

Superposition Superposition::rotate_right(const int n) const {  // We use int for n instead of ulong because parameters[0] returns int not ulong.
    Superposition result(*this);
    ulong pos = n % result.sort_order.size();
    std::rotate(result.sort_order.rbegin(), result.sort_order.rbegin() + pos, result.sort_order.rend());
    return result;
}


Superposition Superposition::apply_sigmoid(const std::function<double(double)> &sigmoid) const {
    Superposition result;
    for (auto idx : sort_order) {
        double value = sp.at(idx);
        double sig_value = sigmoid(value);
        result.add(idx, sig_value);
    }
    return result;
}

Superposition Superposition::apply_sigmoid(const std::function<double(double,double)> &sigmoid, const double t) const {
    Superposition result;
    for (auto idx : sort_order) {
        double value = sp.at(idx);
        double sig_value = sigmoid(value, t);
        result.add(idx, sig_value);
    }
    return result;
}

Superposition Superposition::apply_sigmoid(const std::function<double(double,double,double)> &sigmoid, const double t1, const double t2) const {
    Superposition result;
    for (auto idx : sort_order) {
        double value = sp.at(idx);
        double sig_value = sigmoid(value, t1, t2);
        result.add(idx, sig_value);
    }
    return result;
}

Superposition Superposition::apply_compound_sigmoid(const std::function<double(double, const std::vector<std::shared_ptr<CompoundConstant> >)> &sigmoid, std::vector<std::shared_ptr<CompoundConstant> > parameters) const {
    Superposition result;
    for (auto idx : sort_order) {
        double value = sp.at(idx);
        double sig_value = sigmoid(value, parameters);
        result.add(idx, sig_value);
    }
    return result;
}


Sequence Superposition::Compile(ContextList& context) const {
    return this->to_seq();
}

Sequence Superposition::Compile(ContextList& context, const Ket& label_ket) const {
    // return this->to_seq();
    Sequence tmp(*this);
    tmp.multiply(label_ket.value());
    return tmp;
}

Sequence Superposition::Compile(ContextList &context, const Ket& label_ket, const Ket& multi_label_ket) const {
    return this->to_seq();
}

Sequence Superposition::Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const {
    return this->to_seq();
}


ulong Superposition::old_process_infix_compile(ulong idx1, unsigned int infix_type, ulong idx2) const {
    // std::cout << "  idx1: " << ket_map.get_str(idx1) << "\n";
    // std::cout << "  idx2: " << ket_map.get_str(idx2) << "\n";
    ulong empty_idx = ket_map.get_idx("");
    ulong yes_idx = ket_map.get_idx("yes");
    ulong no_idx = ket_map.get_idx("no");

    switch (infix_type) {
        case OPGREATEREQUAL:
        case OPGREATER:
        case OPLESSEQUAL:
        case OPLESS: {
            try {
                auto one_idx_vec = ket_map.get_split_idx(idx1);  // Handle more than just kets later! Ie, arithmetic over superpositions and sequences.
                auto two_idx_vec = ket_map.get_split_idx(idx2);
                if (one_idx_vec.empty() || two_idx_vec.empty()) { return empty_idx; }
                long double x = std::stold(ket_map.get_str(one_idx_vec.back()));
                long double y = std::stold(ket_map.get_str(two_idx_vec.back()));
                one_idx_vec.pop_back();
                two_idx_vec.pop_back();
                if (one_idx_vec != two_idx_vec) { return empty_idx; } // Do we want this check here? Ie, checking that the categories are equal?
                switch (infix_type) {
                    case OPGREATEREQUAL:
                        if (x >= y) { return yes_idx; }
                        return no_idx;
                    case OPGREATER:
                        if (x > y) { return yes_idx; }
                        return no_idx;
                    case OPLESSEQUAL:
                        if (x <= y) { return yes_idx; }
                        return no_idx;
                    case OPLESS:
                        if (x < y) { return yes_idx; }
                        return no_idx;
                    default:
                        return empty_idx;
                }
            } catch (const std::invalid_argument& e) {
                return empty_idx;
            }
        }
        case OPAND: {
            if (idx1 == yes_idx && idx2 == yes_idx) { return yes_idx; }
            return no_idx;
        }
        case OPOR: {
            if (idx1 == yes_idx || idx2 == yes_idx) { return yes_idx; }
            return no_idx;
        }
        case OPPLUS:
        case OPMINUS:
        case OPMULT:
        case OPDIV:
        case OPMOD:
        case OPARITHPOWER: {
            // std::cout << "    arithmetic section:\n";
            auto one_idx_vec = ket_map.get_split_idx(idx1);  // Handle more than just kets later! Ie, arithmetic over superpositions and sequences.
            auto two_idx_vec = ket_map.get_split_idx(idx2);
            if (one_idx_vec.empty() || two_idx_vec.empty()) { return empty_idx; }
            try {
                long double x = std::stold(ket_map.get_str(one_idx_vec.back()));
                long double y = std::stold(ket_map.get_str(two_idx_vec.back()));
                one_idx_vec.pop_back();
                two_idx_vec.pop_back();

                std::string label;
                if (!one_idx_vec.empty() && two_idx_vec.empty()) {
                    label = ket_map.get_str(one_idx_vec) + ": ";
                } else if (one_idx_vec.empty() && !two_idx_vec.empty()) {
                    label = ket_map.get_str(two_idx_vec) + ": ";
                } else if (one_idx_vec == two_idx_vec) {
                    if (!one_idx_vec.empty()) {
                        label = ket_map.get_str(one_idx_vec) + ": ";
                    }
                } else {
                    return empty_idx;
                }

                long double value;

                switch (infix_type) {
                    case OPPLUS : {
                        value = x + y;
                        break;
                    }
                    case OPMINUS : {
                        value = x - y;
                        break;
                    }
                    case OPMULT : {
                        value = x * y;
                        break;
                    }
                    case OPDIV : {
                        value = x / y;
                        break;
                    } // check for div by zero here!
                    case OPMOD : {
                        value = static_cast<long long>(x) % static_cast<long long>(y);
                        break;
                    }
                    case OPARITHPOWER : {
                        value = pow(x, y);
                        break;
                    }
                    default:
                        return empty_idx;
                }
                return ket_map.get_idx(label + float_to_int(value, default_decimal_places));
            } catch (const std::invalid_argument& e) {
                return empty_idx;
            }
        }
            // case OPRANGE: {
            //     Sequence empty;
            //     return op_range2(empty, seq_one, seq_two);  // Should we inline it, or leave as a function call?
            // }
        default: return ket_map.get_idx("unimplemented");
    }
}

ulong Superposition::process_infix_compile(ulong idx1, unsigned int infix_type, ulong idx2) const {
    // std::cout << "  idx1: " << ket_map.get_str(idx1) << "\n";
    // std::cout << "  idx2: " << ket_map.get_str(idx2) << "\n";
    ulong empty_idx = ket_map.get_idx("");
    ulong yes_idx = ket_map.get_idx("yes");
    ulong no_idx = ket_map.get_idx("no");

    switch (infix_type) {
        case OPGREATEREQUAL:
        case OPGREATER:
        case OPLESSEQUAL:
        case OPLESS: {
            try {
                auto one_idx_vec = ket_map.get_split_idx(idx1);  // Handle more than just kets later! Ie, arithmetic over superpositions and sequences.
                auto two_idx_vec = ket_map.get_split_idx(idx2);
                if (one_idx_vec.empty() || two_idx_vec.empty()) { return empty_idx; }
                long double x = std::stold(ket_map.get_str(one_idx_vec.back()));
                long double y = std::stold(ket_map.get_str(two_idx_vec.back()));
                one_idx_vec.pop_back();
                two_idx_vec.pop_back();
                if (one_idx_vec != two_idx_vec) { return empty_idx; } // Do we want this check here? Ie, checking that the categories are equal?
                switch (infix_type) {
                    case OPGREATEREQUAL:
                        if (x >= y) { return yes_idx; }
                        return no_idx;
                    case OPGREATER:
                        if (x > y) { return yes_idx; }
                        return no_idx;
                    case OPLESSEQUAL:
                        if (x <= y) { return yes_idx; }
                        return no_idx;
                    case OPLESS:
                        if (x < y) { return yes_idx; }
                        return no_idx;
                    default:
                        return empty_idx;
                }
            } catch (const std::invalid_argument& e) {
                return empty_idx;
            }
        }
        case OPAND: {
            if (idx1 == yes_idx && idx2 == yes_idx) { return yes_idx; }
            return no_idx;
        }
        case OPOR: {
            if (idx1 == yes_idx || idx2 == yes_idx) { return yes_idx; }
            return no_idx;
        }
        case OPPLUS:
        case OPMINUS:
        case OPMULT:
        case OPDIV:
        case OPMOD:
        case OPARITHPOWER: {
            // std::cout << "    arithmetic section:\n";
            auto one_idx_vec = ket_map.get_split_idx(idx1);  // Handle more than just kets later! Ie, arithmetic over superpositions and sequences.
            auto two_idx_vec = ket_map.get_split_idx(idx2);
            if (one_idx_vec.empty() || two_idx_vec.empty()) { return empty_idx; }
            if (one_idx_vec.size() == 1 && two_idx_vec.size() == 1) {
                try {
                    long double x = std::stold(ket_map.get_str(one_idx_vec[0]));
                    long double y = std::stold(ket_map.get_str(two_idx_vec[0]));
                    long double value;
                    switch (infix_type) {
                        case OPPLUS : {
                            value = x + y;
                            break;
                        }
                        case OPMINUS : {
                            value = x - y;
                            break;
                        }
                        case OPMULT : {
                            value = x * y;
                            break;
                        }
                        case OPDIV : {
                            value = x / y;
                            break;
                        } // check for div by zero here!
                        case OPMOD : {
                            value = static_cast<long long>(x) % static_cast<long long>(y);
                            break;
                        }
                        case OPARITHPOWER : {
                            value = std::pow(x, y);
                            break;
                        }
                        default:
                            return empty_idx;
                    }
                    return ket_map.get_idx(float_to_int(value, default_decimal_places));
                } catch (const std::invalid_argument &e) {
                    return empty_idx;
                }
            }
            std::string categories1;
            std::vector<long double> values1;
            bool first_pass = true;
            for (const auto idx: one_idx_vec) {
                std::string label = ket_map.get_str(idx);
                if (!is_number(label)) {
                    if (first_pass) {
                        categories1 += label + ": ";
                    } else {
                        break;
                    }
                } else {
                    long double value = std::stold(label);  // Still a potential exception here.
                    values1.push_back(value);
                    first_pass = false;
                }
            }
            std::string categories2;
            std::vector<long double> values2;
            first_pass = true;
            for (const auto idx: two_idx_vec) {
                std::string label = ket_map.get_str(idx);
                if (!is_number(label)) {
                    if (first_pass) {
                        categories2 += label + ": ";
                    } else {
                        break;
                    }
                } else {
                    long double value = std::stold(label);  // Still a potential exception here.
                    values2.push_back(value);
                    first_pass = false;
                }
            }
            // if (categories1 != categories2 || values1.size() != values2.size()) { return empty_idx; }  // Handle cases such as: |number: 5> ** |3> later!
            if (values1.size() != values2.size()) { return empty_idx; }  // Currently broken if you want to do, say: |2: 5> ** |10> == |2: 50>
            std::string categories = categories1;                        // Instead you have to do: |2: 5> ** (|1> :_ |10>)
            if (categories1.empty()) {
                categories = categories2;
            } else if (categories2.empty()) {
                categories = categories1;
            } else if (categories1 != categories2) {
                return empty_idx;
            }
            if (values1.size() == 1) {
                long double x = values1[0];
                long double y = values2[0];
                long double value;
                switch (infix_type) {
                    case OPPLUS : {
                        value = x + y;
                        break;
                    }
                    case OPMINUS : {
                        value = x - y;
                        break;
                    }
                    case OPMULT : {
                        value = x * y;
                        break;
                    }
                    case OPDIV : {
                        value = x / y;
                        break;
                    } // check for div by zero here!
                    case OPMOD : {
                        value = static_cast<long long>(x) % static_cast<long long>(y);
                        break;
                    }
                    case OPARITHPOWER : {
                        value = std::pow(x, y);
                        break;
                    }
                    default:
                        return empty_idx;
                }
                return ket_map.get_idx(categories +float_to_int(value, default_decimal_places));
            }
            if (values1.size() == 2) {
                long double x1 = values1[0];
                long double x2 = values1[1];
                long double y1 = values2[0];
                long double y2 = values2[1];
                long double value1;
                long double value2;
                switch (infix_type) {
                    case OPPLUS : {
                        value1 = x1 + y1;
                        value2 = x2 + y2;
                        break;
                    }
                    case OPMINUS : {
                        value1 = x1 - y1;
                        value2 = x2 - y2;
                        break;
                    }
                    case OPMULT : {
                        value1 = x1 * y1;
                        value2 = x2 * y2;
                        break;
                    }
                    case OPDIV : {
                        value1 = x1 / y1;
                        value2 = x2 / y2;
                        break;
                    } // check for div by zero here!
                    case OPMOD : {
                        value1 = static_cast<long long>(x1) % static_cast<long long>(y1);
                        value2 = static_cast<long long>(x2) % static_cast<long long>(y2);
                        break;
                    }
                    case OPARITHPOWER : {
                        value1 = std::pow(x1, y1);
                        value2 = std::pow(x2, y2);
                        break;
                    }
                    default:
                        return empty_idx;
                }
                return ket_map.get_idx(categories + float_to_int(value1, default_decimal_places) + ": " + float_to_int(value2, default_decimal_places));
            }
            return ket_map.get_idx("unimplemented");
        }
        default: return ket_map.get_idx("unimplemented");
    }
}