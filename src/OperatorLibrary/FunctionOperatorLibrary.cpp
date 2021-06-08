//
// Created by Garry Morrison on 21/09/2020.
//

#include <iostream>
#include <set>
#include <unordered_set>
#include <math.h>
#include "FunctionOperatorLibrary.h"
#include "../Function/misc.h"
#include "../Operator/SimpleOperator.h"
#include "../Function/split_join.h"
#include "../Operator/FunctionOperator.h"

Superposition old_range2(ulong start_idx2, ulong stop_idx2) {
    auto start_vec = ket_map.get_split_idx(start_idx2);
    auto stop_vec = ket_map.get_split_idx(stop_idx2);

    if (start_vec.empty() || stop_vec.empty()) { return Superposition(); }
    ulong start_idx = start_vec.back();
    ulong stop_idx = stop_vec.back();

    start_vec.pop_back();
    stop_vec.pop_back();
    if ( start_vec != stop_vec ) { return Superposition(); }
    try {
        long double v1 = std::stold(ket_map.get_str(start_idx));
        long double v2 = std::stold(ket_map.get_str(stop_idx));

        std::string label = ket_map.get_str(start_vec);  // what if start_vec.size() == 0?
        Superposition sp;
        if (label.length() > 0) { label += ": "; }
        // unsigned int steps = std::ceil(v2 - v1);  // Do we need ceiling() or floor()?
        unsigned int steps = std::floor(v2 - v1);  // Do we need ceiling() or floor()?
        for (unsigned int i = 0; i <= steps; i++) {  // Fix float iteration variables issue!
            sp.add(label + float_to_int(i + v1, default_decimal_places));
        }
        return sp;
    } catch (const std::invalid_argument &e) {
        return Superposition();
    }
}

