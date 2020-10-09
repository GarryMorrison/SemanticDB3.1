//
// Created by Admin on 16/09/2020.
//

#include <iostream>
#include "OperatorWithSequence.h"
#include "Ket.h"
#include "Superposition.h"
#include "Sequence.h"
#include "SelfKet.h"

void OperatorWithSequence::append(const unsigned int the_sign, const OperatorWithSequence &opWithSeq) {
    if (opWithSeq.size() == 0) { return; }
    sign_vec.push_back(the_sign);
    op_vec.push_back(opWithSeq.op_vec[0]);  // handle longer opWithSeq later!!!
    seq_vec.push_back(opWithSeq.seq_vec[0]);
}

const size_t OperatorWithSequence::size() const {
    return sign_vec.size();
}

const std::string OperatorWithSequence::to_string() const {
    if (sign_vec.empty()) { return ""; }

    std::string s;
    auto sign_vec_iter = sign_vec.cbegin();
    auto op_vec_iter = op_vec.cbegin();
    auto seq_vec_iter = seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_vec_iter != op_vec.end() and seq_vec_iter != seq_vec.end();
        ++sign_vec_iter, ++op_vec_iter, ++seq_vec_iter) {
        switch(*sign_vec_iter) {
            case SPLUS: s += "+ "; break;
            case SMINUS: s += "- "; break;
            case SSEQ: s += ". "; break;
            case SMERGE: s += "_ "; break;
            case SMERGE2: s += "__ "; break;
        }
        std::string pre = " ( ";
        std::string post = " ) ";
        if ((*seq_vec_iter)->is_ket()) { pre = ""; post = " "; }
        // s += (*op_vec_iter)->to_string() + " ( " + (*seq_vec_iter)->to_string() + " ) ";
        s += (*op_vec_iter)->to_string() + pre + (*seq_vec_iter)->to_string() + post;
    }
    return s;
}

// Maybe implement something more interesting later.
Ket OperatorWithSequence::to_ket() const {
    Ket tmp;
    return tmp;
}

Superposition OperatorWithSequence::to_sp() const {
    Superposition tmp;
    return tmp;
}

Sequence OperatorWithSequence::to_seq() const {
    Sequence tmp;
    return tmp;
}

Sequence OperatorWithSequence::Compile(NewContext& context) const {
    Sequence result;
    if (sign_vec.empty()) { return result; }

    auto sign_vec_iter = sign_vec.cbegin();
    auto op_vec_iter = op_vec.cbegin();
    auto seq_vec_iter = seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_vec_iter != op_vec.end() and seq_vec_iter != seq_vec.end();
           ++sign_vec_iter, ++op_vec_iter, ++seq_vec_iter) {
        // Sequence tmp_result;
        // tmp_result = op_vec.at(k)->Compile(context, seq_vec.at(k)->to_seq());
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, (*seq_vec_iter)->Compile(context));
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq);
        // std::cout << "op_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "tmp_result: " << tmp_result.to_string() << std::endl;
        if ((*sign_vec_iter) == SMINUS) {
            tmp_result.multiply(-1);
        }
        switch((*sign_vec_iter)) {
            case SPLUS: { result.add(tmp_result); break; }
            case SMINUS: { result.add(tmp_result); break; }
            case SSEQ: { result.append(tmp_result); break; }
            case SMERGE: { result.merge(tmp_result); break; }
            case SMERGE2: { result.merge(tmp_result, " "); break; }
        }
    }
    return result;
}

