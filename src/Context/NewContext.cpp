//
// Created by Garry Morrison on 12/09/2020.
//

// Temp define Rule Type:
#define STOREDRULE 3
#define MEMOIZINGRULE 4

#include <iostream>
#include "NewContext.h"
#include "BoundFunction.h"


NewContext::NewContext(const std::string &s) {
    name = s;
}

/*
void NewContext::learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].learn(op_idx, bSeq);
}

void NewContext::learn(const std::string& op, const std::string& label, BaseSequence* bSeq){
    if (op == "supported-ops") { return;}
    if (bSeq->size() == 0) {return; }
    ulong op_idx, label_idx;
    // op_idx = ket_map.get_idx("op: " + op);
    op_idx = ket_map.get_idx(op);
    label_idx = ket_map.get_idx(label);
    Frame frame;

    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].learn(op_idx, bSeq);
}


void NewContext::learn(const std::string& op, const std::string& label, const std::string& srule){
    if (op == "supported-ops") { return;}
    if (srule == "") {return; }
    ulong op_idx, label_idx;
    // op_idx = ket_map.get_idx("op: " + op);
    op_idx = ket_map.get_idx(op);
    label_idx = ket_map.get_idx(label);
    Frame frame;

//     Superposition *sp = new Superposition(srule);  // Implement without using pointers!
//     BaseSequence* bSeq;
//     bSeq = sp;
    Sequence *seq = new Sequence(srule);  // Implement without using pointers!
    BaseSequence* bSeq;
    bSeq = seq;

    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].learn(op_idx, bSeq);
}

void NewContext::add_learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].add_learn(op_idx, bSeq);
}

void NewContext::add_learn(const std::string& op, const std::string& label, BaseSequence* bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    this->add_learn(op_idx, label_idx, bSeq);
}

void NewContext::add_learn(const std::string& op, const std::string& label, const std::string& srule) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);

    Sequence *seq = new Sequence(srule);  // Implement without using pointers!
    BaseSequence* bSeq;
    bSeq = seq;
    this->add_learn(op_idx, label_idx, bSeq);
}


void NewContext::seq_learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }  // Maybe we don't want this here for sequences?

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].seq_learn(op_idx, bSeq);
}

void NewContext::seq_learn(const std::string& op, const std::string& label, BaseSequence* bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    this->seq_learn(op_idx, label_idx, bSeq);
}

void NewContext::seq_learn(const std::string& op, const std::string& label, const std::string& srule) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);

    Sequence *seq = new Sequence(srule);  // Implement without using pointers!
    BaseSequence* bSeq;
    bSeq = seq;
    this->seq_learn(op_idx, label_idx, bSeq);
}


BaseSequence* NewContext::recall(const std::string& op, const std::string& label) {
    // ulong op_idx = ket_map.get_idx("op: " + op);
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    return this->recall(op_idx, label_idx);
}

BaseSequence* NewContext::recall(const ulong op_idx, const ulong label_idx) {
    BaseSequence* result;
    ulong trial_label_idx = label_idx;

    if (rules_dict.find(trial_label_idx) != rules_dict.end()) {
        result = rules_dict[trial_label_idx].recall(op_idx);
        if (result->size() != 0) { return result; };
    }

    ulong star_idx = ket_map.get_idx("*");  // implement label descent, not sure cost of this vs just splitting strings approach
    auto label_split_idx = ket_map.get_split_idx(trial_label_idx);
    while (!label_split_idx.empty()) {
        label_split_idx.pop_back();
        label_split_idx.push_back(star_idx);
        trial_label_idx = ket_map.get_idx(label_split_idx);
        // std::cout << "trial_label: " << ket_map.get_str(trial_label_idx) << std::endl;

        if (rules_dict.find(trial_label_idx) != rules_dict.end()) {
            result = rules_dict[trial_label_idx].recall(op_idx);
            if (result->size() != 0) { return result; };
        }
        label_split_idx.pop_back();
    }

    Superposition *sp = new Superposition();  // Memory leak??
    result = sp;

    return result;
}
*/