Superposition range2(ulong start_idx, ulong stop_idx) {
    auto start_vec = ket_map.get_split_idx(start_idx);
    auto stop_vec = ket_map.get_split_idx(stop_idx);
    if (start_vec.empty() || stop_vec.empty()) { return Superposition(); }
    if (start_vec.size() == 1 && stop_vec.size() == 1) {
        try {
            long double v1 = std::stold(ket_map.get_str(start_vec[0]));
            long double v2 = std::stold(ket_map.get_str(stop_vec[0]));
            Superposition sp;
            unsigned int steps = std::floor(v2 - v1);  // Do we need ceiling() or floor()?
            for (unsigned int i = 0; i <= steps; i++) {
                sp.add(float_to_int(i + v1, default_decimal_places));
            }
            return sp;
        } catch (const std::invalid_argument &e) {
            return Superposition();
        }
    }
    std::string categories1;
    std::vector<long double> values1;
    bool first_pass = true;
    for (const auto idx: start_vec) {
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
    for (const auto idx: stop_vec) {
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
    if (categories1 != categories2 || values1.size() != values2.size()) { return Superposition(); }
    if (values1.size() == 1) {
        Superposition sp;
        unsigned int steps = std::floor(values2[0] - values1[0]);  // Do we need ceiling() or floor()?
        for (unsigned int i = 0; i <= steps; i++) {
            sp.add(categories1 + float_to_int(i + values1[0], default_decimal_places));
        }
        return sp;
    }
    if (values1.size() == 2) {
        Superposition sp;
        unsigned int steps0 = std::floor(values2[0] - values1[0]);  // Do we need ceiling() or floor()?
        unsigned int steps1 = std::floor(values2[1] - values1[1]);  // Do we need ceiling() or floor()?
        for (unsigned int i = 0; i <= steps0; i++) {
            for (unsigned int j = 0; j <= steps1; j++) {
                sp.add(categories1 + float_to_int(i + values1[0], default_decimal_places) + ": " + float_to_int(j + values1[1], default_decimal_places));
            }
        }
        return sp;
    }
    return Superposition("unimplemented range");
}

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
    try {
        long double v1 = std::stold(ket_map.get_str(start_idx));
        long double v2 = std::stold(ket_map.get_str(stop_idx));
        long double v3 = std::stold(ket_map.get_str(step_idx));
        if (long_double_eq(v3, 0)) { return start; }
        std::string label = ket_map.get_str(start_vec);  // what if start_vec.size() == 0?
        Sequence seq;
        if (label.length() > 0) { label += ": "; }
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
    } catch (const std::invalid_argument &e) {
        return Sequence();
    }
}

Superposition op_range2(const Sequence& input_seq, const Sequence& start, const Sequence& stop) {
    Sequence step("1");
    return op_range3(input_seq, start, stop, step);
}

Superposition op_range3(const Sequence& input_seq, const Sequence& start, const Sequence& stop, const Sequence& step) {
    if (!start.is_ket() || !stop.is_ket() || !step.is_ket()) { return Superposition(); }
    auto start_vec = start.to_ket().label_split_idx();
    auto stop_vec = stop.to_ket().label_split_idx();
    auto step_vec = step.to_ket().label_split_idx();
    if (start_vec.empty() || stop_vec.empty() || step_vec.empty()) { return Superposition(); }
    ulong start_idx = start_vec.back();
    ulong stop_idx = stop_vec.back();
    ulong step_idx = step_vec.back();
    start_vec.pop_back();
    stop_vec.pop_back();
    if ( start_vec != stop_vec ) { return Superposition(); }
    try {
        long double v1 = std::stold(ket_map.get_str(start_idx));
        long double v2 = std::stold(ket_map.get_str(stop_idx));
        long double v3 = std::stold(ket_map.get_str(step_idx));
        if (long_double_eq(v3, 0)) { return start.to_sp(); }
        std::string label = ket_map.get_str(start_vec);  // what if start_vec.size() == 0?
        Superposition sp;
        if (label.length() > 0) { label += ": "; }
        if (v3 > 0) {
            for (long double i = v1; i <= v2; i += v3) {
                sp.add(label + float_to_int(i, default_decimal_places));
            }
        } else if (v3 < 0) {
            for (long double i = v1; i >= v2; i += v3) {
                sp.add(label + float_to_int(i, default_decimal_places));
            }
        }
        return sp;
    } catch (const std::invalid_argument &e) {
        return Superposition();
    }
}


// Should this return a Ket instead??
Sequence op_arithmetic3(const Sequence &input_seq, const Sequence &one, const Sequence &symbol_ket, const Sequence &two) {
    if (!one.is_ket() || !two.is_ket() || !symbol_ket.is_ket()) { return Sequence(); }
    auto one_idx_vec = one.to_ket().label_split_idx();
    auto symbol = symbol_ket.to_ket().label();
    auto two_idx_vec = two.to_ket().label_split_idx();

    if (one_idx_vec.empty() || two_idx_vec.empty() || symbol.empty()) { return Sequence(); }
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
            return Sequence();
        }

        long double value;

        char symbol_char = symbol.front();
        switch (symbol_char) {
            case '+' : {
                value = x + y;
                break;
            }
            case '-' : {
                value = x - y;
                break;
            }
            case '*' : {
                value = x * y;
                break;
            }
            case '/' : {
                value = x / y;
                break;
            } // check for div by zero here!
            case '%' : {
                value = static_cast<long long>(x) % static_cast<long long>(y);
                break;
            }
            case '^' : {
                value = pow(x, y);
                break;
            }
            default:
                return Sequence();
        }
        return Sequence(label + float_to_int(value, default_decimal_places));
    } catch (const std::invalid_argument& e) {
        return Sequence();
    }
}

