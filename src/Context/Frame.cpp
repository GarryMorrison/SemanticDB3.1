//
// Created by Garry Morrison on 12/09/2020.
//
#include <iostream>
#include "Frame.h"

/*
std::vector<ulong> Frame::supported_ops() {
    std::vector<ulong> result;
    result = supported_operators;
    return result;
}
*/

/*
void Frame::learn(const ulong op_idx, BaseSequence* bSeq) {
    if (rules.find(op_idx) == rules.end()) {
        supported_operators.push_back(op_idx);
    }
    // else {
    //     delete rules[op_idx];  // Do we need this here??
    // }
    rules[op_idx] = bSeq;
    return;
}

void Frame::add_learn(const ulong op_idx, BaseSequence* bSeq) {
    BaseSequence* rule;
    if (rules.find(op_idx) == rules.end()) {
        supported_operators.push_back(op_idx);
        rules[op_idx] = bSeq;
    }
    else {
        rule = rules[op_idx]->b_add(bSeq);  // I don't know if my use of pointers are correct here!
        delete rules[op_idx];
        rules[op_idx] = rule;
    }
    return;
}

void Frame::seq_learn(const ulong op_idx, BaseSequence* bSeq) {
    BaseSequence* rule;
    if (rules.find(op_idx) == rules.end()) {
        supported_operators.push_back(op_idx);
        rules[op_idx] = bSeq;
    }
    else {
        rule = rules[op_idx]->b_append(bSeq);  // I don't know if my use of pointers are correct here!
        delete rules[op_idx];
        rules[op_idx] = rule;
    }
    return;
}
*/

void Frame::learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (ptr_rules.find(op_idx) == ptr_rules.end()) {
        supported_operators.push_back(op_idx);
    }
    ptr_rules[op_idx] = bSeq;
    rule_types[op_idx] = RULENORMAL;
    return;
}

void Frame::add_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq) {
    std::shared_ptr<BaseSequence> rule;
    if (ptr_rules.find(op_idx) == ptr_rules.end()) {
        supported_operators.push_back(op_idx);
        ptr_rules[op_idx] = bSeq;
    }
    else {
        rule = ptr_rules[op_idx]->b_add(bSeq);
        ptr_rules[op_idx] = rule;
    }
    rule_types[op_idx] = RULENORMAL;
    return;
}

void Frame::seq_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq) {
    std::shared_ptr<BaseSequence> rule;
    if (ptr_rules.find(op_idx) == ptr_rules.end()) {
        supported_operators.push_back(op_idx);
        ptr_rules[op_idx] = bSeq;
    }
    else {
        rule = ptr_rules[op_idx]->b_append(bSeq);
        ptr_rules[op_idx] = rule;
    }
    rule_types[op_idx] = RULENORMAL;
    return;
}

void Frame::stored_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (ptr_rules.find(op_idx) == ptr_rules.end()) {
        supported_operators.push_back(op_idx);
    }
    ptr_rules[op_idx] = bSeq;
    rule_types[op_idx] = RULESTORED;
    return;
}

void Frame::memoize_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (ptr_rules.find(op_idx) == ptr_rules.end()) {
        supported_operators.push_back(op_idx);
    }
    ptr_rules[op_idx] = bSeq;
    rule_types[op_idx] = RULEMEMOIZE;
    return;
}


/*
BaseSequence* Frame::recall(const ulong op_idx) {
    BaseSequence* result;
    Superposition *sp = new Superposition(); // FIX! So doesn't need pointers. Also, is it a memory leak?
    // ulong supported_ops_idx = ket_map.get_idx("op: supported-ops");
    ulong supported_ops_idx = ket_map.get_idx("supported-ops");
    if (op_idx == supported_ops_idx) {
        for (const ulong op_idx2: supported_operators) {
            std::string s = "op: " + ket_map.get_str(op_idx2);
            sp->add(s);
        }
        result = sp;
        return result;
    }
    if (rules.find(op_idx) == rules.end()) {
        result = sp;
        return result;
    }
    delete sp;  // this branch doesn't need to keep sp
    result = rules[op_idx];
    return result;
}
*/

std::shared_ptr<BaseSequence> Frame::recall(const ulong op_idx) {
    // ulong supported_ops_idx = ket_map.get_idx("op: supported-ops");
    ulong supported_ops_idx = ket_map.get_idx("supported-ops");
    if (op_idx == supported_ops_idx) {
        Superposition sp;                                                           // Maybe we should be returning Sequence instead.
        for (const ulong op_idx2: supported_operators) {
            std::string s = "op: " + ket_map.get_str(op_idx2);
            sp.add(s);
        }
        // std::shared_ptr<BaseSequence> result = std::make_shared<Superposition>(sp);  // Maybe we should be returning Sequence instead.
        // return result;
        return std::make_shared<Sequence>(sp);
    }
    if (ptr_rules.find(op_idx) == ptr_rules.end()) {
        // std::shared_ptr<BaseSequence> result = std::make_shared<Superposition>();  // Maybe we should be returning Sequence instead.
        // return result;
        return std::make_shared<Sequence>("");
    }
    // std::shared_ptr<BaseSequence> result = ptr_rules[op_idx];  // Can we merge this into one line?
    // return result;
    return ptr_rules[op_idx];
}

unsigned int Frame::recall_type(const ulong op_idx) {
    if (rule_types.find(op_idx) == rule_types.end()) { return RULEUNDEFINED; }
    return rule_types[op_idx];
}

void Frame::unlearn(const ulong op_idx) {
    auto supported_ops_iter = find(supported_operators.begin(), supported_operators.end(), op_idx);
    if (supported_ops_iter == supported_operators.end()) { return; }
    if (ptr_rules.find(op_idx) == ptr_rules.end()) { return; }
    if (rule_types.find(op_idx) == rule_types.end()) { return; }

    supported_operators.erase(supported_ops_iter);
    ptr_rules.erase(op_idx);
    rule_types.erase(op_idx);
}

void Frame::print() {
    std::cout << "supported_operators:" << std::endl;
    for (const ulong op_idx: supported_operators) {
        std::cout << "    " << op_idx << " " << ket_map.get_str(op_idx) << std::endl;
    }
    std::cout << "rules:" << std::endl;
    for (const auto &pair: ptr_rules) {
        std::cout << "    " << pair.first << " " << pair.second->to_string() << std::endl;
    }
}