void NewContext::learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].learn(op_idx, bSeq);
}

void NewContext::learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq){
    if (op == "supported-ops") { return;}
    if (bSeq->size() == 0) {return; }
    ulong op_idx, label_idx;
    // op_idx = ket_map.get_idx("op: " + op);
    op_idx = ket_map.get_idx(op);
    label_idx = ket_map.get_idx(label);
    Frame frame;

    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].learn(op_idx, bSeq);
}


void NewContext::learn(const std::string& op, const std::string& label, const std::string& srule){
    if (op == "supported-ops") { return;}
    if (srule == "") {return; }
    ulong op_idx, label_idx;
    // op_idx = ket_map.get_idx("op: " + op);
    op_idx = ket_map.get_idx(op);
    label_idx = ket_map.get_idx(label);
    Frame frame;

//     Superposition *sp = new Superposition(srule);  // Implement without using raw pointers!
//     BaseSequence* bSeq;
//     bSeq = sp;
//     Sequence *seq = new Sequence(srule);  // Implement without using raw pointers!
//     BaseSequence* bSeq;
//     bSeq = seq;
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(srule);

    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].learn(op_idx, bSeq);
}

void NewContext::add_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].add_learn(op_idx, bSeq);
}

void NewContext::add_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    this->add_learn(op_idx, label_idx, bSeq);
}

void NewContext::add_learn(const std::string& op, const std::string& label, const std::string& srule) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);

//     Sequence *seq = new Sequence(srule);  // Implement without using raw pointers!
//     BaseSequence* bSeq;
//     bSeq = seq;
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(srule);
    this->add_learn(op_idx, label_idx, bSeq);
}


void NewContext::seq_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }  // Maybe we don't want this here for sequences?

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].seq_learn(op_idx, bSeq);
}

void NewContext::seq_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    this->seq_learn(op_idx, label_idx, bSeq);
}

void NewContext::seq_learn(const std::string& op, const std::string& label, const std::string& srule) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);

//     Sequence *seq = new Sequence(srule);  // Implement without using raw pointers!
//     BaseSequence* bSeq;
//     bSeq = seq;
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(srule);
    this->seq_learn(op_idx, label_idx, bSeq);
}

void NewContext::stored_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }  // Maybe we don't want this here for sequences or stored rules.

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].stored_learn(op_idx, bSeq);
}

void NewContext::stored_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    this->stored_learn(op_idx, label_idx, bSeq);
}

void NewContext::stored_learn(const std::string& op, const std::string& label, const std::string& srule) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(srule);
    this->stored_learn(op_idx, label_idx, bSeq);
}

void NewContext::memoize_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (op_idx == ket_map.get_idx("supported-ops")) { return; }  // maybe hard wire in "supported-ops" as ket_map idx = 1?
    if (bSeq->size() == 0) { return; }  // Maybe we don't want this here for sequences or stored rules.

    Frame frame;
    if (rules_dict.find(label_idx) == rules_dict.end()) {
        sort_order.push_back(label_idx);
        rules_dict[label_idx] = frame;
    }
    rules_dict[label_idx].memoize_learn(op_idx, bSeq);
}

void NewContext::memoize_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    this->memoize_learn(op_idx, label_idx, bSeq);
}

void NewContext::memoize_learn(const std::string& op, const std::string& label, const std::string& srule) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(srule);
    this->memoize_learn(op_idx, label_idx, bSeq);
}

void NewContext::unlearn(const ulong op_idx, const ulong label_idx) {
    if (rules_dict.find(label_idx) == rules_dict.end()) { return; }
    rules_dict[label_idx].unlearn(op_idx);
}

