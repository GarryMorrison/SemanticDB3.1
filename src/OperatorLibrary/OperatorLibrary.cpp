//
// Created by Garry Morrison on 21/09/2020.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include "OperatorLibrary.h"
#include "../CompoundConstant/ConstantString.h"
#include "../Function/misc.h"
#include "FunctionOperatorLibrary.h"


Superposition split(const Ket k) {
    Superposition sp;
    for (const auto c: k.label()) {
        std::string s;
        s.push_back(c);
        sp.add(s, k.value());
    }
    return sp;
}

Superposition split(const Ket k, const std::string& delimiter) {
    if (delimiter.empty()) { return split(k); }
    Superposition sp;
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::string s = k.label();
    double value = k.value();

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        sp.add(token, value);
    }
    sp.add(s.substr(pos_start), value);
    return sp;
}

Superposition op_split(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    // if (parameters.empty()) { return Superposition(); }  // Maybe return split(k) instead?
    if (parameters.empty()) { return split(k); }
    // std::string delim = std::dynamic_pointer_cast<ConstantString>(parameters[0])->get_string();
    std::string delim = parameters[0]->get_string();
    return split(k, delim);
}

Sequence ssplit(const Ket k) {
    Sequence seq;
    for (const auto c: k.label()) {
        // std::cout << c << std::endl;
        std::string s;
        s.push_back(c);
        // Ket k2(s, k.value());
        seq.append(s, k.value());
    }
    return seq;
}

Sequence ssplit(const Ket k, const std::string& delimiter) {
    if (delimiter.empty()) { return ssplit(k); }
    Sequence seq;
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::string s = k.label();
    double value = k.value();

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);  // Maybe use string_view instead: https://www.youtube.com/watch?v=ZO68JEgoPeg
        pos_start = pos_end + delim_len;
        // Ket k2(token, value);
        seq.append(token, value);
    }
    // Ket k2(s.substr(pos_start), value);
    seq.append(s.substr(pos_start), value);
    return seq;
}

/*
Sequence ssplit(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    // if (parameters.empty()) { return Sequence(); }  // Maybe return ssplit(k) instead?
    if (parameters.empty()) { return ssplit(k); }
    // std::string delim = std::dynamic_pointer_cast<ConstantString>(parameters[0])->get_string();
    std::string delim = parameters[0]->get_string();
    return ssplit(k, delim);
}
*/

Sequence op_ssplit(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return ssplit(k); }
    std::string delim = parameters[0]->get_string();
    return ssplit(k, delim);
}

Superposition op_split_ket(const Ket k) {
    return split(k);
}

Sequence op_ssplit_ket(const Ket k) {
    return ssplit(k);
}

Ket extract_head(const Ket k) {
    ulong head_idx = ket_map.get_head_idx(k.label_idx());
    return Ket(head_idx, k.value());
}

Ket extract_tail(const Ket k) {
    ulong tail_idx = ket_map.get_tail_idx(k.label_idx());
    return Ket(tail_idx, k.value());
}

Ket extract_category(const Ket k) {
    ulong category_idx = ket_map.get_category_idx(k.label_idx());
    return Ket(category_idx, k.value());
}

Ket extract_value(const Ket k) {
    ulong value_idx = ket_map.get_value_idx(k.label_idx());
    return Ket(value_idx, k.value());
}

Ket push_float(const Ket k) {
    if (k.label_idx() == ket_map.get_idx("")) { return k;}
    std::string label = k.label();

    std::string s;
    if (label == " ") {
        s = "";
    } else {
        s = label + ": ";
    }
    return Ket(s + float_to_int(k.value(), default_decimal_places));
}

Ket pop_float(const Ket k) {
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); } // should never happen
    std::string value_str = ket_map.get_str(k_vec.back());
    try {
        double value = std::stod(value_str);
        if ( k_vec.size() == 1) {
            return Ket(" ", value);
        }
        k_vec.pop_back();
        ulong label_idx = ket_map.get_idx(k_vec);
        return Ket(label_idx, value * k.value());
    } catch (std::invalid_argument) {
        return k;
    }
}

