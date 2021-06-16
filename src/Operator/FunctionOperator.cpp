//
// Created by Garry Morrison on 28/09/2020.
//

#include <memory>
#include <iostream>
#include "FunctionOperator.h"
#include "../FunctionOperatorMap/FunctionOperatorMap.h"

FunctionOperator::FunctionOperator(const ulong idx2, const OperatorWithSequence &param0) : idx(idx2) {
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param0));
}

FunctionOperator::FunctionOperator(const ulong idx2, const OperatorWithSequence &param0,
                                   const OperatorWithSequence &param1) : idx(idx2) {
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param0));
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param1));
}

FunctionOperator::FunctionOperator(const ulong idx2, const OperatorWithSequence &param0,
                                   const OperatorWithSequence &param1, const OperatorWithSequence &param2) : idx(idx2) {
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param0));
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param1));
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param2));
}

FunctionOperator::FunctionOperator(const ulong idx2, const OperatorWithSequence &param0,
                                   const OperatorWithSequence &param1, const OperatorWithSequence &param2,
                                   const OperatorWithSequence &param3) : idx(idx2) {
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param0));
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param1));
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param2));
    seq_vec.push_back(std::make_shared<OperatorWithSequence>(param3));
}

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
    return s + " ) ";
}

/*
Sequence FunctionOperator::Compile(NewContext& context, const Sequence& seq) const {
    Sequence result;  // Fill out later, if we ever use this code branch.
    return result;
}
*/

