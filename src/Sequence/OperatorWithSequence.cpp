//
// Created by Admin on 16/09/2020.
//

#include <iostream>
#include "OperatorWithSequence.h"
#include "Ket.h"
#include "Superposition.h"
#include "Sequence.h"
#include "SelfKet.h"
#include "../Operator/InfixOperator.h"

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
            case SPLUS: { if (sign_vec_iter == sign_vec.cbegin()) { s = ""; } else { s += " + "; } break; }  // s = " " or s = "" ?
            case SMINUS: s += " - "; break;
            case SSEQ: s += " . "; break;
            case SMERGE: s += " _ "; break;
            case SMERGE2: s += " __ "; break;

            /*  // Currently we don't need these! They are handled by InfixOperator.
            case OPEQUAL: s += " == "; break;
            case OPNOTEQUAL: s += " != "; break;
            case OPAND: s += " && "; break;
            case OPOR: s += " || "; break;
            case OPPLUS: s += " ++ "; break;
            case OPMINUS: s += " -- "; break;
            case OPMULT: s += " ** "; break;
            case OPDIV: s += " // "; break;
            case OPMOD: s += " %% "; break;
            case OPARITHPOWER: s += " ^^ "; break;
            case OPRANGE: s += " .. "; break;
            */
            default: s += " ?? ";
        }
        std::string pre = " ( ";
        std::string post = " ) ";
        if ((*seq_vec_iter)->is_ket()) { pre = ""; post = ""; }
        // s += (*op_vec_iter)->to_string() + " ( " + (*seq_vec_iter)->to_string() + " ) ";
        s += (*op_vec_iter)->to_string() + pre + (*seq_vec_iter)->to_string() + post;
    }
    return s;
}

const std::string OperatorWithSequence::to_string(const std::string &prefix) const {
    return prefix + this->to_string();  // Not sure this is correct. this->to_string() might be multi-line.
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

        // std::cout << "\nOperatorWithSequence::Compile:" << std::endl;  // Later, if in debug mode, print this out.
        // std::cout << "    op_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "    op_vec_iter type: " << (*op_vec_iter)->type() << std::endl;
        // std::cout << "    seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "    sign_vec_iter: " << (*sign_vec_iter) << std::endl;
        // std::cout << "    tmp_seq: " << tmp_seq.to_string() << std::endl;
        // std::cout << "    tmp_result: " << tmp_result.to_string() << std::endl;
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
Sequence OperatorWithSequence::Compile(ContextList& context, const Ket& label_ket) const {
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
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context, label_ket);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq, label_ket);
        // Sequence tmp_result;

        // std::cout << "OperatorWithSequence::Compile(context, label_ket):" << std::endl;
        // std::cout << "    label_ket: " << label_ket.to_string() << std::endl;
        // std::cout << "    op_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "    seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "    tmp_seq: " << tmp_seq.to_string() << std::endl;
        // std::cout << "    tmp_result: " << tmp_result.to_string() << std::endl;

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

Sequence OperatorWithSequence::Compile(ContextList &context, const Ket& label_ket, const Ket& multi_label_ket) const {
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
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context, label_ket, multi_label_ket);
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq, label_ket, multi_label_ket);
        // Sequence tmp_result;

        // std::cout << "OperatorWithSequence::Compile(context, label_ket, multi_label_ket):" << std::endl;
        // std::cout << "    label_ket: " << label_ket.to_string() << std::endl;
        // std::cout << "    multi_label_ket: " << multi_label_ket.to_string() << std::endl;
        // std::cout << "    op_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "    seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "    tmp_seq: " << tmp_seq.to_string() << std::endl;
        // std::cout << "    tmp_result: " << tmp_result.to_string() << std::endl;

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


Sequence OperatorWithSequence::Compile(ContextList& context, const Ket& label_ket, const std::vector<Sequence>& args) const {
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
        Sequence tmp_seq = (*seq_vec_iter)->Compile(context, label_ket, args);
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq);
        // Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq, label_idx);
        Sequence tmp_result = (*op_vec_iter)->Compile(context, tmp_seq, label_ket, args);
        // Sequence tmp_result;

        // std::cout << "OperatorWithSequence::Compile(context, label_ket, args):" << std::endl;
        // std::cout << "    label_ket: " << label_ket.to_string() << std::endl;
        // std::cout << "    op_vec_iter: " << (*op_vec_iter)->to_string() << std::endl;
        // std::cout << "    seq_vec_iter: " << (*seq_vec_iter)->to_string() << std::endl;
        // std::cout << "    tmp_seq: " << tmp_seq.to_string() << std::endl;
        // std::cout << "    tmp_result: " << tmp_result.to_string() << std::endl;

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