Superposition rank(const Superposition& sp) {
    Superposition result;
    ulong pos = 0;
    for (const auto k: sp) {
        pos++;
        result.add(k.label_idx(), pos);
    }
    return result;
}

Sequence sp2seq(const Superposition &sp) {
    Sequence result;
    for (const auto k : sp) {
        result.append(k);
    }
    return result;
}

Superposition seq2sp(const Sequence &seq) {
    Superposition result;
    for (const auto &sp: seq) {
        result.add(sp);
    }
    return result;
}

Ket bar_chart(const Superposition &sp, const unsigned int width) {
    if (sp.size() == 0 || width <= 0) { return Ket(); }

    ulong max_len = 0;
    for (const auto k: sp) {
        max_len = std::max(k.label().size(), max_len);  // Is there a smarter way to do this?
    }
    // Superposition one = sp.rescale(width); // do we need .drop() too? Probably.
    Superposition one = sp.drop().rescale(width);
    std::cout << "----------" << std::endl;
    for (const auto k: one) {
        std::cout << std::left << std::setfill(' ') << std::setw(max_len) << k.label() << " : ";
        std::cout << std::setfill('|') << std::setw((ulong)k.value()) << "|" << std::endl;
    }
    std::cout << "----------" << std::endl;
    return Ket("bar chart");
}

Ket op_bar_chart(const Superposition &sp, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    unsigned int width = 10;
    if (parameters.empty()) {
        width = 10;
    } else {
        width = parameters[0]->get_int();
    }
    return bar_chart(sp, width);
}


Ket print_ket(const Ket k) {
    std::cout << k.label() << std::endl;
    return k;
}

Superposition op_relevant_kets(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Superposition(); }
    Superposition result;
    for (const auto &param: parameters ) {
        ulong op_idx = param->get_operator().get_idx();
        // Superposition tmp(context.relevant_kets(op_idx));
        // result.add(tmp);
        result.add(context.relevant_kets(op_idx));
    }
    return result;
}

Superposition op_similar_input(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Superposition(); }  // Alternatively, return seq.
    ulong op_idx = parameters[0]->get_operator().get_idx();
    std::vector<ulong> ket_vec = context.relevant_kets(op_idx);
    Superposition result;
    for (const ulong label_idx : ket_vec) {
        Sequence pattern = context.recall(op_idx, label_idx)->to_seq();  // active recall? Would that stomp on memoize rules too?
        double score = simm(seq, pattern);
        if (score > 0) {
            result.add(label_idx, score);
        }
    }
    result.coeff_sort();
    switch (parameters.size()) {
        case 1: { return result; }
        case 2: {
            int stop = parameters[1]->get_int();
            return result.select(1, stop);
            }
        case 3: {
            int start = parameters[1]->get_int();
            int stop = parameters[2]->get_int();
            return result.select(start, stop);
        }
        default: return Superposition();  // Alternatively, we could return seq.
    }
}

// Wow! A lot of work just to subtract a number!!
Ket op_minus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    long double value = parameters[0]->get_float();
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    if (split_idx.empty()) {
        return Ket(float_to_int(number - value, default_decimal_places), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(number - value, default_decimal_places), k.value());
    }
}

Ket op_plus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    long double value = parameters[0]->get_float();
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    if (split_idx.empty()) {
        return Ket(float_to_int(number + value, default_decimal_places), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(number + value, default_decimal_places), k.value());
    }
}

Ket op_times_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    long double value = parameters[0]->get_float();
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    if (split_idx.empty()) {
        return Ket(float_to_int(number * value, default_decimal_places), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(number * value, default_decimal_places), k.value());
    }
}

Ket op_divide_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    long double value = parameters[0]->get_float();
    if (double_eq(value, 0)) { return k; } // prevent divide by zero.
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    if (split_idx.empty()) {
        return Ket(float_to_int(number / value, default_decimal_places), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(number / value, default_decimal_places), k.value());
    }
}

Ket op_int_divide_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    long double value = parameters[0]->get_float();
    if (double_eq(value, 0)) { return k; } // prevent divide by zero.
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    if (split_idx.empty()) {
        return Ket(float_to_int((long long)(number / value), default_decimal_places), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(number / value, default_decimal_places), k.value());
    }
}

