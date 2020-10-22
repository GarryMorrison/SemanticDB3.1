//
// Created by Garry Morrison on 13/09/2020.
//

#include <iostream>
#include "ContextList.h"
#include "../Sequence/OperatorWithSequence.h"
#include "../Sequence/SelfKet.h"

ContextList::ContextList(const std::string& s) {
    name = s;
    index = 0;
    max_index = 1;
    NewContext tmp(name);
    data.push_back(tmp);
}

void ContextList::set(const std::string& s) {
    bool match = false;
    for (ulong k = 0; k < max_index; k++ ) {
        if (data[k].get_name() == s) {
            index = k;
            match = true;
            break;
        }
    }
    if (!match) {
        max_index++;
        index = max_index - 1;
        NewContext tmp(s);
        data.push_back(tmp);
    }
}

/*
void ContextList::learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq) {
    data[index].learn(op_idx, label_idx, bSeq);
}

void ContextList::learn(const std::string& op, const std::string& label, BaseSequence* bSeq) {
    data[index].learn(op, label, bSeq);
}

void ContextList::learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].learn(op, label, srule);
}


void ContextList::add_learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq) {
    data[index].add_learn(op_idx, label_idx, bSeq);
}

void ContextList::add_learn(const std::string& op, const std::string& label, BaseSequence* bSeq) {
    data[index].add_learn(op, label, bSeq);
}

void ContextList::add_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].add_learn(op, label, srule);
}

void ContextList::seq_learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq) {
    data[index].seq_learn(op_idx, label_idx, bSeq);
}

void ContextList::seq_learn(const std::string& op, const std::string& label, BaseSequence* bSeq) {
    data[index].seq_learn(op, label, bSeq);
}

void ContextList::seq_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].seq_learn(op, label, srule);
}

BaseSequence* ContextList::recall(const std::string& op, const std::string& label) {
    return data[index].recall(op, label);
}

BaseSequence* ContextList::recall(const ulong op_idx, const ulong label_idx) {
    return data[index].recall(op_idx, label_idx);
}
*/

void ContextList::learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        // Sequence result = bSeq->Compile(*this);
        // Sequence result;
        // Sequence result = bSeq->Compile(this->data[index]);  // This seems to be bug free so far.
        // Sequence result = bSeq->Compile(this->data[index], label_idx);
        Sequence result = bSeq->Compile(*this, label_idx);  // Maybe this should be in LearnRule instead??
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);  // should we merge these two lines?
        data[index].learn(op_idx, label_idx, bSeq2);
    } else {
        data[index].learn(op_idx, label_idx, bSeq);
    }
}

void ContextList::learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {
    // if (bSeq->type() == OPERATORWITHSEQUENCE) {
    // if (bSeq->type() == SELFKET) {
        // Sequence result = bSeq->Compile(*this);
        // Sequence result = bSeq->Compile(*this, label_idx);
        // Sequence result = bSeq->Compile(this->data[index]);
        // Sequence result = bSeq->Compile(this->data[index], label_idx);
        Sequence result = bSeq->Compile(*this, label_idx);
        //Sequence result;
        // std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>();
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);  // should we merge these two lines?
        data[index].learn(op_idx, label_idx, bSeq2);
    //} else if (bSeq->type() == SELFKET) {  // merge with OPERATORWITHSEQUENCE branch
    } else if (bSeq->type() == OPERATORWITHSEQUENCE && false) {  // merge with OPERATORWITHSEQUENCE branch
        // Sequence result = bSeq->Compile(*this, label_idx);
        // Sequence result;
        // Sequence result = bSeq->Compile(this->data[index]);
        // Sequence result = bSeq->Compile(this->data[index], label_idx);
        Sequence result = bSeq->Compile(*this, label_idx);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);  // should we merge these two lines?
        data[index].learn(op_idx, label_idx, bSeq2);
    } else {
        data[index].learn(op_idx, label_idx, bSeq);
    }
}

void ContextList::learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].learn(op, label, srule);
}


void ContextList::add_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    data[index].add_learn(op_idx, label_idx, bSeq);
}

void ContextList::add_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    data[index].add_learn(op, label, bSeq);
}

