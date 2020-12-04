//
// Created by Garry Morrison on 21/09/2020.
//

#include <iostream>
#include <set>
#include "FunctionOperatorLibrary.h"
#include "../Function/misc.h"
#include "../Operator/SimpleOperator.h"
#include "../Function/split_join.h"


Sequence op_srange2(const Sequence& input_seq, const Sequence& start, const Sequence& stop) {
    Sequence step("1");
    return op_srange3(input_seq, start, stop, step);
}

Sequence op_srange3(const Sequence& input_seq, const Sequence& start, const Sequence& stop, const Sequence& step) {
    if (!start.is_ket() || !stop.is_ket() || !step.is_ket()) { return Sequence(); }
    auto start_vec = start.to_ket().label_split_idx();
    auto stop_vec = stop.to_ket().label_split_idx();
    auto step_vec = step.to_ket().label_split_idx();
    if (start_vec.empty() || stop_vec.empty() || step_vec.empty()) { return Sequence(); }
    ulong start_idx = start_vec.back();
    ulong stop_idx = stop_vec.back();
    ulong step_idx = step_vec.back();
    start_vec.pop_back();
    stop_vec.pop_back();
    if ( start_vec != stop_vec ) { return Sequence(); }
    long double v1 = std::stold(ket_map.get_str(start_idx));
    long double v2 = std::stold(ket_map.get_str(stop_idx));
    long double v3 = std::stold(ket_map.get_str(step_idx));
    if (long_double_eq(v3, 0)) { return start; }
    std::string cat = ket_map.get_str(start_vec);  // what if start_vec.size() == 0?
    std::string label;
    Sequence seq;
    if (cat.length() > 0 ) { label = cat + ": "; }
    if (v3 > 0) {
        for (long double i = v1; i <= v2; i += v3) {
            seq.append(label + float_to_int(i, default_decimal_places));
        }
    } else if (v3 < 0) {
        for (long double i = v1; i >= v2; i += v3) {
            seq.append(label + float_to_int(i, default_decimal_places));
        }
    }
    return seq;
}

// Should this return a Ket instead??
Sequence op_arithmetic3(const Sequence &input_seq, const Sequence &one, const Sequence &symbol_ket, const Sequence &two) {
    if (!one.is_ket() || !two.is_ket() || !symbol_ket.is_ket()) { return Sequence(); }
    auto one_idx_vec = one.to_ket().label_split_idx();
    auto symbol = symbol_ket.to_ket().label();
    auto two_idx_vec = two.to_ket().label_split_idx();

    if (one_idx_vec.empty() || two_idx_vec.empty() || symbol.empty()) { return Sequence(); }
    long double x = std::stold(ket_map.get_str(one_idx_vec.back()));
    long double y = std::stold(ket_map.get_str(two_idx_vec.back()));
    one_idx_vec.pop_back();
    two_idx_vec.pop_back();

    if (one_idx_vec != two_idx_vec) { return Sequence(); }
    std::string cat = ket_map.get_str(one_idx_vec);  // what if one_idx_vec.size() == 0?
    std::string label;
    if (cat.length() > 0 ) { label = cat + ": "; }

    long double value;

    char symbol_char = symbol.front();
    switch(symbol_char) {
        case '+' : { value = x + y; break; }
        case '-' : { value = x - y; break; }
        case '*' : { value = x * y; break; }
        case '/' : { value = x / y; break; } // check for div by zero here!
        case '%' : { value = static_cast<long long>(x) % static_cast<long long>(y); break; }
        default: return Sequence();
    }
    return Sequence(label + float_to_int(value, default_decimal_places));
}

double simm(const Superposition &sp1, const Superposition &sp2) {
    if (sp1.size() == 0 || sp2.size() == 0) { return 0; }
    std::set<ulong> merged;
    double one_sum(0), two_sum(0), merged_sum(0);
    for (const auto k : sp1) {
        merged.insert(k.label_idx());
        one_sum += k.value();
    }
    for (const auto k : sp2) {
        merged.insert(k.label_idx());
        two_sum += k.value();
    }

    if ( double_eq(one_sum, 0) || double_eq(two_sum, 0)) { return 0; } // prevent div by zero

    for (const auto idx : merged) {
        double v1 = sp1.find_value(idx);
        double v2 = sp2.find_value(idx);
        merged_sum += std::min(v1, v2);
    }
    return merged_sum / std::max(one_sum, two_sum);
}