Sequence OperatorWithSequence::Compile(NewContext& context, const ulong label_idx) const {
    Sequence result;
    if (sign_vec.empty()) { return result; }

    auto sign_vec_iter = sign_vec.cbegin();
    auto op_vec_iter = op_vec.cbegin();
    auto seq_vec_iter = seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_vec_iter != op_vec.end() and seq_vec_iter != seq_vec.end();
           ++sign_vec_iter, ++op_vec_iter, ++seq_vec_iter) {
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, (*seq_vec_iter)->Compile(context));
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context, label_idx);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq);  // later make use of label_idx too!
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq, label_idx);
        // std::cout << "\ntmp_seq: " << tmp_seq.to_string() << std::endl;
        // std::cout << "tmp_result: " << tmp_result.to_string() << std::endl;
        if ((*sign_vec_iter) == SMINUS) {
            tmp_result.multiply(-1);
        }
        switch((*sign_vec_iter)) {
            case SPLUS: { result.add(tmp_result); break; }
            case SMINUS: { result.add(tmp_result); break; }
            case SSEQ: { result.append(tmp_result); break; }
            case SMERGE: { result.merge(tmp_result); break; }
            case SMERGE2: { result.merge(tmp_result, " "); break; }
        }
    }
    return result;
}

Sequence OperatorWithSequence::Compile(ContextList& context) const {
    Sequence result;
    if (sign_vec.empty()) { return result; }

    auto sign_vec_iter = sign_vec.cbegin();
    auto op_vec_iter = op_vec.cbegin();
    auto seq_vec_iter = seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_vec_iter != op_vec.end() and seq_vec_iter != seq_vec.end();
           ++sign_vec_iter, ++op_vec_iter, ++seq_vec_iter) {
        // Sequence tmp_result;
        // tmp_result = op_vec.at(k)->Compile(context, seq_vec.at(k)->to_seq());
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, (*seq_vec_iter)->Compile(context));
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq);
        // std::cout << "\nop_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "sign_vec_iter: " << (*sign_vec_iter) << std::endl;
        // std::cout << "tmp_result: " << tmp_result.to_string() << std::endl;
        if ((*sign_vec_iter) == SMINUS) {
            tmp_result.multiply(-1);
        }
        switch((*sign_vec_iter)) {
            case SPLUS: { result.add(tmp_result); break; }
            case SMINUS: { result.add(tmp_result); break; }
            case SSEQ: { result.append(tmp_result); break; }
            case SMERGE: { result.merge(tmp_result); break; }
            case SMERGE2: { result.merge(tmp_result, " "); break; }
        }
    }
    return result;
}

// finish label_idx component!
Sequence OperatorWithSequence::Compile(ContextList& context, const ulong label_idx) const {
    Sequence result;
    if (sign_vec.empty()) { return result; }

    auto sign_vec_iter = sign_vec.cbegin();
    auto op_vec_iter = op_vec.cbegin();
    auto seq_vec_iter = seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_vec_iter != op_vec.end() and seq_vec_iter != seq_vec.end();
           ++sign_vec_iter, ++op_vec_iter, ++seq_vec_iter) {
        // Sequence tmp_result;
        // tmp_result = op_vec.at(k)->Compile(context, seq_vec.at(k)->to_seq());
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, (*seq_vec_iter)->Compile(context));
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context, label_idx);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq);
        // Sequence tmp_result;
        // std::cout << "\nop_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "tmp_seq: " << tmp_seq.to_string() << std::endl;
        // std::cout << "tmp_result: " << tmp_result.to_string() << std::endl;
        if ((*sign_vec_iter) == SMINUS) {
            tmp_result.multiply(-1);
        }
        switch((*sign_vec_iter)) {
            case SPLUS: { result.add(tmp_result); break; }
            case SMINUS: { result.add(tmp_result); break; }
            case SSEQ: { result.append(tmp_result); break; }
            case SMERGE: { result.merge(tmp_result); break; }
            case SMERGE2: { result.merge(tmp_result, " "); break; }
        }
    }
    // return Sequence();  // This causes a bug somewhere. Exit code 139 == seg fault.
    return result;
}

// Implement later!
Sequence OperatorWithSequence::Compile(ContextList& context, const std::vector<Sequence>& args) const {
    return this->to_seq();
}