void ContextList::add_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].add_learn(op, label, srule);
}

void ContextList::seq_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    data[index].seq_learn(op_idx, label_idx, bSeq);
}

void ContextList::seq_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    data[index].seq_learn(op, label, bSeq);
}

void ContextList::seq_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].seq_learn(op, label, srule);
}

void ContextList::stored_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    data[index].stored_learn(op_idx, label_idx, bSeq);
}

void ContextList::stored_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    data[index].stored_learn(op, label, bSeq);
}

void ContextList::stored_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].stored_learn(op, label, srule);
}

void ContextList::memoize_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    data[index].memoize_learn(op_idx, label_idx, bSeq);
}

void ContextList::memoize_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    data[index].memoize_learn(op, label, bSeq);
}

void ContextList::memoize_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].memoize_learn(op, label, srule);
}

std::shared_ptr<BaseSequence> ContextList::recall(const std::string& op, const std::string& label) {
    return data[index].recall(op, label);
}

std::shared_ptr<BaseSequence> ContextList::recall(const ulong op_idx, const ulong label_idx) {
    return data[index].recall(op_idx, label_idx);
}

unsigned int ContextList::recall_type(const std::string& op, const std::string& label) {
    return data[index].recall_type(op, label);
}

unsigned int ContextList::recall_type(const ulong op_idx, const ulong label_idx) {
    return data[index].recall_type(op_idx, label_idx);
}

std::vector<ulong> ContextList::relevant_kets(const ulong op_idx) {
    return data[index].relevant_kets(op_idx);
}

std::vector<ulong> ContextList::relevant_kets(const std::string &op) {
    ulong op_idx = ket_map.get_idx(op);
    return data[index].relevant_kets(op_idx);
}


Sequence ContextList::active_recall(const std::string& op, const std::string& label) {
    // return data[index].recall(op, label)->Compile(this->data[index]);  // replace with label_idx version
    ulong op_idx = ket_map.get_idx(op);
    ulong label_idx = ket_map.get_idx(label);
    // return data[index].recall(op_idx, label_idx)->Compile(this->data[index], label_idx);
    // return data[index].recall(op_idx, label_idx)->Compile(*this, label_idx);
    // return Sequence("fish");
    // return Sequence();
    // std::shared_ptr<BaseSequence> recall_result = data[index].recall(op_idx, label_idx);
    // if (recall_result->size() > 0 && false) {
    //     return recall_result->Compile(*this, label_idx);
    // }
    // return Sequence();
    return this->active_recall(op_idx, label_idx);
}

Sequence ContextList::active_recall(const ulong op_idx, const ulong label_idx) {
    // return data[index].recall(op_idx, label_idx)->Compile(*this);
    // return data[index].recall(op_idx, label_idx)->Compile(this->data[index]);
    // return data[index].recall(op_idx, label_idx)->Compile(this->data[index], label_idx);
    unsigned int the_recall_type = data[index].recall_type(op_idx, label_idx);
    // std::cout << "active_recall: the_recall_type: " << the_recall_type << std::endl;
    Sequence result = data[index].recall(op_idx, label_idx)->Compile(*this, label_idx);
    if (the_recall_type == RULEMEMOIZE ) {
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(result);
        data[index].learn(op_idx, label_idx, bSeq);
    }
    return result;

    // return data[index].recall(op_idx, label_idx)->Compile(*this, label_idx);
    // return data[index].recall(op_idx, label_idx);
    //return Sequence("fish");

    if (data[index].recall_type(op_idx, label_idx) == RULESTORED) {
        return data[index].recall(op_idx, label_idx)->Compile(*this, label_idx);
    } else if (data[index].recall_type(op_idx, label_idx) == RULEMEMOIZE) {
        Sequence result = data[index].recall(op_idx, label_idx)->Compile(*this, label_idx);
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(result);
        data[index].learn(op_idx, label_idx, bSeq);
        return result;
    } else {
        return data[index].recall(op_idx, label_idx)->to_seq();
    }
}


void ContextList::print_multiverse() const {
    for (const auto &context: data) {
        context.print_universe();
        std::cout << std::endl;
    }
}

void ContextList::print_universe() const {
    data[index].print_universe();
}