double scaled_simm(const Superposition &sp1, const Superposition &sp2) {
    if (sp1.size() == 0 || sp2.size() == 0) { return 0; }

    if (sp1.size() == 1 && sp2.size() == 1) {
        Ket k1 = sp1.to_ket();
        Ket k2 = sp2.to_ket();
        if (k1.label_idx() != k2.label_idx()) { return 0; }
        double a = std::max(k1.value(), 0.0);
        double b = std::max(k2.value(), 0.0);
        if ( double_eq(a, 0) && double_eq(b, 0)) { return 0; }
        return std::min(a, b) / std::max(a, b);
    }

    std::set<ulong> merged;
    double one_sum(0), two_sum(0), merged_sum(0);
    for (const auto k : sp1) {
        merged.insert(k.label_idx());
        one_sum += k.value();
    }
    for (const auto k : sp2) {
        merged.insert(k.label_idx());
        two_sum += k.value();
    }

    if ( double_eq(one_sum, 0) || double_eq(two_sum, 0)) { return 0; } // prevent div by zero

    for (const auto idx : merged) {
        double v1 = sp1.find_value(idx) / one_sum;
        double v2 = sp2.find_value(idx) / two_sum;
        merged_sum += std::min(v1, v2);
    }
    return merged_sum;
}

double simm(const Sequence &seq1, const Sequence &seq2) {
    size_t size = std::max(seq1.size(), seq2.size());
    if (size == 0) { return 0; }
    double r = 0;
    auto seq1_iter = seq1.cbegin();
    auto seq2_iter = seq2.cbegin();
    for (; seq1_iter != seq1.end() and seq2_iter != seq2.end(); ++seq1_iter, ++seq2_iter) {
        // r += simm(*seq1_iter, *seq2_iter);  // probably want scaled_simm() here instead.
        r += scaled_simm(*seq1_iter, *seq2_iter);
    }
    return r / size;
}

double strict_simm(const Sequence &seq1, const Sequence &seq2) {
    if (seq1.size() != seq2.size()) { return 0; }
    size_t size = seq1.size();
    if (size == 0) { return 0; }
    double r = 0;
    auto seq1_iter = seq1.cbegin();
    auto seq2_iter = seq2.cbegin();
    for (; seq1_iter != seq1.end() and seq2_iter != seq2.end(); ++seq1_iter, ++seq2_iter) {
        // r += simm(*seq1_iter, *seq2_iter);  // probably want scaled_simm() here instead.
        r += scaled_simm(*seq1_iter, *seq2_iter);
    }
    return r / size;
}


Sequence op_simm2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2) {
    double result = simm(seq1, seq2);
    if (input_seq.size() == 0) { return Ket("simm", result); }
    if (input_seq.is_ket() && input_seq.to_ket().label_idx() == ket_map.get_idx("")) { return Ket("simm", result); }  // Tidy!
    if (input_seq.is_ket()) { Ket k = input_seq.to_ket(); return Ket(k.label_idx(), k.value()*result); }
    Superposition sp;
    for (const auto k: input_seq.to_sp()) { // For now, let's just cast input_seq to a superposition.
        sp.add(k.label_idx(), k.value()*result);
    }
    return sp;  // Let C++ convert sp to a sequence for us. Maybe change later.
}

Sequence op_strict_simm2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2) {
    double result = strict_simm(seq1, seq2);
    if (input_seq.size() == 0) { return Ket("simm", result); }
    if (input_seq.is_ket() && input_seq.to_ket().label_idx() == ket_map.get_idx("")) { return Ket("simm", result); }  // Tidy!
    if (input_seq.is_ket()) { Ket k = input_seq.to_ket(); return Ket(k.label_idx(), k.value()*result); }
    Superposition sp;
    for (const auto k: input_seq.to_sp()) { // For now, let's just cast input_seq to a superposition.
        sp.add(k.label_idx(), k.value()*result);
    }
    return sp;  // Let C++ convert sp to a sequence for us. Maybe change later.
}


Superposition intersection(const Superposition &sp1, const Superposition &sp2) {
    if (sp1.size() == 0 || sp2.size() == 0) { return Superposition(); }
    std::set<ulong> merged;
    for (const auto k : sp1) {
        merged.insert(k.label_idx());
    }
    for (const auto k : sp2) {
        merged.insert(k.label_idx());
    }
    Superposition result;
    for (const auto idx: merged) {
        double v1 = sp1.find_value(idx);
        double v2 = sp2.find_value(idx);
        double value = std::min(v1, v2);
        if (value > 0) {
            result.add(idx, value);
        }
    }
    return result;
}

