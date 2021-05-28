//
// Created by Garry Morrison on 26/05/2021.
//

#include <math.h>
#include "InfixOperator.h"
#include "../Function/misc.h"
#include "../OperatorLibrary/FunctionOperatorLibrary.h"


InfixOperator::InfixOperator(const OperatorWithSequence &param0, int infix_type2, const OperatorWithSequence &param1) {
    one = std::make_shared<OperatorWithSequence>(param0);
    infix_type = infix_type2;
    two = std::make_shared<OperatorWithSequence>(param1);
}

const std::string InfixOperator::to_string() const {
    std::string str_type;
    switch (infix_type) {
        case OPEQUAL: str_type = " == "; break;
        case OPNOTEQUAL: str_type = " != "; break;
        case OPAND: str_type = " && "; break;
        case OPOR: str_type = " || "; break;
        case OPPLUS: str_type = " ++ "; break;
        case OPMINUS: str_type = " -- "; break;
        case OPMULT: str_type = " ** "; break;
        case OPDIV: str_type = " // "; break;
        case OPMOD: str_type = " %% "; break;
        case OPPOWER: str_type = " ^^ "; break;
        case OPRANGE: str_type = " .. "; break;
        default: str_type = " ?? ";
    }
    return "( " + one->to_string() + str_type + two->to_string() + " )";
}

Sequence InfixOperator::Compile(ContextList &context, const Sequence &seq) const {
    Sequence seq_one = one->Compile(context);
    Sequence seq_two = two->Compile(context);
    // std::cout << "InfixOperator::Compile(context, seq):\n";
    // std::cout << "    seq_one: " << seq_one.to_string() << "\n";
    // std::cout << "    seq_two: " << seq_two.to_string() << "\n";
    return process_compile(seq_one, seq_two);
}

Sequence InfixOperator::Compile(ContextList &context, const Sequence &seq, const Ket &label_ket) const {
    Sequence seq_one = one->Compile(context, label_ket);
    Sequence seq_two = two->Compile(context, label_ket);
    // std::cout << "InfixOperator::Compile(context, seq, label_ket):\n";
    // std::cout << "    seq_one: " << seq_one.to_string() << "\n";
    // std::cout << "    seq_two: " << seq_two.to_string() << "\n";
    return process_compile(seq_one, seq_two);
}

Sequence InfixOperator::Compile(ContextList &context, const Sequence &seq, const Ket &label_ket, const Ket &multi_label_ket) const {
    Sequence seq_one = one->Compile(context, label_ket, multi_label_ket);
    Sequence seq_two = two->Compile(context, label_ket, multi_label_ket);
    // std::cout << "InfixOperator::Compile(context, seq, label_ket, multi_label_ket):\n";
    // std::cout << "    seq_one: " << seq_one.to_string() << "\n";
    // std::cout << "    seq_two: " << seq_two.to_string() << "\n";
    return process_compile(seq_one, seq_two);
}

Sequence InfixOperator::Compile(ContextList &context, const Sequence &seq, const Ket &label_ket, const std::vector<Sequence> &args) const {
    Sequence seq_one = one->Compile(context, label_ket, args);
    Sequence seq_two = two->Compile(context, label_ket, args);
    // std::cout << "InfixOperator::Compile(context, seq, label_ket, args):\n";
    // std::cout << "    seq_one: " << seq_one.to_string() << "\n";
    // std::cout << "    seq_two: " << seq_two.to_string() << "\n";
    return process_compile(seq_one, seq_two);
}

Sequence InfixOperator::process_compile(const Sequence &seq_one, const Sequence &seq_two) const {
    ulong yes_idx = ket_map.get_idx("yes");

    switch (infix_type) {
        case OPEQUAL: {
            if (seq_one == seq_two) { return Sequence("yes"); }
            return Sequence("no");
        }
        case OPNOTEQUAL: {
            if (!(seq_one == seq_two)) { return Sequence("yes"); }
            return Sequence("no");
        }
        case OPAND: {
            if (seq_one.to_ket().label_idx() == yes_idx && seq_two.to_ket().label_idx() == yes_idx) { return Sequence("yes"); }
            return Sequence("no");
        }
        case OPOR: {
            if (seq_one.to_ket().label_idx() == yes_idx || seq_two.to_ket().label_idx() == yes_idx) { return Sequence("yes"); }
            return Sequence("no");
        }
        case OPPLUS:
        case OPMINUS:
        case OPMULT:
        case OPDIV:
        case OPMOD:
        case OPARITHPOWER: {
            // std::cout << "    arithmetic section:\n";
            auto one_idx_vec = seq_one.to_ket().label_split_idx();  // Handle more than just kets later! Ie, arithmetic over superpositions and sequences.
            auto two_idx_vec = seq_two.to_ket().label_split_idx();
            if (one_idx_vec.empty() || two_idx_vec.empty()) { return Sequence(); }
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
                        return Sequence();
                }
                return Sequence(label + float_to_int(value, default_decimal_places));
            } catch (const std::invalid_argument& e) {
                return Sequence();
            }
        }
        case OPRANGE: {
            Sequence empty;
            return op_range2(empty, seq_one, seq_two);  // Should we inline it, or leave as a function call?
        }
        default: return Sequence("unimplemented");
    }
}