std::shared_ptr<BaseSequence> NewContext::recall(const std::string& op, const std::string& label) {
    // ulong op_idx = ket_map.get_idx("op: " + op);
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    return this->recall(op_idx, label_idx);
}

std::shared_ptr<BaseSequence> NewContext::recall(const ulong op_idx, const ulong label_idx) {
    std::shared_ptr<BaseSequence> result;
    ulong trial_label_idx = label_idx;

    if (rules_dict.find(trial_label_idx) != rules_dict.end()) {
        result = rules_dict[trial_label_idx].recall(op_idx);
        if (result->size() != 0) { return result; };
    }

    if (op_idx == ket_map.get_idx("supported-ops")) {  // We should have found supported-ops by now.
        return std::make_shared<Sequence>("");            // We don't want label descent for this case.
    }

    ulong star_idx = ket_map.get_idx("*");  // implement label descent, not sure cost of this vs just splitting strings approach
    auto label_split_idx = ket_map.get_split_idx(trial_label_idx);  // Maybe use recall_type() before jumping into label descent?
    while (!label_split_idx.empty()) {
        label_split_idx.pop_back();
        label_split_idx.push_back(star_idx);
        trial_label_idx = ket_map.get_idx(label_split_idx);
        // std::cout << "trial_label: " << ket_map.get_str(trial_label_idx) << std::endl;

        if (rules_dict.find(trial_label_idx) != rules_dict.end()) {
            result = rules_dict[trial_label_idx].recall(op_idx);
            if (result->size() != 0) { return result; };
        }
        label_split_idx.pop_back();
    }

    // std::shared_ptr<BaseSequence> result2;  // Caused a memory access bug!
    // std::shared_ptr<BaseSequence> result2 = std::make_shared<Sequence>();
    // return result2;
    return std::make_shared<Sequence>("");
}

unsigned int NewContext::recall_type(const std::string& op, const std::string& label) {
    // ulong op_idx = ket_map.get_idx("op: " + op);
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    return this->recall_type(op_idx, label_idx);
}

unsigned int NewContext::recall_type(const ulong op_idx, const ulong label_idx) {
    if (rules_dict.find(label_idx) != rules_dict.end()) {
        return rules_dict[label_idx].recall_type(op_idx);
    }
    return RULEUNDEFINED;
}

unsigned int NewContext::recall_descent_type(const std::string &op, const std::string &label) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    return this->recall_descent_type(op_idx, label_idx);
}

unsigned int NewContext::recall_descent_type(const ulong op_idx, const ulong label_idx) {  // I suspect this really slows things down!
    unsigned int result;
    if (rules_dict.find(label_idx) != rules_dict.end()) {
        result = rules_dict[label_idx].recall_type(op_idx);
        if (result != RULEUNDEFINED) { return result; }  // Is this correct? Should we just return result, no matter what it is??
    }
    ulong trial_label_idx = label_idx;
    ulong star_idx = ket_map.get_idx("*");  // implement label descent, not sure cost of this vs just splitting strings approach
    auto label_split_idx = ket_map.get_split_idx(trial_label_idx);
    while (!label_split_idx.empty()) {
        label_split_idx.pop_back();
        label_split_idx.push_back(star_idx);
        trial_label_idx = ket_map.get_idx(label_split_idx);

        if (rules_dict.find(trial_label_idx) != rules_dict.end()) {
            result = rules_dict[trial_label_idx].recall_type(op_idx);
            if (result != RULEUNDEFINED) { return result; }
        }
        label_split_idx.pop_back();
    }
    return RULEUNDEFINED;
}