Sequence op_intersection2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    auto one_iter = one.cbegin();
    auto two_iter = two.cbegin();
    Sequence seq;
    for (; one_iter != one.end() and two_iter != two.end(); ++one_iter, ++two_iter) {
        seq.append(intersection(*one_iter, *two_iter));
    }
    return seq;
}

Superposition sp_union(const Superposition &sp1, const Superposition &sp2) {
    if (sp1.size() == 0 && sp2.size() == 0) { return Superposition(); }
    std::set<ulong> merged;
    for (const auto k : sp1) {
        merged.insert(k.label_idx());
    }
    for (const auto k : sp2) {
        merged.insert(k.label_idx());
    }
    Superposition result;
    for (const auto idx: merged) {
        double v1 = sp1.find_value(idx);
        double v2 = sp2.find_value(idx);
        double value = std::max(v1, v2);
        if (value > 0) {
            result.add(idx, value);
        }
    }
    return result;
}

Sequence op_union2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    auto one_iter = one.cbegin();
    auto two_iter = two.cbegin();
    Sequence seq;
    for (; one_iter != one.end() and two_iter != two.end(); ++one_iter, ++two_iter) {
        seq.append(sp_union(*one_iter, *two_iter));
    }
    return seq;
}


// Filter could do with some optimization!
Sequence op_filter(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {

    Sequence result;
    // First branch:
    // NB: Can be expensive! Especially if rel-kets[*] is large, or supported-ops is large.
    // Eg: filter(|*>, |doctor> + |nurse>) rel-kets[*]
    if (one.to_ket().label_idx() == ket_map.get_idx("*")) {
        ulong supported_ops_idx = ket_map.get_idx("supported-ops");
        for (const auto &sp: input_seq) {
            Sequence tmp;
            for (const auto &k: sp) {
                Superposition s_ops = context.recall(supported_ops_idx, k.label_idx())->to_sp();
                for (const auto &s_op: s_ops) {
                    ulong result_idx = context.recall(s_op.label_split_idx()[1], k.label_idx())->to_ket().label_idx();
                    for (const auto &k2: two.to_sp()) {
                        if (result_idx == k2.label_idx()) {
                            tmp.add(k);
                            goto break_loop;
                        }
                    }
                }
                break_loop:;
            }
            result.append(tmp);
        }
        return result;
    }

    // General branch:
    auto one_vec = one.to_ket().label_split_idx();
    if (one_vec.size() < 2) { return Ket(""); }

    std::vector<SimpleOperator> operators;
    if (one_vec[0] == ket_map.get_idx("op")) {
        SimpleOperator op(one_vec[1]);
        operators.push_back(op);
    } else if (one_vec[0] == ket_map.get_idx("ops")) {
        auto string_ops = split(ket_map.get_str(one_vec[1]), " ");
        for (const auto &s: string_ops) {
            SimpleOperator op(s);
            operators.push_back(op);
        }
    } else {
        return Ket("");
    }

    ulong two_idx = two.to_ket().label_idx();
    ulong star_idx = ket_map.get_idx("*");

    if (two_idx == star_idx) {
        for (const auto &sp: input_seq) {
            Sequence tmp;
            for (const auto &k: sp) {
                Sequence seq = k.to_seq();
                bool match = true;
                for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                    if (context.recall_type((*it).get_idx(), seq.to_ket().label_idx()) != RULENORMAL) {
                        match = false;
                        break;
                    }
                    seq = (*it).Compile(context, seq);
                }
                if (match) {
                    tmp.add(k);
                }
            }
            result.append(tmp);
        }
    } else {
        for (const auto &sp: input_seq) {
            Sequence tmp;
            for (const auto &k: sp) {
                Sequence seq = k.to_seq();
                for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                    seq = (*it).Compile(context, seq);
                }
                ulong result_idx = seq.to_ket().label_idx();
                for (const auto &k2: two.to_sp()) {
                    if (result_idx == k2.label_idx()) {
                        tmp.add(k);
                        break;
                    }
                }
            }
            result.append(tmp);
        }
    }
    return result;
}