Ket op_round(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    unsigned int value = parameters[0]->get_int();
    if (double_eq(value, 0)) { return k; } // prevent divide by zero.
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    if (split_idx.empty()) {
        return Ket(float_to_int(number, value), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(number / value, default_decimal_places), k.value());
    }
}


Ket op_modulus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(); }  // Alternatively, return k.
    long double value = parameters[0]->get_float();
    auto split_idx = k.label_split_idx();
    long double number = std::stold(ket_map.get_str(split_idx.back()));  // Possibly wrap this in a try/catch.
    split_idx.pop_back();
    long long result = static_cast<long long>(number) % static_cast<long long>(value);
    if (split_idx.empty()) {
        return Ket(float_to_int(result, default_decimal_places), k.value());
    } else {
        std::string category = ket_map.get_str(split_idx) + ": ";
        return Ket(category + float_to_int(result, default_decimal_places), k.value());
    }
}

Ket op_toupper(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }
    std::string label = k.label();
    for (const auto &elt: parameters) {
        int idx = elt->get_int() - 1;  // to-upper[1] changes case of first char, not the second.
        if (idx < 0 || idx >= label.size()) {
            continue;
        }
        // if (label[idx] >= 'a' && label[idx] <= 'z') {  // assumes ASCII. Is there a better way to do this?
        //     label[idx] = label[idx] - 32;
        // }
        label[idx] = std::toupper(label[idx]);
    }
    return Ket(label, k.value());
}

Ket op_tolower(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }
    std::string label = k.label();
    for (const auto &elt: parameters) {
        int idx = elt->get_int() - 1;  // to-lower[1] changes case of first char, not the second.
        if (idx < 0 || idx >= label.size()) {
            continue;
        }
        // if (label[idx] >= 'A' && label[idx] <= 'Z') {  // assumes ASCII. Is there a better way to do this?
        //     label[idx] = label[idx] + 32;
        // }
        label[idx] = std::tolower(label[idx]);
    }
    return Ket(label, k.value());
}

