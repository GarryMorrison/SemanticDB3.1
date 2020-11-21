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
    size_t size = std::min(seq1.size(), seq2.size());
    if (size == 0) { return 0; }
    double r = 0;
    auto seq1_iter = seq1.cbegin();
    auto seq2_iter = seq2.cbegin();
    for (; seq1_iter != seq1.end() and seq2_iter != seq2.end(); ++seq1_iter, ++seq2_iter) {
        r += simm(*seq1_iter, *seq2_iter);  // probably want scaled_simm() here instead.
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

// Filter could do with some optimization!
Sequence op_filter(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two) {

    Superposition result;
    // First branch:
    // NB: Can be expensive! Especially if rel-kets[*] is large, or supported-ops is large.
    // Eg: filter(|*>, |doctor> + |nurse>) rel-kets[*]
    if (one.to_ket().label_idx() == ket_map.get_idx("*")) {
        ulong supported_ops_idx = ket_map.get_idx("supported-ops");
        for (const auto &k: input_seq.to_sp()) {
            Superposition s_ops = context.recall(supported_ops_idx, k.label_idx())->to_sp();
            for (const auto &s_op: s_ops) {
                ulong result_idx = context.recall(s_op.label_split_idx()[1], k.label_idx())->to_ket().label_idx();
                for (const auto &k2: two.to_sp()) {
                    if (result_idx == k2.label_idx()) {
                        result.add(k.label_idx());
                        goto break_loop;
                    }
                }
            }
            break_loop:;
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
        for (const auto &k: input_seq.to_sp()) {
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
                result.add(k.label_idx());
            }
        }
    } else {
        for (const auto &k: input_seq.to_sp()) {
            Sequence seq = k.to_seq();
            for (auto it = operators.rbegin(); it != operators.rend(); ++it) {
                seq = (*it).Compile(context, seq);
            }
            ulong result_idx = seq.to_ket().label_idx();
            for (const auto &k2: two.to_sp()) {
                if (result_idx == k2.label_idx()) {
                    result.add(k.label_idx());
                    break;
                }
            }
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