Sequence FunctionOperator::Compile(ContextList& context, const Sequence& seq) const {
    if (seq_vec.empty()) { return seq; }  // Maybe return |> instead??
    // std::cout << "FunctionOperator::Compile(context, seq):" << std::endl;
    // std::cout << "    seq: " << seq.to_string() << std::endl;

    // Compile the sequences:
    std::vector<Sequence> args; // specify size of args here. Ie, seq_vec.size(). Or maybe something more efficient?
    args.push_back(seq);
    for (const auto &param: seq_vec) {
        Sequence tmp = param->Compile(context);
        args.push_back(std::move(tmp));  // Use std::move here?
    }
    Ket empty_ket("");
    switch (seq_vec.size()) {
        case 1: { if (fn_map.whitelist_1.find(idx) != fn_map.whitelist_1.end()) {
                auto our_fn = fn_map.whitelist_1[idx];
                return our_fn(args[0], args[1]);
        } else if (fn_map.context_whitelist_1.find(idx) != fn_map.context_whitelist_1.end()) {
                auto our_fn = fn_map.context_whitelist_1[idx];
                return our_fn(context, args[0], args[1]);
        } else if (context.fn_recall_type(idx, 1) == RULESTORED) {
                auto rule = context.fn_recall(idx, 1);
                return rule->Compile(context, empty_ket, args);  // Currently set label_idx to 0. Not sure what else we can do.
        }
        break;
        }
        case 2: { if (fn_map.whitelist_2.find(idx) != fn_map.whitelist_2.end()) {
                auto our_fn = fn_map.whitelist_2[idx];
                return our_fn(args[0], args[1], args[2]);
            } else if (fn_map.context_whitelist_2.find(idx) != fn_map.context_whitelist_2.end()) {
                auto our_fn = fn_map.context_whitelist_2[idx];
                return our_fn(context, args[0], args[1], args[2]);
            } else if (context.fn_recall_type(idx, 2) == RULESTORED) {
                auto rule = context.fn_recall(idx, 2);
                return rule->Compile(context, empty_ket, args);  // Currently set label_idx to 0. Not sure what else we can do.
            }
            break;
        }
        case 3: { if (fn_map.whitelist_3.find(idx) != fn_map.whitelist_3.end()) {
                auto our_fn = fn_map.whitelist_3[idx];
                return our_fn(args[0], args[1], args[2], args[3]);
            } else if (fn_map.context_whitelist_3.find(idx) != fn_map.context_whitelist_3.end()) {
                auto our_fn = fn_map.context_whitelist_3[idx];
                return our_fn(context, args[0], args[1], args[2], args[3]);
            } else if (context.fn_recall_type(idx, 3) == RULESTORED) {
                auto rule = context.fn_recall(idx, 3);
                return rule->Compile(context, empty_ket, args);  // Currently set label_idx to 0. Not sure what else we can do.
            }
            break;
        }
        case 4: { if (fn_map.whitelist_4.find(idx) != fn_map.whitelist_4.end()) {
                auto our_fn = fn_map.whitelist_4[idx];
                return our_fn(args[0], args[1], args[2], args[3], args[4]);
            } else if (fn_map.context_whitelist_4.find(idx) != fn_map.context_whitelist_4.end()) {
                auto our_fn = fn_map.context_whitelist_4[idx];
                return our_fn(context, args[0], args[1], args[2], args[3], args[4]);
            } else if (context.fn_recall_type(idx, 4) == RULESTORED) {
                auto rule = context.fn_recall(idx, 4);
                return rule->Compile(context, empty_ket, args);  // Currently set label_idx to 0. Not sure what else we can do.
            }
            break;
        }
    }
    if (context.bound_fn_recall_type(idx, seq_vec.size()) == RULESTORED) {
        // std::cout << "found a bound function, size: " << seq_vec.size() << "\n";
        ulong input_idx = ket_map.get_idx("input");  // learn input|seq>. A little heavier and slower than I would like!
        ulong seq_idx = ket_map.get_idx("seq");      // Also, we have hard wired in the op/ket labels here. Is there a better way?
        // context.unlearn(input_idx, seq_idx);         // Not needed now we can learn empty sequences |>.

        std::shared_ptr<BaseSequence> bSeq_empty = std::make_shared<Ket>();  // Learn variables before we stomp on them.
        std::shared_ptr<BaseSequence> bSeq_tmp;
        std::vector<unsigned int> stored_type;
        std::vector<std::shared_ptr<BaseSequence> > stored_bseq;
        unsigned int rule_type = context.recall_type(input_idx, seq_idx);
        stored_type.push_back(rule_type);
        if (rule_type == RULEUNDEFINED) {
            stored_bseq.push_back(bSeq_empty);
        } else {
            bSeq_tmp = context.recall(input_idx, seq_idx);
            stored_bseq.push_back(bSeq_tmp);
        }
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[0]);
        context.learn(input_idx, seq_idx, bSeq);
        std::vector<ulong> params = context.bound_fn_params_recall(idx, seq_vec.size());
        for (unsigned int i = 0; i < params.size(); i += 2) {
            ulong op_idx = params[i];
            ulong ket_idx = params[i + 1];
            bSeq = std::make_shared<Sequence>(args[(i/2) + 1]);
            // context.unlearn(op_idx, ket_idx);           // A little bit expensive, but I think we need it if |> is fed as a parameter to our functions.
            // context.learn(op_idx, ket_idx, bSeq);  // In which case, the learn() will be a no-op, meaning previous values will be used. Which we don't want!
            rule_type = context.recall_type(op_idx, ket_idx);
            stored_type.push_back(rule_type);
            if (rule_type == RULEUNDEFINED) {
                stored_bseq.push_back(bSeq_empty);
            } else {
                bSeq_tmp = context.recall(op_idx, ket_idx);
                stored_bseq.push_back(bSeq_tmp);
            }
            context.learn(op_idx, ket_idx, bSeq);
        }
        auto rule = context.bound_fn_body_recall(idx, seq_vec.size());
        // return rule->Compile(context, empty_ket, args);
        Sequence result = rule->Compile(context, empty_ket, args);

        // Now restore the original variables:
        rule_type = stored_type[0];
        switch (rule_type) {
            case RULENORMAL: context.learn(input_idx, seq_idx, stored_bseq[0]); break;
            case RULESTORED: context.stored_learn(input_idx, seq_idx, stored_bseq[0]); break;
            case RULEMEMOIZE: context.memoize_learn(input_idx, seq_idx, stored_bseq[0]); break;
            case RULEUNDEFINED: context.unlearn(input_idx, seq_idx); break;
            default: break;
        }
        for (unsigned int i = 0; i < params.size(); i += 2) {
            ulong op_idx = params[i];
            ulong ket_idx = params[i + 1];
            rule_type = stored_type[i/2 + 1];
            switch (rule_type) {
                case RULENORMAL: context.learn(op_idx, ket_idx, stored_bseq[i/2 + 1]); break;
                case RULESTORED: context.stored_learn(op_idx, ket_idx, stored_bseq[i/2 + 1]); break;
                case RULEMEMOIZE: context.memoize_learn(op_idx, ket_idx, stored_bseq[i/2 + 1]); break;
                case RULEUNDEFINED: context.unlearn(op_idx, ket_idx); break;
                default: break;
            }
        }
        return result;
    }
    return Sequence();
}