Ket toupperket(const Ket k) {
    std::string label = k.label();
    std::transform(label.begin(), label.end(), label.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return Ket(label, k.value());
}

Ket tolowerket(const Ket k) {
    std::string label = k.label();
    std::transform(label.begin(), label.end(), label.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return Ket(label, k.value());
}


Ket op_table(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(""); }

    std::vector<SimpleOperator> operators;
    if (parameters.size() == 2) {
        if (parameters[1]->get_operator().to_string() == "*") {
            SimpleOperator op = parameters[0]->get_operator();
            operators.push_back(op);
            Superposition supported_ops;
            for (const auto &k: sp) {
                Superposition tmp_sp = context.recall("supported-ops", k.label())->to_sp(); // swap to idx version later.
                supported_ops.add(tmp_sp);
            }
            for (const auto &s_op: supported_ops) {
                ulong idx = s_op.label_split_idx()[1];  // Assumes our s_op are in form: |op: some-op>
                SimpleOperator op2(idx);
                operators.push_back(op2);
            }
        }
    }
    if (operators.empty()) {
        for (const auto &elt: parameters) {
            SimpleOperator op = elt->get_operator();
            operators.push_back(op);
        }
    }

    unsigned int width = operators.size();
    unsigned int height = sp.size();
    std::vector<std::string> header;
    header.reserve(width);
    std::vector<unsigned int> column_widths;
    column_widths.reserve(width);
    std::vector<std::string> table_body;
    table_body.reserve(width * height);

    for (const auto &op: operators) {
        std::string op_label = op.to_string();
        header.push_back(op_label);
        unsigned int column_width = op_label.size();
        column_widths.push_back(column_width);  // initial size of columns is the header widths
    }

    for (const auto &k: sp) {
        Sequence seq = k.to_seq();
        unsigned int idx = 0;
        for (const auto &op: operators) {
            std::string str;
            if (idx == 0) {
                str = k.label();
            } else {
                // str = op.Compile(context, seq).to_string();  // Swap in something better to display in a table than Sequence::to_string().
                str = op.Compile(context, seq).readable_display();
            }
            table_body.push_back(str);
            column_widths[idx] = std::max(column_widths[idx], (unsigned int)str.size());
            idx++;
        }
    }

    unsigned int idx = 0;
    for (const auto column_width: column_widths) {
        std::cout << "+" << std::left << std::setfill('-') << std::setw(column_width + 2) << "-";
    }
    std::cout << "+" << std::endl;
    for (const auto &str: header) {
        std::cout << "| " << std::left << std::setfill(' ') << std::setw(column_widths[idx]) << str << " ";
        idx++;
    }
    std::cout << "|" << std::endl;
    for (const auto column_width: column_widths) {
        std::cout << "+" << std::left << std::setfill('-') << std::setw(column_width + 2) << "-";
    }
    std::cout << "+" << std::endl;

    idx = 0;
    for (const auto &str: table_body) {
        std::cout << "| " << std::left << std::setfill(' ') << std::setw(column_widths[idx]) << str << " ";
        idx++;
        idx %= width;
        if (idx == 0) {
            std::cout << "|" << std::endl;
        }
    }

    for (const auto column_width: column_widths) {
        std::cout << "+" << std::left << std::setfill('-') << std::setw(column_width + 2) << "-";
    }
    std::cout << "+" << std::endl;
    return Ket("table");
}

Ket op_transpose_table(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return Ket(""); }

    std::vector<SimpleOperator> operators;
    if (parameters.size() == 2) {
        if (parameters[1]->get_operator().to_string() == "*") {
            SimpleOperator op = parameters[0]->get_operator();
            operators.push_back(op);
            Superposition supported_ops;
            for (const auto &k: sp) {
                Superposition tmp_sp = context.recall("supported-ops", k.label())->to_sp(); // swap to idx version later.
                supported_ops.add(tmp_sp);
            }
            for (const auto &s_op: supported_ops) {
                ulong idx = s_op.label_split_idx()[1];  // Assumes our s_op are in form: |op: some-op>
                SimpleOperator op2(idx);
                operators.push_back(op2);
            }
        }
    }
    if (operators.empty()) {
        for (const auto &elt: parameters) {
            SimpleOperator op = elt->get_operator();
            operators.push_back(op);
        }
    }

    unsigned int height = operators.size();
    unsigned int width = sp.size() + 1;
    std::vector<std::string> header;
    header.reserve(width);
    std::vector<unsigned int> column_widths;
    column_widths.reserve(width);
    std::vector<std::string> table_body;
    table_body.reserve(width * ( height - 1));

    header.push_back(operators[0].to_string());
    column_widths.push_back(operators[0].to_string().size());
    for (const auto &k: sp) {
        header.push_back(k.label());
        column_widths.push_back(k.label().size());
    }

    unsigned int idx = 0;
    std::string str;
    bool first_pass = true;
    for (const auto &op: operators) {
        if (first_pass) {
            first_pass = false;
            continue;
        } else {
            idx = 0;
            table_body.push_back(op.to_string());
            column_widths[0] = std::max(column_widths[0], (unsigned int) op.to_string().size());
            idx++;
            for (const auto &k: sp) {
                Sequence seq = k.to_seq();
                str = op.Compile(context, seq).readable_display();
                table_body.push_back(str);
                column_widths[idx] = std::max(column_widths[idx], (unsigned int) str.size());
                idx++;
            }
        }
    }

    for (const auto column_width: column_widths) {
        std::cout << "+" << std::left << std::setfill('-') << std::setw(column_width + 2) << "-";
    }
    std::cout << "+" << std::endl;
    idx = 0;
    for (const auto &str: header) {
        std::cout << "| " << std::left << std::setfill(' ') << std::setw(column_widths[idx]) << str << " ";
        idx++;
    }
    std::cout << "|" << std::endl;

    idx = 0;
    for (const auto &str: table_body) {
        std::cout << "| " << std::left << std::setfill(' ') << std::setw(column_widths[idx]) << str << " ";
        idx++;
        idx %= width;
        if (idx == 0) {
            std::cout << "|" << std::endl;
        }
    }

    for (const auto column_width: column_widths) {
        std::cout << "+" << std::left << std::setfill('-') << std::setw(column_width + 2) << "-";
    }
    std::cout << "+" << std::endl;

    return Ket("table");
}

