//
// Created by Garry Morrison on 12/09/2020.
//

// Temp define Rule Type:
#define STOREDRULE 3
#define MEMOIZINGRULE 4

#include <iostream>
#include "NewContext.h"

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
    return std::make_shared<Sequence>();
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

    s += "------------------------------------------\n";
    out << s;
}