double simm(const Superposition &sp1, const Superposition &sp2) {
    // if (sp1.size() == 0 || sp2.size() == 0) { return 0; }
    // if (sp1.size() == 0 && sp2.size() == 0) { return 1; }  // I think we want this? Maybe it should be in sequence simm instead?
    if (sp1.size() == 0 || sp2.size() == 0) { return 0; }  // Eg, If we put it here, it might break things.
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
    if (size == 0) { return 1; }  // Do we want 0 or 1 to be returned when input both empty kets?
    double r = 0;
    auto seq1_iter = seq1.cbegin();
    auto seq2_iter = seq2.cbegin();
    for (; seq1_iter != seq1.end() and seq2_iter != seq2.end(); ++seq1_iter, ++seq2_iter) {
        // r += simm(*seq1_iter, *seq2_iter);  // probably want scaled_simm() here instead.
        // r += scaled_simm(*seq1_iter, *seq2_iter);
        if ((*seq1_iter).size() == 0 && (*seq2_iter).size() == 0) {
            r += 1;
        } else {
            r += scaled_simm(*seq1_iter, *seq2_iter);
        }
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


Superposition sp_intersection(const Superposition &sp1, const Superposition &sp2) {
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
    // for (; one_iter != one.end() and two_iter != two.end(); ++one_iter, ++two_iter) {  // Only has seq-length of the shortest sequence.
    //     seq.append(sp_intersection(*one_iter, *two_iter));
    // }
    size_t min_size = std::min(one.size(), two.size());
    for (size_t k = 0; k < min_size; k++) {
        seq.append(sp_intersection(one.get(k), two.get(k)));
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
        if (value > 0) {   // Do we need this check for unions?
            result.add(idx, value);
        }
    }
    return result;
}

Sequence op_union2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    auto one_iter = one.cbegin();
    auto two_iter = two.cbegin();
    Sequence seq;
    // for (; one_iter != one.end() and two_iter != two.end(); ++one_iter, ++two_iter) {  // Only has seq-length of the shortest sequence.
    //     seq.append(sp_union(*one_iter, *two_iter));
    // }
    size_t max_size = std::max(one.size(), two.size());
    for (size_t k = 0; k < max_size; k++) {
        seq.append(sp_union(one.get(k), two.get(k)));
    }
    return seq;
}

Superposition sp_sum(const Superposition &sp1, const Superposition &sp2) {
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
        double value = v1 + v2;
        if (value > 0) {   // Do we need this check for sum?
            result.add(idx, value);
        }
    }
    return result;
}