Ket op_smerge(const Sequence &seq) {
    std::string s;
    for (const auto &sp: seq) {
        for (const auto &k: sp) {
            s += k.label();
        }
    }
    return Ket(s);
}

Ket op_smerge1(const Sequence &seq, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return op_smerge(seq); }
    std::string merge_str = parameters[0]->get_string();
    std::string s;
    bool first_pass = true;
    for (const auto &sp: seq) {
        for (const auto &k: sp) {
            if (first_pass) {
                s += k.label();
                first_pass = false;
            } else {
                s += merge_str + k.label();
            }
        }
    }
    return Ket(s);
}

ulong grid_element(const unsigned int x, const unsigned int y) {
    return ket_map.get_idx("grid: " + std::to_string(x) + ": " + std::to_string(y));
}

bool is_in_grid(const int x, const int y, const unsigned int width, const unsigned int height) {
    if (x < 0 || y < 0 || x >= width || y >= height) { return false; }
    return true;
}

Ket op_learn_grid(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.size() < 2) { return Ket(""); }
    unsigned int width = parameters[0]->get_int();
    unsigned int height = parameters[1]->get_int();

    ulong op_idx = ket_map.get_idx("value");
    if (parameters.size() == 3) {
        op_idx = parameters[2]->get_operator().get_idx();
    } else if (parameters.size() > 3) {  // later maybe handle more than one operator to zero out?
        return Ket("");
    }
    std::shared_ptr<BaseSequence> zero_seq = std::make_shared<Ket>("0");

    ulong north_idx = ket_map.get_idx("N");
    ulong north_east_idx = ket_map.get_idx("NE");
    ulong east_idx = ket_map.get_idx("E");
    ulong south_east_idx = ket_map.get_idx("SE");
    ulong south_idx = ket_map.get_idx("S");
    ulong south_west_idx = ket_map.get_idx("SW");
    ulong west_idx = ket_map.get_idx("W");
    ulong north_west_idx = ket_map.get_idx("NW");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ulong element_idx = grid_element(y,x);
            context.learn(op_idx, element_idx, zero_seq);
            if (is_in_grid(x, y - 1, width, height)) {
                ulong neighbour_idx = grid_element(y - 1, x);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(north_idx, element_idx, neighbour);
            }
            if (is_in_grid(x + 1, y - 1, width, height)) {
                ulong neighbour_idx = grid_element(y - 1, x + 1);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(north_east_idx, element_idx, neighbour);
            }
            if (is_in_grid(x + 1, y, width, height)) {
                ulong neighbour_idx = grid_element(y, x + 1);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(east_idx, element_idx, neighbour);
            }
            if (is_in_grid(x + 1, y + 1, width, height)) {
                ulong neighbour_idx = grid_element(y + 1, x + 1);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(south_east_idx, element_idx, neighbour);
            }
            if (is_in_grid(x, y + 1, width, height)) {
                ulong neighbour_idx = grid_element(y + 1, x);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(south_idx, element_idx, neighbour);
            }
            if (is_in_grid(x - 1, y + 1, width, height)) {
                ulong neighbour_idx = grid_element(y + 1, x - 1);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(south_west_idx, element_idx, neighbour);
            }
            if (is_in_grid(x - 1, y, width, height)) {
                ulong neighbour_idx = grid_element(y, x - 1);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(west_idx, element_idx, neighbour);
            }
            if (is_in_grid(x - 1, y - 1, width, height)) {
                ulong neighbour_idx = grid_element(y - 1, x - 1);
                std::shared_ptr<BaseSequence> neighbour = std::make_shared<Ket>(neighbour_idx);
                context.learn(north_west_idx, element_idx, neighbour);
            }
        }
    }
    return Ket("learn-grid");
}