Sequence op_apply(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    Sequence result;
    for (const auto &sp: one) {
        Sequence r;
        for (const auto &k_op: sp) {
            auto k_vec = k_op.label_split_idx();
            if (k_vec.size() < 2) {
                continue;
            }
            if (k_vec[0] == ket_map.get_idx("op")) {
                SimpleOperator op(k_vec[1]);
                for (const auto &k: two.to_sp()) {
                    Sequence seq = op.Compile(context, k.to_seq());
                    r.add(seq);
                }
            } else if (k_vec[0] == ket_map.get_idx("ops")) {
                std::vector<SimpleOperator> operators;
                auto string_ops = split(ket_map.get_str(k_vec[1]), " ");
                for (const auto &s: string_ops) {
                    SimpleOperator op(s);
                    operators.push_back(op);
                }
                for (const auto &k: two.to_sp()) {
                    Sequence seq = k.to_seq();
                    for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                        seq = (*it).Compile(context, seq);
                    }
                    r.add(seq);
                }
            }
        }
        result.append(r);
    }
    return result;
}

Sequence op_learn(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(three);
    for (const auto &k_op: one.to_sp()) {
        auto k_vec = k_op.label_split_idx();
        if (k_vec.size() < 2) {
            continue;
        }
        if (k_vec[0] != ket_map.get_idx("op")) {
            continue;
        }
        for (const auto &k: two.to_sp()) {
            context.learn(k_vec[1], k.label_idx(), bSeq);
        }
    }
    return three;
}

Sequence op_add_learn(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(three);
    for (const auto &k_op: one.to_sp()) {
        auto k_vec = k_op.label_split_idx();
        if (k_vec.size() < 2) {
            continue;
        }
        if (k_vec[0] != ket_map.get_idx("op")) {
            continue;
        }
        for (const auto &k: two.to_sp()) {
            context.add_learn(k_vec[1], k.label_idx(), bSeq);
        }
    }
    return three;
}

Sequence op_seq_learn(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(three);
    for (const auto &k_op: one.to_sp()) {
        auto k_vec = k_op.label_split_idx();
        if (k_vec.size() < 2) {
            continue;
        }
        if (k_vec[0] != ket_map.get_idx("op")) {
            continue;
        }
        for (const auto &k: two.to_sp()) {
            context.seq_learn(k_vec[1], k.label_idx(), bSeq);
        }
    }
    return three;
}

Sequence op_zip(ContextList &context, const Sequence &input_seq, const Sequence &one) {
    Sequence result;
    auto input_seq_iter = input_seq.cbegin();
    auto one_iter = one.cbegin();
    for (; input_seq_iter != input_seq.cend() && one_iter != one.cend(); ++input_seq_iter, ++one_iter) {
        Sequence seq;
        auto op_vec = (*one_iter).to_ket().label_split_idx();  // Is op_vec.size() always >= 1? Is it ever 0??
        if (op_vec.size() < 2) {
            result.append(seq);
        } else if (op_vec[0] != ket_map.get_idx("op")) {
            result.append(seq);
        } else {
            SimpleOperator op(op_vec[1]);
            seq = op.Compile(context, (*input_seq_iter).to_seq());
            result.append(seq);
        }
    }
    return result;
}

Sequence op_if(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    std::string str_condition = one.to_ket().label();
    Sequence result;
    if (str_condition == "yes" || str_condition == "true") {
        for (const auto &k: two.to_sp()) {
            auto k_vec = k.label_split_idx();
            if (k_vec.size() < 2) {
                continue;
            }
            if (k_vec[0] != ket_map.get_idx("op")) {
                continue;
            }
            SimpleOperator op(k_vec[1]);
            Sequence seq = op.Compile(context, input_seq);
            result.add(seq);
        }
        return result;
    } else {
        for (const auto &k: three.to_sp()) {
            auto k_vec = k.label_split_idx();
            if (k_vec.size() < 2) {
                continue;
            }
            if (k_vec[0] != ket_map.get_idx("op")) {
                continue;
            }
            SimpleOperator op(k_vec[1]);
            Sequence seq = op.Compile(context, input_seq);
            result.add(seq);
        }
        return result;
    }
}

Sequence op_is_mbr(const Sequence &input_seq, const Sequence &one) {
    if (input_seq.size() == 0 || one.size() == 0) { return Ket("no"); }
    ulong ket_idx = one.to_ket().label_idx();
    for (const auto &k: input_seq.to_sp()) {
        if (k.label_idx() == ket_idx) {
            return Ket("yes");
        }
    }
    return Ket("no");
}