std::vector<ulong> NewContext::relevant_kets(const ulong op_idx) {
    if (op_idx == ket_map.get_idx("*")) { return sort_order; }
    std::vector<ulong> result;
    std::copy_if (sort_order.begin(), sort_order.end(), std::back_inserter(result),
//                   [this, op_idx](ulong label_idx) {return this->recall_type(op_idx, label_idx) != RULEUNDEFINED; } );
             [this, op_idx](ulong label_idx) {return this->rules_dict[label_idx].recall_type(op_idx) != RULEUNDEFINED; } );
//     for (ulong label_idx: sort_order) {
//         if (this->recall_type(op_idx, label_idx) != RULEUNDEFINED) {
//             result.push_back(label_idx);
//         }
//     }
    return result;
}

std::vector<ulong> NewContext::relevant_kets(const std::string &op) {
    ulong op_idx = ket_map.get_idx(op);
    return this->relevant_kets(op_idx);
}

std::vector<ulong> NewContext::supported_ops(const ulong label_idx) {
    if (rules_dict.find(label_idx) != rules_dict.end()) {
        return rules_dict[label_idx].supported_ops();
    }
    std::vector<ulong> result;
    return result;
}


void NewContext::find_inverse(const ulong op_idx) {
    if (op_idx == ket_map.get_idx("*")) {
        std::vector<ulong> starting_sort_order(sort_order);  // So we don't stomp on loop condition.
        for (const auto &label_idx: starting_sort_order) {
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(label_idx);
            Frame frame = rules_dict.at(label_idx);
            for (const ulong working_op_idx: frame.supported_ops()) {
                ulong new_op_idx = ket_map.get_idx("inverse-" + ket_map.get_str(working_op_idx));
                if (frame.recall_type(working_op_idx) == RULENORMAL) {
                    Superposition rule = frame.recall(working_op_idx)->to_sp();
                    for (const auto &k: rule) {
                        this->add_learn(new_op_idx, k.label_idx(), bSeq);
                    }
                }
            }
        }
    } else {
        ulong new_op_idx = ket_map.get_idx("inverse-" + ket_map.get_str(op_idx));
        std::vector<ulong> starting_sort_order(sort_order);  // So we don't stomp on loop condition.
        for (const auto &the_label_idx: starting_sort_order) {
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(the_label_idx);
            Frame frame = rules_dict.at(the_label_idx);
            if (frame.recall_type(op_idx) == RULENORMAL) {
                Superposition rule = frame.recall(op_idx)->to_sp();
                for (const auto &k: rule) {
                    this->add_learn(new_op_idx, k.label_idx(), bSeq);
                }
            }
        }
    }
}

void NewContext::fn_learn(const ulong op_idx, const int param_size, std::shared_ptr<BaseSequence> bSeq) {
    Frame frame;
    if (fn_rules_dict.find(param_size) == fn_rules_dict.end()) {
        fn_rules_dict[param_size] = frame;
    }
    fn_rules_dict[param_size].stored_learn(op_idx, bSeq);
}

std::shared_ptr<BaseSequence> NewContext::fn_recall(const ulong op_idx, const int param_size) {
    if (fn_rules_dict.find(param_size) == fn_rules_dict.end()) {
        return std::make_shared<Sequence>();
    }
    return fn_rules_dict[param_size].recall(op_idx);
}

unsigned int NewContext::fn_recall_type(const ulong op_idx, const int param_size) {
    if (fn_rules_dict.find(param_size) == fn_rules_dict.end()) {
        return RULEUNDEFINED;
    }
    return fn_rules_dict[param_size].recall_type(op_idx);
}

void NewContext::bound_fn_learn(ulong op_idx, std::vector<ulong> &op_ket_idx_vec, std::shared_ptr<BaseSequence> bSeq) {
    unsigned int vec_size = op_ket_idx_vec.size() / 2 ;
    BoundFunction bound_function;
    bound_fn_rules_dict[{op_idx, vec_size}] = bound_function;
    bound_fn_rules_dict[{op_idx, vec_size}].learn(op_ket_idx_vec, bSeq);
}

