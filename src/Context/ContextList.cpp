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

void ContextList::set(const ulong idx) {
    if (idx < 0 || idx >= max_index) {
        return;
    }
    index = idx;
}

void ContextList::reset() {
    index = 0;
    max_index = 1;
    NewContext tmp(name);
    data.clear();
    data.push_back(tmp);
}

void ContextList::show_context_list() {
    std::cout << "\nContextList:\n";
    for (ulong k = 0; k < max_index; k++ ) {
        std::string star = "   ";
        if (k == index) {
            star = " * ";
        }
        std::cout << "    " << k << ")" << star << data[k].get_name() << std::endl;
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
        Ket label_ket(label_idx);
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].learn(op_idx, label_idx, bSeq2);
    } else {
        data[index].learn(op_idx, label_idx, bSeq);
    }
}

void ContextList::learn(const ulong op_idx, const Ket& label_ket, std::shared_ptr<BaseSequence> bSeq) {
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].learn(op_idx, label_ket.label_idx(), bSeq2);
    } else {
        data[index].learn(op_idx, label_ket.label_idx(), bSeq);
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
        Ket label_ket(label_idx);
        Sequence result = bSeq->Compile(*this, label_ket);
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
    // data[index].add_learn(op_idx, label_idx, bSeq);
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Ket label_ket(label_idx);
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].add_learn(op_idx, label_idx, bSeq2);
    } else {
        data[index].add_learn(op_idx, label_idx, bSeq);
    }
}

void ContextList::add_learn(const ulong op_idx, const Ket &label_ket, std::shared_ptr<BaseSequence> bSeq) {
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].add_learn(op_idx, label_ket.label_idx(), bSeq2);
    } else {
        data[index].add_learn(op_idx, label_ket.label_idx(), bSeq);
    }
}

void ContextList::add_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    // data[index].add_learn(op, label, bSeq);
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Ket label_ket(label);
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].add_learn(op, label, bSeq2);
    } else {
        data[index].add_learn(op, label, bSeq);
    }
}

void ContextList::add_learn(const std::string& op, const std::string& label, const std::string& srule) {
    data[index].add_learn(op, label, srule);
}


void ContextList::seq_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq) {
    // data[index].seq_learn(op_idx, label_idx, bSeq);
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Ket label_ket(label_idx);
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].seq_learn(op_idx, label_idx, bSeq2);
    } else {
        data[index].seq_learn(op_idx, label_idx, bSeq);
    }
}

void ContextList::seq_learn(const ulong op_idx, const Ket &label_ket, std::shared_ptr<BaseSequence> bSeq) {
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].seq_learn(op_idx, label_ket.label_idx(), bSeq2);
    } else {
        data[index].seq_learn(op_idx, label_ket.label_idx(), bSeq);
    }
}

void ContextList::seq_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq) {
    // data[index].seq_learn(op, label, bSeq);
    if (bSeq->type() == OPERATORWITHSEQUENCE || bSeq->type() == SELFKET) {  // Probably need to add more types here.
        Ket label_ket(label);
        Sequence result = bSeq->Compile(*this, label_ket);
        std::shared_ptr<BaseSequence> bSeq2 = std::make_shared<Sequence>(result);
        data[index].seq_learn(op, label, bSeq2);
    } else {
        data[index].seq_learn(op, label, bSeq);
    }
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

void ContextList::unlearn(const ulong op_idx, const ulong label_idx) {
    data[index].unlearn(op_idx, label_idx);
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

std::vector<ulong> ContextList::supported_ops(const ulong label_idx) {
    return data[index].supported_ops(label_idx);
}

void ContextList::find_inverse(const ulong op_idx) {
    data[index].find_inverse(op_idx);
}

void ContextList::find_multi_inverse(const ulong op_idx) {
    for (unsigned int idx = 0; idx < max_index; idx++) {
        data[idx].find_inverse(op_idx);
    }
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
    // unsigned int the_recall_type = data[index].recall_type(op_idx, label_idx);
    unsigned int the_recall_type = data[index].recall_descent_type(op_idx, label_idx);  // Probably slows things down quite a bit over .recall_type()!

    // std::cout << "ContextList::active_recall(op_idx, label_idx):" << std::endl;
    // std::cout << "    op_idx: " << ket_map.get_str(op_idx) << std::endl;
    // std::cout << "    label_idx: " << ket_map.get_str(label_idx) << std::endl;
    // std::cout << "    recall_descent_type: " << the_recall_type << std::endl;

    Ket label_ket(label_idx);
    Sequence result = data[index].recall(op_idx, label_idx)->Compile(*this, label_ket);
    if (the_recall_type == RULEMEMOIZE) {
    // if (the_recall_type == RULEMEMOIZE || the_recall_type == RULEUNDEFINED) {
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

Sequence ContextList::active_recall(const ulong op_idx, const Ket& label_ket) {
    unsigned int the_recall_type = data[index].recall_descent_type(op_idx, label_ket.label_idx());  // Probably slows things down quite a bit over .recall_type()!

    // std::cout << "ContextList::active_recall(op_idx, label_ket):" << std::endl;
    // std::cout << "    op_idx: " << ket_map.get_str(op_idx) << std::endl;
    // std::cout << "    label_ket: " << label_ket.to_string() << std::endl;
    // std::cout << "    recall_descent_type: " << the_recall_type << std::endl;

    Sequence result = data[index].recall(op_idx, label_ket.label_idx())->Compile(*this, label_ket);
    if (the_recall_type == RULEMEMOIZE) {
        // if (the_recall_type == RULEMEMOIZE || the_recall_type == RULEUNDEFINED) {
        std::shared_ptr<BaseSequence> bSeq = std::make_shared<Sequence>(result);
        data[index].learn(op_idx, label_ket.label_idx(), bSeq);
    }
    return result;
}


void ContextList::fn_learn(const ulong op_idx, const int param_size, std::shared_ptr<BaseSequence> bSeq) {
    data[index].fn_learn(op_idx, param_size, bSeq);
}

std::shared_ptr<BaseSequence> ContextList::fn_recall(const ulong op_idx, const int param_size) {
    return data[index].fn_recall(op_idx, param_size);
}

unsigned int ContextList::fn_recall_type(const ulong op_idx, const int param_size) {
    return data[index].fn_recall_type(op_idx, param_size);
}

void ContextList::bound_fn_learn(ulong op_idx, std::vector<ulong> &op_ket_idx_vec, std::shared_ptr<BaseSequence> bSeq) {
    return data[index].bound_fn_learn(op_idx, op_ket_idx_vec, bSeq);
}

std::vector<ulong> ContextList::bound_fn_params_recall(const ulong op_idx, const int param_size) {
    return data[index].bound_fn_params_recall(op_idx, param_size);
}

std::shared_ptr<BaseSequence> ContextList::bound_fn_body_recall(const ulong op_idx, const int param_size) {
    return data[index].bound_fn_body_recall(op_idx, param_size);
}

unsigned int ContextList::bound_fn_recall_type(const ulong op_idx, const int param_size) {
    return data[index].bound_fn_recall_type(op_idx, param_size);
}

void ContextList::print_multiverse(bool clean, std::ostream& out) const {
    for (const auto &context: data) {
        context.print_universe(clean, out);  // Maybe loop over index instead?
        std::cout << std::endl;
    }
}

void ContextList::print_universe(bool clean, std::ostream& out) const {
    data[index].print_universe(clean, out);
}