Sequence op_is_subset(const Sequence &input_seq, const Sequence &one) {
    if (input_seq.size() == 0 || one.size() == 0) { return Ket("no"); }
    for (const auto &sp_one: one) {
        for (const auto &sp_input: input_seq) {
            for (const auto &k: sp_one) {
                double one_value = k.value();
                double input_seq_value = sp_input.find_value(k);
                if (one_value > input_seq_value) {
                    return Ket("no");
                }
            }
        }
    }
    return Ket("yes");
}

Sequence op_and2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    ulong yes_idx = ket_map.get_idx("yes");
    if (one.to_ket().label_idx() == yes_idx && two.to_ket().label_idx() == yes_idx) {
        return Sequence("yes");
    }
    return Sequence("no");
}

Sequence op_or2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    ulong yes_idx = ket_map.get_idx("yes");
    if (one.to_ket().label_idx() == yes_idx || two.to_ket().label_idx() == yes_idx) {
        return Sequence("yes");
    }
    return Sequence("no");
}

Sequence op_xor2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    ulong yes_idx = ket_map.get_idx("yes");
    if (!(one.to_ket().label_idx() == yes_idx) != !(two.to_ket().label_idx() == yes_idx)) {
        return Sequence("yes");
    }
    return Sequence("no");
}