Sequence FunctionOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket) const {
    if (seq_vec.empty()) { return seq; }  // Maybe return |> instead??
    // Compile the sequences:
    std::vector<Sequence> args; // specify size of args here. Ie, seq_vec.size(). Or maybe something more efficient?
    args.push_back(seq);
    for (const auto &param: seq_vec) {
        Sequence tmp = param->Compile(context, label_ket);
        args.push_back(std::move(tmp));  // Use std::move here?
    }
    switch (seq_vec.size()) {
        case 1: { if (fn_map.whitelist_1.find(idx) != fn_map.whitelist_1.end()) {
                auto our_fn = fn_map.whitelist_1[idx];
                return our_fn(args[0], args[1]);
            } else if (fn_map.context_whitelist_1.find(idx) != fn_map.context_whitelist_1.end()) {
                auto our_fn = fn_map.context_whitelist_1[idx];
                return our_fn(context, args[0], args[1]);
            } else if (context.fn_recall_type(idx, 1) == RULESTORED) {
                auto rule = context.fn_recall(idx, 1);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 2: { if (fn_map.whitelist_2.find(idx) != fn_map.whitelist_2.end()) {
                auto our_fn = fn_map.whitelist_2[idx];
                return our_fn(args[0], args[1], args[2]);
            } else if (fn_map.context_whitelist_2.find(idx) != fn_map.context_whitelist_2.end()) {
                auto our_fn = fn_map.context_whitelist_2[idx];
                return our_fn(context, args[0], args[1], args[2]);
            } else if (context.fn_recall_type(idx, 2) == RULESTORED) {
                auto rule = context.fn_recall(idx, 2);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 3: { if (fn_map.whitelist_3.find(idx) != fn_map.whitelist_3.end()) {
                auto our_fn = fn_map.whitelist_3[idx];
                return our_fn(args[0], args[1], args[2], args[3]);
            } else if (fn_map.context_whitelist_3.find(idx) != fn_map.context_whitelist_3.end()) {
                auto our_fn = fn_map.context_whitelist_3[idx];
                return our_fn(context, args[0], args[1], args[2], args[3]);
            } else if (context.fn_recall_type(idx, 3) == RULESTORED) {
                auto rule = context.fn_recall(idx, 3);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 4: { if (fn_map.whitelist_4.find(idx) != fn_map.whitelist_4.end()) {
                auto our_fn = fn_map.whitelist_4[idx];
                return our_fn(args[0], args[1], args[2], args[3], args[4]);
            } else if (fn_map.context_whitelist_4.find(idx) != fn_map.context_whitelist_4.end()) {
                auto our_fn = fn_map.context_whitelist_4[idx];
                return our_fn(context, args[0], args[1], args[2], args[3], args[4]);
            } else if (context.fn_recall_type(idx, 4) == RULESTORED) {
                auto rule = context.fn_recall(idx, 4);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
    }
    if (context.bound_fn_recall_type(idx, seq_vec.size()) == RULESTORED) {
        // std::cout << "found a bound function, size: " << seq_vec.size() << "\n";
        ulong input_idx = ket_map.get_idx("input");
        ulong seq_idx = ket_map.get_idx("seq");
        context.unlearn(input_idx, seq_idx);
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[0]);
        context.learn(input_idx, seq_idx, bSeq);
        std::vector<ulong> params = context.bound_fn_params_recall(idx, seq_vec.size());
        for (unsigned int i = 0; i < params.size(); i += 2) {
            ulong op_idx = params[i];
            ulong ket_idx = params[i + 1];
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[(i/2) + 1]);
            context.unlearn(op_idx, ket_idx);           // A little bit expensive, but I think we need it if |> is fed as a parameter to our functions.
            context.learn(op_idx, ket_idx, bSeq);  // In which case, the learn() will be a no-op, meaning previous values will be used. Which we don't want!
        }
        auto rule = context.bound_fn_body_recall(idx, seq_vec.size());
        return rule->Compile(context, label_ket, args);
    }
    return Sequence();
}


Sequence FunctionOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket, const Ket& multi_label_ket) const {
    if (seq_vec.empty()) { return seq; }  // Maybe return |> instead??
    // Compile the sequences:
    std::vector<Sequence> args; // specify size of args here. Ie, seq_vec.size(). Or maybe something more efficient?
    args.push_back(seq);
    for (const auto &param: seq_vec) {
        Sequence tmp = param->Compile(context, label_ket, multi_label_ket);
        args.push_back(std::move(tmp));  // Use std::move here?
    }
    switch (seq_vec.size()) {
        case 1: { if (fn_map.whitelist_1.find(idx) != fn_map.whitelist_1.end()) {
                auto our_fn = fn_map.whitelist_1[idx];
                return our_fn(args[0], args[1]);
            } else if (fn_map.context_whitelist_1.find(idx) != fn_map.context_whitelist_1.end()) {
                auto our_fn = fn_map.context_whitelist_1[idx];
                return our_fn(context, args[0], args[1]);
            } else if (context.fn_recall_type(idx, 1) == RULESTORED) {
                auto rule = context.fn_recall(idx, 1);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 2: { if (fn_map.whitelist_2.find(idx) != fn_map.whitelist_2.end()) {
                auto our_fn = fn_map.whitelist_2[idx];
                return our_fn(args[0], args[1], args[2]);
            } else if (fn_map.context_whitelist_2.find(idx) != fn_map.context_whitelist_2.end()) {
                auto our_fn = fn_map.context_whitelist_2[idx];
                return our_fn(context, args[0], args[1], args[2]);
            } else if (context.fn_recall_type(idx, 2) == RULESTORED) {
                auto rule = context.fn_recall(idx, 2);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 3: { if (fn_map.whitelist_3.find(idx) != fn_map.whitelist_3.end()) {
                auto our_fn = fn_map.whitelist_3[idx];
                return our_fn(args[0], args[1], args[2], args[3]);
            } else if (fn_map.context_whitelist_3.find(idx) != fn_map.context_whitelist_3.end()) {
                auto our_fn = fn_map.context_whitelist_3[idx];
                return our_fn(context, args[0], args[1], args[2], args[3]);
            } else if (context.fn_recall_type(idx, 3) == RULESTORED) {
                auto rule = context.fn_recall(idx, 3);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 4: { if (fn_map.whitelist_4.find(idx) != fn_map.whitelist_4.end()) {
                auto our_fn = fn_map.whitelist_4[idx];
                return our_fn(args[0], args[1], args[2], args[3], args[4]);
            } else if (fn_map.context_whitelist_4.find(idx) != fn_map.context_whitelist_4.end()) {
                auto our_fn = fn_map.context_whitelist_4[idx];
                return our_fn(context, args[0], args[1], args[2], args[3], args[4]);
            } else if (context.fn_recall_type(idx, 4) == RULESTORED) {
                auto rule = context.fn_recall(idx, 4);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
    }
    if (context.bound_fn_recall_type(idx, seq_vec.size()) == RULESTORED) {
        // std::cout << "found a bound function, size: " << seq_vec.size() << "\n";
        ulong input_idx = ket_map.get_idx("input");
        ulong seq_idx = ket_map.get_idx("seq");
        context.unlearn(input_idx, seq_idx);
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[0]);
        context.learn(input_idx, seq_idx, bSeq);
        std::vector<ulong> params = context.bound_fn_params_recall(idx, seq_vec.size());
        for (unsigned int i = 0; i < params.size(); i += 2) {
            ulong op_idx = params[i];
            ulong ket_idx = params[i + 1];
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[(i/2) + 1]);
            context.unlearn(op_idx, ket_idx);           // A little bit expensive, but I think we need it if |> is fed as a parameter to our functions.
            context.learn(op_idx, ket_idx, bSeq);  // In which case, the learn() will be a no-op, meaning previous values will be used. Which we don't want!
        }
        auto rule = context.bound_fn_body_recall(idx, seq_vec.size());
        return rule->Compile(context, label_ket, args);
    }
    return Sequence();
}

Sequence FunctionOperator::Compile(ContextList &context, const Sequence &seq, const Ket& label_ket, const std::vector<Sequence> &args1) const {
    if (seq_vec.empty()) { return seq; }  // Maybe return |> instead??
    // Compile the sequences:
    std::vector<Sequence> args; // specify size of args here. Ie, seq_vec.size(). Or maybe something more efficient?
    args.push_back(seq);
    for (const auto &param: seq_vec) {
        Sequence tmp = param->Compile(context, label_ket, args1);
        args.push_back(std::move(tmp));  // Use std::move here?
    }
    switch (seq_vec.size()) {
        case 1: { if (fn_map.whitelist_1.find(idx) != fn_map.whitelist_1.end()) {
                auto our_fn = fn_map.whitelist_1[idx];
                return our_fn(args[0], args[1]);
            } else if (fn_map.context_whitelist_1.find(idx) != fn_map.context_whitelist_1.end()) {
                auto our_fn = fn_map.context_whitelist_1[idx];
                return our_fn(context, args[0], args[1]);
            } else if (context.fn_recall_type(idx, 1) == RULESTORED) {
                auto rule = context.fn_recall(idx, 1);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 2: { if (fn_map.whitelist_2.find(idx) != fn_map.whitelist_2.end()) {
                auto our_fn = fn_map.whitelist_2[idx];
                return our_fn(args[0], args[1], args[2]);
            } else if (fn_map.context_whitelist_2.find(idx) != fn_map.context_whitelist_2.end()) {
                auto our_fn = fn_map.context_whitelist_2[idx];
                return our_fn(context, args[0], args[1], args[2]);
            } else if (context.fn_recall_type(idx, 2) == RULESTORED) {
                auto rule = context.fn_recall(idx, 2);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 3: { if (fn_map.whitelist_3.find(idx) != fn_map.whitelist_3.end()) {
                auto our_fn = fn_map.whitelist_3[idx];
                return our_fn(args[0], args[1], args[2], args[3]);
            } else if (fn_map.context_whitelist_3.find(idx) != fn_map.context_whitelist_3.end()) {
                auto our_fn = fn_map.context_whitelist_3[idx];
                return our_fn(context, args[0], args[1], args[2], args[3]);
            } else if (context.fn_recall_type(idx, 3) == RULESTORED) {
                auto rule = context.fn_recall(idx, 3);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
        case 4: { if (fn_map.whitelist_4.find(idx) != fn_map.whitelist_4.end()) {
                auto our_fn = fn_map.whitelist_4[idx];
                return our_fn(args[0], args[1], args[2], args[3], args[4]);
            } else if (fn_map.context_whitelist_4.find(idx) != fn_map.context_whitelist_4.end()) {
                auto our_fn = fn_map.context_whitelist_4[idx];
                return our_fn(context, args[0], args[1], args[2], args[3], args[4]);
            } else if (context.fn_recall_type(idx, 4) == RULESTORED) {
                auto rule = context.fn_recall(idx, 4);
                return rule->Compile(context, label_ket, args);
            }
            break;
        }
    }
    if (context.bound_fn_recall_type(idx, seq_vec.size()) == RULESTORED) {
        // std::cout << "found a bound function, size: " << seq_vec.size() << "\n";
        ulong input_idx = ket_map.get_idx("input");
        ulong seq_idx = ket_map.get_idx("seq");
        context.unlearn(input_idx, seq_idx);
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[0]);
        context.learn(input_idx, seq_idx, bSeq);
        std::vector<ulong> params = context.bound_fn_params_recall(idx, seq_vec.size());
        for (unsigned int i = 0; i < params.size(); i += 2) {
            ulong op_idx = params[i];
            ulong ket_idx = params[i + 1];
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(args[(i/2) + 1]);
            context.unlearn(op_idx, ket_idx);           // A little bit expensive, but I think we need it if |> is fed as a parameter to our functions.
            context.learn(op_idx, ket_idx, bSeq);  // In which case, the learn() will be a no-op, meaning previous values will be used. Which we don't want!
        }
        auto rule = context.bound_fn_body_recall(idx, seq_vec.size());
        return rule->Compile(context, label_ket, args);
    }
    return Sequence();
}