Ket op_display_grid(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.size() < 2) { return Ket(""); }
    unsigned int width = parameters[0]->get_int();
    unsigned int height = parameters[1]->get_int();

    ulong op_idx = ket_map.get_idx("value");
    std::string empty_char = ".";
    if (parameters.size() == 3) {
        op_idx = parameters[2]->get_operator().get_idx();
    } else if (parameters.size() == 4) {
        empty_char = parameters[3]->get_string();
    } else if (parameters.size() > 4) {
        return Ket("");
    }

    std::string value_char;

    std::cout << "width:  " << width << "\n";
    std::cout << "height: " << height << "\n\n";
    for (int y = 0; y < height; y++) {
        std::cout << std::left << std::setw(4) << y;
        for (int x = 0; x < width; x++) {
            ulong element_idx = grid_element(y,x);
            Ket cell_value = context.recall(op_idx, element_idx)->to_ket();
            value_char = cell_value.label();
            if (value_char == " ") {
                value_char = float_to_int(cell_value.value(), default_decimal_places);
            } else if (value_char == "0") {
                value_char = empty_char;
            }
            std::cout << std::right << std::setw(4) << value_char;
        }
        std::cout << "\n";
    }
    std::cout << std::endl;  // Flush. Not sure if needed, but should be fine to leave it in.
    return Ket("display-grid");
}


Ket op_is_less_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }  // Maybe something different here.
    long double value = parameters[0]->get_float();
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); }
    try {
        long double ket_value = std::stold(ket_map.get_str(k_vec.back()));
        if (ket_value < value) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

Ket op_is_less_equal_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }  // Maybe something different here.
    long double value = parameters[0]->get_float();
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); }
    try {
        long double ket_value = std::stold(ket_map.get_str(k_vec.back()));
        if (ket_value <= value) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

Ket op_is_equal(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }  // Maybe something different here.
    long double value = parameters[0]->get_float();
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); }
    try {
        long double ket_value = std::stold(ket_map.get_str(k_vec.back()));
        if ( long_double_eq(ket_value, value)) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

Ket op_is_greater_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }  // Maybe something different here.
    long double value = parameters[0]->get_float();
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); }
    try {
        long double ket_value = std::stold(ket_map.get_str(k_vec.back()));
        if (ket_value > value) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

Ket op_is_greater_equal_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return k; }  // Maybe something different here.
    long double value = parameters[0]->get_float();
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); }
    try {
        long double ket_value = std::stold(ket_map.get_str(k_vec.back()));
        if (ket_value >= value) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

Ket op_is_in_range(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.size() < 2) { return k; }  // Maybe something different here.
    long double value1 = parameters[0]->get_float();
    long double value2 = parameters[1]->get_float();
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(); }
    try {
        long double ket_value = std::stold(ket_map.get_str(k_vec.back()));
        if (value1 <= ket_value && ket_value <= value2) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

bool is_prime(const long long p) {  // Just a naive is_prime() function.
    bool is_prime = true;
    if (p == 0 || p == 1) {
        is_prime = false;
    } else {
        for (long long i = 2; i <= p / 2; ++i) {
            if (p % i == 0) {
                is_prime = false;
                break;
            }
        }
    }
    return is_prime;
}

Ket op_is_prime(const Ket k) {
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(""); }
    try {
        long long ket_value = std::stoll(ket_map.get_str(k_vec.back()));  // Would be nice to not have to use try/catch here.
        if (is_prime(ket_value)) {
            return Ket("yes");
        }
        return Ket("no");
    } catch (const std::invalid_argument& e) {
        return Ket("");
    }
}

Superposition op_prime_factors(const Ket k) {  // Just a naive prime_factors() function.
    Superposition result;
    auto k_vec = k.label_split_idx();
    if (k_vec.empty()) { return Ket(""); }
    std::string ket_label = ket_map.get_str(k_vec.back());
    k_vec.pop_back();
    std::string category = ket_map.get_str(k_vec);
    if (!category.empty()) {
        category += ": ";
    }
    try {
        long long p = std::stoll(ket_label);  // Would be nice to not have to use try/catch here.
        while (p % 2 == 0) {
            result.add(category + "2");
            p /= 2;
        }
        for (long long i = 3; i <= sqrt(p); i += 2)
        {
            while (p % i == 0)
            {
                result.add(category + std::to_string(i));
                p /= i;
            }
        }
        if (p > 2) {
            result.add(category + std::to_string(p));
        }
        return result;
    } catch (const std::invalid_argument& e) {
        return Superposition("");
    }
}