/*  // Not yet sure how to implement this!
std::shared_ptr<BaseSequence> NewContext::bound_fn_recall(const ulong op_idx, const int param_size) {
    // test it is in the dict first. If not, return std::make_shared<Sequence>();
    std::vector<ulong> params = bound_fn_rules_dict[{op_idx, param_size}].get_params();
    std::shared_ptr<BaseSequence> bSeq = bound_fn_rules_dict[{op_idx, param_size}].get_body();
    for (unsigned int i = 0; i < params.size(); i += 2) {
        ulong var_op_idx = params[i];
        ulong ket_op_idx = params[i + 1];
        // unlearn first.

    }
    return bSeq;
}
*/

void NewContext::print_universe(bool clean, std::ostream& out) const {
    std::string s, op, label;
    ulong supported_ops_idx;
    Frame frame;
    std::shared_ptr<BaseSequence> rule;
    unsigned int rule_type;

    // supported_ops_idx = ket_map.get_idx("op: supported-ops");
    supported_ops_idx = ket_map.get_idx("supported-ops");

    s += "------------------------------------------\n";
    // s += "|context> => |context: " + name + ">\n\n";
    s += "|context> => |" + name + ">\n\n";

    for (const ulong label_idx: sort_order) {
        label = ket_map.get_str(label_idx);
        // frame = rules_dict[label_idx];
        frame = rules_dict.at(label_idx);
        if (!clean) {
            rule = frame.recall(supported_ops_idx);
            s += "supported-ops |" + label + "> => " + rule->to_string() + "\n";
        }
        for (const ulong op_idx: frame.supported_ops()) {
            // ulong op_split_idx = ket_map.get_split_idx(op_idx).back();
            // op = ket_map.get_str(op_split_idx);
            op = ket_map.get_str(op_idx);
            rule = frame.recall(op_idx);
            rule_type = frame.recall_type(op_idx);
            std::string rule_type_str;  // Move this to outer scope?
            if (rule_type == RULESTORED) {
                rule_type_str = "#";    // Potentially make this a char instead.
            }
            else if (rule_type == RULEMEMOIZE) {
                rule_type_str = "!";
            }
            s += op + " |" + label + "> " + rule_type_str + "=> " + rule->to_string() + "\n";
        }
        s += "\n";
    }
    s += "\n";

    for (int param_size = 1; param_size <= 4; param_size++) {
        switch(param_size) {
            case 1 : label = "(*)"; break;
            case 2 : label = "(*,*)"; break;
            case 3 : label = "(*,*,*)"; break;
            case 4 : label = "(*,*,*,*)"; break;
        }
        if (fn_rules_dict.find(param_size) != fn_rules_dict.end()) {
            frame = fn_rules_dict.at(param_size);
            if (!clean) {
                rule = frame.recall(supported_ops_idx);
                s += "supported-ops " + label + " => " + rule->to_string() + "\n";
            }
            for (const ulong op_idx : frame.supported_ops()) {
                s += ket_map.get_str(op_idx) + " " + label + " #=> " + frame.recall(op_idx)->to_string() + "\n";
            }
            s += "\n";
        }
    }

    // Now the bound functions:
    for(const auto &entry: bound_fn_rules_dict) {
        auto key_pair = entry.first;
        std::vector<ulong> params = entry.second.get_params();

        std::string string_params;
        bool first_pass = true;
        for( unsigned int i = 0; i < params.size(); i += 2) {
            ulong op_idx = params[i];
            ulong ket_idx = params[i + 1];
            if (first_pass) {
                string_params += ket_map.get_str(op_idx) + "|" + ket_map.get_str(ket_idx) + ">";
                first_pass = false;
            } else {
                string_params += ", " + ket_map.get_str(op_idx) + "|" + ket_map.get_str(ket_idx) + ">";
            }
        }
        s += ket_map.get_str(key_pair.first) + " {" + string_params + "} #=> " + entry.second.get_body()->to_string();
        s += "\n";
    }

    s += "------------------------------------------\n";
    out << s;
}