Sequence op_sum2(const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    auto one_iter = one.cbegin();
    auto two_iter = two.cbegin();
    Sequence seq;
    // for (; one_iter != one.end() and two_iter != two.end(); ++one_iter, ++two_iter) {  // Only has seq-length of the shortest sequence.
    //     seq.append(sp_union(*one_iter, *two_iter));
    // }
    size_t max_size = std::max(one.size(), two.size());
    for (size_t k = 0; k < max_size; k++) {
        seq.append(sp_sum(one.get(k), two.get(k)));
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

// Filter could do with some optimization!
Sequence op_not_filter(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {

    Sequence result;
    Superposition two_sp = two.to_sp();
    ulong two_idx = two.to_ket().label_idx();
    ulong star_idx = ket_map.get_idx("*");
    ulong supported_ops_idx = ket_map.get_idx("supported-ops");

    // First branch:
    // NB: Can be expensive! Especially if rel-kets[*] is large, or supported-ops is large.
    // Eg: not-filter(|*>, |doctor> + |nurse>) rel-kets[*]
    if (one.to_ket().label_idx() == star_idx) {
        for (const auto &sp: input_seq) {
            Sequence tmp;
            for (const auto &k: sp) {
                Superposition s_ops = context.recall(supported_ops_idx, k.label_idx())->to_sp();
                for (const auto &s_op: s_ops) {
                    ulong result_idx = context.recall(s_op.label_split_idx()[1], k.label_idx())->to_ket().label_idx();
                    for (const auto &k2: two_sp) {
                        if (result_idx == k2.label_idx()) {
                            goto break_loop;
                        }
                    }
                }
                tmp.add(k);
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

    if (two_idx == star_idx) {  // Need to verify this branch is correct!
        for (const auto &sp: input_seq) {
            Sequence tmp;
            for (const auto &k: sp) {
                bool match = false;
                Sequence seq = k.to_seq();
                for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                    if (context.recall_type((*it).get_idx(), seq.to_ket().label_idx()) == RULENORMAL) {
                        match = true;
                        break;
                    }
                    seq = (*it).Compile(context, seq);
                }
                if (!match) {
                    tmp.add(k);
                }
            }
            result.append(tmp);
        }
    } else {
        for (const auto &sp: input_seq) {
            Sequence tmp;
            for (const auto &k: sp) {
                bool match = false;
                Sequence seq = k.to_seq();
                for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                    seq = (*it).Compile(context, seq);
                }
                ulong result_idx = seq.to_ket().label_idx();
                for (const auto &k2: two_sp) {
                    if (result_idx == k2.label_idx()) {
                        match = true;
                        break;
                    }
                }
                if (!match) {
                    tmp.add(k);
                }
            }
            result.append(tmp);
        }
    }

    return result;
}

Sequence op_apply(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    Sequence result, empty;
    for (const auto &sp: one) {
        Sequence r;
        for (const auto &k_op: sp) {
            auto k_vec = k_op.label_split_idx();
            if (k_vec.size() < 2) {
                continue;
            }
            if (k_vec[0] == ket_map.get_idx("op")) {
                SimpleOperator op(k_vec[1]);
                if (two.size() == 0) {
                    Sequence seq = op.Compile(context, empty);
                    r.add(seq);
                } else {
                    for (const auto &k: two.to_sp()) {
                        Sequence seq = op.Compile(context, k.to_seq());
                        r.add(seq);
                    }
                }
            } else if (k_vec[0] == ket_map.get_idx("ops")) {
                std::vector<SimpleOperator> operators;
                auto string_ops = split(ket_map.get_str(k_vec[1]), " ");
                for (const auto &s: string_ops) {
                    SimpleOperator op(s);
                    operators.push_back(op);
                }
                if (two.size() == 0) {
                    Sequence seq;
                    for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                        seq = (*it).Compile(context, seq);
                    }
                    r.add(seq);
                } else {
                    for (const auto &k: two.to_sp()) {
                        Sequence seq = k.to_seq();
                        for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                            seq = (*it).Compile(context, seq);
                        }
                        r.add(seq);
                    }
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

Sequence op_value_if(const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    std::string str_condition = one.to_ket().label();
    if (str_condition == "yes" || str_condition == "true") {  // Should we instead cast "yes" and "true" to ulong using ket_map, and then do the compare?
        return two;
    }
    return three;
}

Sequence op_is_mbr(const Sequence &input_seq, const Sequence &one) {
    if (input_seq.size() == 0 || one.size() == 0) { return Ket("no"); }
    ulong ket_idx = one.to_ket().label_idx();
    for (const auto &k: input_seq.to_sp()) {  // could probably tighten this up using sp.find_value(ket).
        if (k.label_idx() == ket_idx) {
            return Ket("yes");
        }
    }
    return Ket("no");
}

Sequence op_mbr(const Sequence &input_seq, const Sequence &one) {
    if (input_seq.size() == 0 || one.size() == 0) { return Ket(""); }
    Ket one_ket = one.to_ket();
    double value = input_seq.to_sp().find_value(one_ket);
    return Ket(one_ket.label_idx(), value);
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
    if ((seq1.size() != seq2.size()) || (seq1.size() == 0 && seq2.size() == 0)) { return Sequence("no"); }
    // Ket one = seq1.to_ket();
    // Ket two = seq2.to_ket();
    // if (one.label_idx() == two.label_idx() && double_eq(one.value(), two.value())) {
    //     return Sequence("yes");
    // }
    if (seq1 == seq2) { return Sequence("yes"); }  // Make use of our new operator== methods.
    return Sequence("no");
}

// Implement Dijkstra's algorithm:
// Currently infinite loop if there is no pathway. Fix!
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
            Superposition neighbours = context.recall(s_op, u)->to_sp();
            for (const auto &k: neighbours) {
                ulong v = k.label_idx();
                if (Q.find(v) != Q.end()) {
                    unsigned int alt = dist[u] + 1;  // all neighbours are 1 step away.
                    if (alt < dist[v]) {
                        dist[v] = alt;
                        prev[v] = u;
                    }
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

// Currently infinite loop if there is no pathway. Fix!
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
            Superposition neighbours = context.recall(s_op, u)->to_sp();
            // bool found_neighbour = false;
            for (const auto &k: neighbours) {
                ulong v = k.label_idx();
                if (Q.find(v) != Q.end()) {
                    // found_neighbour = true;
                    unsigned int alt = dist[u] + 1;  // all neighbours are 1 step away.
                    if (alt < dist[v]) {
                        dist[v] = alt;
                        prev[v] = u;
                        prev_op[v] = s_op;
                    }
                }
            }
            // if (!found_neighbour) {
            //     return Sequence("path not found");
            // }
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

Sequence op_sread(const Sequence &input_seq, const Sequence &one) {
    Sequence result;
    for (const auto &sp: one) {
        Sequence tmp;
        for (const auto &k: sp) {
            try {
                long long idx = std::stoll(k.label()) - 1;
                if (idx < 0) {
                    idx += input_seq.size() + 1;
                }
                tmp.add(input_seq.get((ulong)idx));
            } catch (const std::invalid_argument &e) {
                Superposition empty;
                tmp.add(empty);
            }
        }
        result.append(tmp);
    }
    return result;
}

Sequence op_not_sread(const Sequence &input_seq, const Sequence &one) {
    std::set<ulong> index_set;  // Any use in using unordered_set here?
    Superposition positions = one.to_sp();
    for (const auto &k: positions) {
        try {
            long long idx = std::stoll(k.label()) - 1;
            if (idx < 0) {
                idx += input_seq.size() + 1;
            }
            index_set.insert((ulong)idx);
        } catch (const std::invalid_argument &e) {
            continue;
        }
    }
    Sequence result;
    ulong idx = 0;
    for (const auto &sp: input_seq) {  // Can we improve the speed of this for loop?
        if (index_set.find(idx) == index_set.end()) {
            result.append(sp);
        }
        idx++;
    }
    return result;
}

Sequence op_read(const Sequence &input_seq, const Sequence &one) {
    std::set<ulong> index_set;  // Any use in using unordered_set here?
    Superposition positions = one.to_sp();
    for (const auto &k: positions) {
        try {
            long long idx = std::stoll(k.label()) - 1;
            if (idx < 0) {
                idx += input_seq.size() + 1;
            }
            index_set.insert((ulong)idx);
        } catch (const std::invalid_argument &e) {
            continue;
        }
    }
    Sequence result;
    for (const auto &sp: input_seq) {
        Superposition tmp_sp;
        ulong idx = 0;
        for (const auto &k: sp) {
            if (index_set.find(idx) != index_set.end()) {
                tmp_sp.add(k);
            }
            idx++;
        }
        result.append(tmp_sp);
    }
    return result;
}

Sequence op_not_read(const Sequence &input_seq, const Sequence &one) {
    std::set<ulong> index_set;  // Any use in using unordered_set here?
    Superposition positions = one.to_sp();
    for (const auto &k: positions) {
        try {
            long long idx = std::stoll(k.label()) - 1;
            if (idx < 0) {
                idx += input_seq.size() + 1;
            }
            index_set.insert((ulong)idx);
        } catch (const std::invalid_argument &e) {
            continue;
        }
    }
    Sequence result;
    for (const auto &sp: input_seq) {
        Superposition tmp_sp;
        ulong idx = 0;
        for (const auto &k: sp) {
            if (index_set.find(idx) == index_set.end()) {
                tmp_sp.add(k);
            }
            idx++;
        }
        result.append(tmp_sp);
    }
    return result;
}


Sequence op_string_replace(const Sequence &input_seq, const Sequence &one, const Sequence &two) {  // Could do with some optimization.
    Sequence result;
    Superposition from_patterns = one.to_sp();
    std::string to_str = two.to_ket().label();
    for (const auto &sp: input_seq) {
        Superposition tmp_sp;
        for (const auto &k: sp) {
            std::string working_str = k.label();
            for (const auto &k2: from_patterns) {
                string_replace_all(working_str, k2.label(), to_str);
            }
            tmp_sp.add(working_str, k.value());
        }
        result.append(tmp_sp);
    }
    return result;
}

Sequence op_for2(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    auto op_vec = one.to_ket().label_split_idx();
    if (op_vec.size() < 2 || (op_vec[0] != ket_map.get_idx("op"))) { return Sequence(); }
    ulong op_idx = op_vec[1];
    Sequence result, empty;
    for (const Ket &k: two.to_sp()) {
        std::shared_ptr<BaseSequence> seq1 = std::make_shared<Ket>(k);  // Should this be Ket, Superposition, or Sequence?
        FunctionOperator fn(op_idx, seq1);
        Sequence tmp_result = fn.Compile(context, empty);
        result.add(tmp_result);
    }
    return result;
}

Sequence op_for3(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    auto op_vec = one.to_ket().label_split_idx();
    if (op_vec.size() < 2 || (op_vec[0] != ket_map.get_idx("op"))) { return Sequence(); }
    ulong op_idx = op_vec[1];
    Sequence result, empty;
    for (const Ket &k1: two.to_sp()) {
        for (const Ket &k2: three.to_sp()) {
            std::shared_ptr<BaseSequence> seq1 = std::make_shared<Ket>(k1);  // Should this be Ket, Superposition, or Sequence?
            std::shared_ptr<BaseSequence> seq2 = std::make_shared<Ket>(k2);  // Should this be Ket, Superposition, or Sequence?
            FunctionOperator fn(op_idx, seq1, seq2);
            Sequence tmp_result = fn.Compile(context, empty);
            result.add(tmp_result);
        }
    }
    return result;
}

Sequence op_for4(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three, const Sequence &four) {
    auto op_vec = one.to_ket().label_split_idx();
    if (op_vec.size() < 2 || (op_vec[0] != ket_map.get_idx("op"))) { return Sequence(); }
    ulong op_idx = op_vec[1];
    Sequence result, empty;
    for (const Ket &k1: two.to_sp()) {
        for (const Ket &k2: three.to_sp()) {  // Should we apply .to_sp() outside of the for loops?
            for (const Ket &k3: four.to_sp()) {
                std::shared_ptr<BaseSequence> seq1 = std::make_shared<Ket>(k1);  // Should this be Ket, Superposition, or Sequence?
                std::shared_ptr<BaseSequence> seq2 = std::make_shared<Ket>(k2);  // Should this be Ket, Superposition, or Sequence?
                std::shared_ptr<BaseSequence> seq3 = std::make_shared<Ket>(k3);  // Should this be Ket, Superposition, or Sequence?
                FunctionOperator fn(op_idx, seq1, seq2, seq3);
                Sequence tmp_result = fn.Compile(context, empty);
                result.add(tmp_result);
            }
        }
    }
    return result;
}


Sequence op_sfor2(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    auto op_vec = one.to_ket().label_split_idx();
    if (op_vec.size() < 2 || (op_vec[0] != ket_map.get_idx("op"))) { return Sequence(); }
    ulong op_idx = op_vec[1];
    Sequence result, empty;
    for (const Superposition &sp: two) {
        std::shared_ptr<BaseSequence> seq1 = std::make_shared<Sequence>(sp);  // Should this be Superposition, or Sequence?
        FunctionOperator fn(op_idx, seq1);
        Sequence tmp_result = fn.Compile(context, empty);
        // result.add(tmp_result);
        result.append(tmp_result);  // Do we want add() or append()?
    }
    return result;
}

Sequence op_sfor3(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three) {
    auto op_vec = one.to_ket().label_split_idx();
    if (op_vec.size() < 2 || (op_vec[0] != ket_map.get_idx("op"))) { return Sequence(); }
    ulong op_idx = op_vec[1];
    Sequence result, empty;
    for (const Superposition &sp1: two) {
        for (const Superposition &sp2: three) {
            std::shared_ptr<BaseSequence> seq1 = std::make_shared<Sequence>(sp1);  // Should this be Superposition, or Sequence?
            std::shared_ptr<BaseSequence> seq2 = std::make_shared<Sequence>(sp2);  // Should this be Superposition, or Sequence?
            FunctionOperator fn(op_idx, seq1, seq2);
            Sequence tmp_result = fn.Compile(context, empty);
            // result.add(tmp_result);
            result.append(tmp_result);  // Do we want add() or append()?
        }
    }
    return result;
}

Sequence op_sfor4(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three, const Sequence &four) {
    auto op_vec = one.to_ket().label_split_idx();
    if (op_vec.size() < 2 || (op_vec[0] != ket_map.get_idx("op"))) { return Sequence(); }
    ulong op_idx = op_vec[1];
    Sequence result, empty;
    for (const Superposition &sp1: two) {
        for (const Superposition &sp2: three) {
            for (const Superposition &sp3: four) {
                std::shared_ptr<BaseSequence> seq1 = std::make_shared<Sequence>(sp1);  // Should this be Superposition, or Sequence?
                std::shared_ptr<BaseSequence> seq2 = std::make_shared<Sequence>(sp2);  // Should this be Superposition, or Sequence?
                std::shared_ptr<BaseSequence> seq3 = std::make_shared<Sequence>(sp3);  // Should this be Superposition, or Sequence?
                FunctionOperator fn(op_idx, seq1, seq2, seq3);
                Sequence tmp_result = fn.Compile(context, empty);
                // result.add(tmp_result);
                result.append(tmp_result);  // Do we want add() or append()?
            }
        }
    }
    return result;
}

Sequence op_unlearn_fn(ContextList &context, const Sequence &input_seq, const Sequence &one) {
    if (input_seq.size() == 0 || one.size() == 0) { return Sequence(); }
    ulong op_prefix_idx = ket_map.get_idx("op");
    std::vector<ulong> op_idx_to_delete;
    for (const auto &k: one.to_sp()) {
        auto vec = k.label_split_idx();
        if (vec.size() != 2 || vec[0] != op_prefix_idx) {
            continue;
        }
        op_idx_to_delete.push_back(vec[1]);
    }
    for (const auto &k: input_seq.to_sp()) {
        ulong ket_idx = k.label_idx();
        for (const auto &op_idx: op_idx_to_delete) {
            context.unlearn(op_idx, ket_idx);
        }
    }
    return Sequence("unlearned");
}

Sequence op_grid_simm2(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {
    std::string ket_name = "grid-simm";
    if (input_seq.size() > 0) {
        ket_name = input_seq.to_ket().label();
    }
    if (one.size() == 0 || two.size() == 0) { return Sequence(); }
    auto one_vec = one.to_ket().label_split_idx();
    auto two_vec = two.to_ket().label_split_idx();
    if (one_vec.size() != 2 || two_vec.size() != 2) { return Sequence(); }
    ulong op_prefix_idx = ket_map.get_idx("op");
    if (one_vec[0] != op_prefix_idx || two_vec[0] != op_prefix_idx) { return Sequence(); }
    ulong one_idx = one_vec[1];
    ulong two_idx = two_vec[1];
    std::vector<ulong> one_kets = context.relevant_kets(one_idx);
    std::vector<ulong> two_kets = context.relevant_kets(two_idx);
    if (one_kets.empty() || two_kets.empty()) { return Sequence(ket_name, 0); }
    unsigned int max_size = std::max(one_kets.size(), two_kets.size());
    std::unordered_set<ulong> m(one_kets.begin(), one_kets.end());  // Find the intersection of one_kets and two_kets:
    std::vector<ulong> intersection_kets;
    for (auto a: two_kets) {
        if (m.count(a)) {
            intersection_kets.push_back(a);
            m.erase(a);
        }
    }
    double r = 0;
    for (ulong pattern_idx: intersection_kets) {
        Sequence seq1 = context.recall(one_idx, pattern_idx)->to_seq();
        Sequence seq2 = context.recall(two_idx, pattern_idx)->to_seq();
        r += simm(seq1, seq2);
    }
    return Sequence(ket_name, r / max_size);
}

Superposition op_similar_grid(ContextList &context, const Sequence &input_seq, const Sequence &one) {
    if (input_seq.size() == 0 || one.size() == 0) { return Superposition(); }
    auto one_vec = one.to_ket().label_split_idx();
    ulong op_prefix_idx = ket_map.get_idx("op");
    if (one_vec.size() != 2 || one_vec[0] != op_prefix_idx) { return Superposition(); }
    std::vector<ulong> input_idx_vec;
    for (const auto &k: input_seq.to_sp()) {
        auto input_vec = k.label_split_idx();
        if (input_vec.size() != 2 || input_vec[0] != op_prefix_idx) {
            continue;
        }
        input_idx_vec.push_back(input_vec[1]);
    }

    if (input_idx_vec.empty()) { return Superposition(); }
    ulong one_idx = one_vec[1];
    std::vector<ulong> one_kets = context.relevant_kets(one_idx);
    if (one_kets.empty()) { return Superposition(); }
    Superposition result;
    for (ulong input_idx: input_idx_vec) {
        std::vector<ulong> input_kets = context.relevant_kets(input_idx);
        if (input_kets.empty()) {
            continue;
        }
        unsigned int max_size = std::max(one_kets.size(), input_kets.size());
        std::unordered_set<ulong> m(one_kets.begin(), one_kets.end());  // Find the intersection of one_kets and two_kets:
        std::vector<ulong> intersection_kets;
        for (auto a: input_kets) {
            if (m.count(a)) {
                intersection_kets.push_back(a);
                m.erase(a);
            }
        }
        double r = 0;
        for (ulong pattern_idx: intersection_kets) {
            Sequence seq1 = context.recall(one_idx, pattern_idx)->to_seq();
            Sequence seq2 = context.recall(input_idx, pattern_idx)->to_seq();
            r += simm(seq1, seq2);
        }
        if (r > 0) {
            result.add(input_idx, r / max_size);
        }
    }
    result.coeff_sort();
    return result;
}