Sequence op_and1(const Sequence &input_seq, const Sequence &one) {
    ulong yes_idx = ket_map.get_idx("yes");
    ulong no_idx = ket_map.get_idx("no");
    bool one_is_yes = one.to_ket().label_idx() == yes_idx;
    Sequence result;
    for (const auto &sp: input_seq) {
        Sequence tmp;
        for (const auto &k: sp) {
            if (k.label_idx() == yes_idx && one_is_yes) {
                tmp.add(yes_idx);  // should we keep the coefficient of k in the result?
            } else {
                tmp.add(no_idx);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence op_or1(const Sequence &input_seq, const Sequence &one) {
    ulong yes_idx = ket_map.get_idx("yes");
    ulong no_idx = ket_map.get_idx("no");
    bool one_is_yes = one.to_ket().label_idx() == yes_idx;
    Sequence result;
    for (const auto &sp: input_seq) {
        Sequence tmp;
        for (const auto &k: sp) {
            if (k.label_idx() == yes_idx || one_is_yes) {
                tmp.add(yes_idx);
            } else {
                tmp.add(no_idx);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence op_xor1(const Sequence &input_seq, const Sequence &one) {
    ulong yes_idx = ket_map.get_idx("yes");
    ulong no_idx = ket_map.get_idx("no");
    bool one_is_yes = one.to_ket().label_idx() == yes_idx;
    Sequence result;
    for (const auto &sp: input_seq) {
        Sequence tmp;
        for (const auto &k: sp) {
            if (!(k.label_idx() == yes_idx) != !one_is_yes) {
                tmp.add(yes_idx);
            } else {
                tmp.add(no_idx);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence op_smap3(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    if (input_seq.size() == 0) { return Sequence(""); }
    try {
        unsigned int min_ngram_size = std::stoi(one.to_ket().label());
        unsigned int max_ngram_size = std::stoi(two.to_ket().label());
        unsigned int width = input_seq.size();
        min_ngram_size = std::min(min_ngram_size, width);
        max_ngram_size = std::min(max_ngram_size, width);
        if (min_ngram_size <= 0 || max_ngram_size <= 0 || max_ngram_size < min_ngram_size) { return Sequence(""); }
        std::vector<SimpleOperator> operators;
        for (const auto &k: three.to_sp()) {
            auto k_vec = k.label_split_idx();
            if (k_vec.size() < 2) {
                continue;
            }
            if (k_vec[0] != ket_map.get_idx("op")) {
                continue;
            }
            SimpleOperator op(k_vec[1]);
            operators.push_back(op);
        }
        Sequence result;
        Superposition empty("");
        for (int i = 0; i < width; i++) {  // Pad the result sequence with empty superpositions.
            result.append(empty);          // This is needed so that result.set() works.
        }
        ulong the_idx = ket_map.get_idx("the");
        ulong smap_pos_idx = ket_map.get_idx("smap pos");
        for (const auto &op: operators) {
            for (unsigned int size = min_ngram_size; size <= max_ngram_size; size++) {
                for (unsigned int start = 0; start < width - size + 1; start++) {
                    auto start_iter = input_seq.cbegin() + (size_t) start;
                    auto stop_iter = input_seq.cbegin() + (size_t) (start + size);
                    Sequence patch;
                    for (auto iter = start_iter; iter != stop_iter; ++iter) {
                        patch.append(*iter);
                    }
                    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(std::to_string(start + size));
                    context.learn(the_idx, smap_pos_idx, bSeq);
                    Superposition patch_result = op.Compile(context, patch).to_sp();
                    Superposition new_patch_result = result.get(start + size - 1);
                    new_patch_result.add(patch_result);
                    result.set(start + size - 1, new_patch_result);
                }
            }
        }
        return result;
    } catch (const std::invalid_argument& e) {
        return Sequence("");
    }
}

Sequence op_is_equal2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2) {
    if (seq1.size() != seq2.size()) { return Sequence("no"); }
    Ket one = seq1.to_ket();
    Ket two = seq2.to_ket();
    if (one.label_idx() == two.label_idx() && double_eq(one.value(), two.value())) {
        return Sequence("yes");
    }
    return Sequence("no");
}

// Implement Dijkstra's algorithm:
Sequence op_find_path_between(ContextList &context, const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2) {
    ulong source = seq1.to_ket().label_idx();
    ulong target = seq2.to_ket().label_idx();
    std::vector<ulong> Q_vec = context.relevant_kets("*");
    std::set<ulong> Q(Q_vec.begin(), Q_vec.end());
    std::map<ulong, unsigned int> dist;
    std::map<ulong, ulong> prev;

    ulong u = source;

    for (const ulong vertex: Q) {
        dist[vertex] = std::numeric_limits<unsigned int>::max();
    }
    dist[source] = 0;

    while (!Q.empty()) {
        unsigned int min_dist = std::numeric_limits<unsigned int>::max();
        for (const ulong vertex: Q) {
            if (dist[vertex] < min_dist) {
                u = vertex;
                min_dist = dist[vertex];
            }
        }
        if (u == target) {
            break;
        }
        Q.erase(u);
        std::vector<ulong> s_ops = context.supported_ops(u);

        for (const ulong s_op: s_ops) {
            ulong v = context.recall(s_op, u)->to_ket().label_idx();
            if (Q.find(v) != Q.end()) {
                unsigned int alt = dist[u] + 1;  // all neighbours are 1 step away.
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }

    Sequence result;
    u = target;
    if (prev.find(u) != prev.end() || u == source) {
        while (prev.find(u) != prev.end()) {
            Superposition tmp(u);
            result.append(tmp);
            u = prev[u];
        }
    }
    Superposition source_sp(source);
    result.append(source_sp);
    return result.sreverse();
}

Sequence op_find_operators_between(ContextList &context, const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2) {
    ulong source = seq1.to_ket().label_idx();
    ulong target = seq2.to_ket().label_idx();
    std::vector<ulong> Q_vec = context.relevant_kets("*");
    std::set<ulong> Q(Q_vec.begin(), Q_vec.end());
    std::map<ulong, unsigned int> dist;
    std::map<ulong, ulong> prev;
    std::map<ulong, ulong> prev_op;

    ulong u = source;

    for (const ulong vertex: Q) {
        dist[vertex] = std::numeric_limits<unsigned int>::max();
    }
    dist[source] = 0;

    while (!Q.empty()) {
        unsigned int min_dist = std::numeric_limits<unsigned int>::max();
        for (const ulong vertex: Q) {
            if (dist[vertex] < min_dist) {
                u = vertex;
                min_dist = dist[vertex];
            }
        }
        if (u == target) {
            break;
        }
        Q.erase(u);
        std::vector<ulong> s_ops = context.supported_ops(u);

        for (const ulong s_op: s_ops) {
            ulong v = context.recall(s_op, u)->to_ket().label_idx();
            if (Q.find(v) != Q.end()) {
                unsigned int alt = dist[u] + 1;  // all neighbours are 1 step away.
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    prev_op[v] = s_op;
                }
            }
        }
    }

    Sequence result;
    u = target;
    if (prev.find(u) != prev.end() || u == source) {
        while (prev.find(u) != prev.end()) {
            ulong idx = ket_map.get_idx("op: " + ket_map.get_str(prev_op[u]));
            Superposition tmp(idx);
            result.append(tmp);
            u = prev[u];
        }
    }
    return result.sreverse();
}
