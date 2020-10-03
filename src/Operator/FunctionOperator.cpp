//
// Created by Garry Morrison on 28/09/2020.
//

#include "FunctionOperator.h"
#include "../FunctionOperatorMap/FunctionOperatorMap.h"

const std::string FunctionOperator::to_string() const {
    std::string s = ket_map.get_str(idx) + "( ";
    bool first_pass = true;
    for (const auto &seq: seq_vec) {
        if (first_pass) {
            s += seq->to_string();
            first_pass = false;
        } else {
            s += ", " + seq->to_string();
        }
    }
    return s + " )";
}

/*
Sequence FunctionOperator::Compile(NewContext& context, const Sequence& seq) const {
    Sequence result;  // Fill out later, if we ever use this code branch.
    return result;
}
*/

Sequence FunctionOperator::Compile(ContextList& context, const Sequence& seq) const {
    if (seq_vec.empty()) { return seq; }  // Maybe return |> instead??
    // Compile the sequences:
    std::vector<Sequence> args; // specify size of args here. Ie, seq_vec.size(). Or maybe something more efficient?
    for (const auto &param: seq_vec) {
        Sequence tmp = param->Compile(context);
        args.push_back(std::move(tmp));  // Use std::move here?
    }
    switch (args.size()) {
        case 1: { if (fn_map.whitelist_1.find(idx) != fn_map.whitelist_1.end()) {
                auto our_fn = fn_map.whitelist_1[idx];
                return our_fn(seq, args[0]);
        } else if (fn_map.context_whitelist_1.find(idx) != fn_map.context_whitelist_1.end()) {
                auto our_fn = fn_map.context_whitelist_1[idx];
                return our_fn(context, seq, args[0]);
        }
        break;
        }
        case 2: { if (fn_map.whitelist_2.find(idx) != fn_map.whitelist_2.end()) {
                auto our_fn = fn_map.whitelist_2[idx];
                return our_fn(seq, args[0], args[1]);
            } else if (fn_map.context_whitelist_2.find(idx) != fn_map.context_whitelist_2.end()) {
                auto our_fn = fn_map.context_whitelist_2[idx];
                return our_fn(context, seq, args[0], args[1]);
            }
            break;
        }
        case 3: { if (fn_map.whitelist_3.find(idx) != fn_map.whitelist_3.end()) {
                auto our_fn = fn_map.whitelist_3[idx];
                return our_fn(seq, args[0], args[1], args[2]);
            } else if (fn_map.context_whitelist_3.find(idx) != fn_map.context_whitelist_3.end()) {
                auto our_fn = fn_map.context_whitelist_3[idx];
                return our_fn(context, seq, args[0], args[1], args[2]);
            }
            break;
        }
        case 4: { if (fn_map.whitelist_4.find(idx) != fn_map.whitelist_4.end()) {
                auto our_fn = fn_map.whitelist_4[idx];
                return our_fn(seq, args[0], args[1], args[2], args[3]);
            } else if (fn_map.context_whitelist_4.find(idx) != fn_map.context_whitelist_4.end()) {
                auto our_fn = fn_map.context_whitelist_4[idx];
                return our_fn(context, seq, args[0], args[1], args[2], args[3]);
            }
            break;
        }
    }
    return Sequence();  // add fn_recall branch here.
}

Sequence FunctionOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    if (seq_vec.empty()) { return seq; }  // Maybe return |> instead??
    // Compile the sequences:
    std::vector<Sequence> args; // specify size of args here. Ie, seq_vec.size(). Or maybe something more efficient?
    for (const auto &param: seq_vec) {
        Sequence tmp = param->Compile(context, label_idx);
        args.push_back(std::move(tmp));  // Use std::move here?
    }
    switch (args.size()) {
        case 1: { if (fn_map.whitelist_1.find(idx) != fn_map.whitelist_1.end()) {
                auto our_fn = fn_map.whitelist_1[idx];
                return our_fn(seq, args[0]);
            } else if (fn_map.context_whitelist_1.find(idx) != fn_map.context_whitelist_1.end()) {
                auto our_fn = fn_map.context_whitelist_1[idx];
                return our_fn(context, seq, args[0]);
            }
            break;
        }
        case 2: { if (fn_map.whitelist_2.find(idx) != fn_map.whitelist_2.end()) {
                auto our_fn = fn_map.whitelist_2[idx];
                return our_fn(seq, args[0], args[1]);
            } else if (fn_map.context_whitelist_2.find(idx) != fn_map.context_whitelist_2.end()) {
                auto our_fn = fn_map.context_whitelist_2[idx];
                return our_fn(context, seq, args[0], args[1]);
            }
            break;
        }
        case 3: { if (fn_map.whitelist_3.find(idx) != fn_map.whitelist_3.end()) {
                auto our_fn = fn_map.whitelist_3[idx];
                return our_fn(seq, args[0], args[1], args[2]);
            } else if (fn_map.context_whitelist_3.find(idx) != fn_map.context_whitelist_3.end()) {
                auto our_fn = fn_map.context_whitelist_3[idx];
                return our_fn(context, seq, args[0], args[1], args[2]);
            }
            break;
        }
        case 4: { if (fn_map.whitelist_4.find(idx) != fn_map.whitelist_4.end()) {
                auto our_fn = fn_map.whitelist_4[idx];
                return our_fn(seq, args[0], args[1], args[2], args[3]);
            } else if (fn_map.context_whitelist_4.find(idx) != fn_map.context_whitelist_4.end()) {
                auto our_fn = fn_map.context_whitelist_4[idx];
                return our_fn(context, seq, args[0], args[1], args[2], args[3]);
            }
            break;
        }
    }
    return Sequence();  // add fn_recall branch here.
}
