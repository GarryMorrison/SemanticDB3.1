//
// Created by Garry Morrison on 13/09/2020.
//

#include "BracketOperator.h"

Sequence BracketOperator::Compile(NewContext& context, const Sequence& seq) const {  // deprecated. Remove later.
    Sequence seq2;
    for (const auto &op_seq : op_seq_vec) {
        Sequence compiled_seq = op_seq.Compile(context, seq);
        // std::cout << "op_seq: " << op_seq.to_string() << std::endl;
        // std::cout << "op_seq symbol type: " << op_seq.symbol_type() << std::endl;
        // std::cout << "compiled_seq: " << compiled_seq.to_string() << std::endl;
        switch (op_seq.symbol_type()) {
            case SPLUS: { seq2.add(compiled_seq); break; }
            case SMINUS: { seq2.add(compiled_seq); break; }
            case SSEQ: { seq2.append(compiled_seq); break; }
            case SMERGE: { seq2.merge(compiled_seq); break; }
            case SMERGE2: { seq2.merge(compiled_seq, " "); break; }
        }
    }
    // std::cout << "BracketOp::seq2: " << seq2.to_string() << std::endl;
    return seq2;
}

/*
Sequence BracketOperator::Compile(ContextList& context, const Sequence& seq) const {
    Sequence seq2;
    for (const auto &op_seq : op_seq_vec) {
        Sequence compiled_seq = op_seq.Compile(context, seq);
        // std::cout << "op_seq: " << op_seq.to_string() << std::endl;
        // std::cout << "op_seq symbol type: " << op_seq.symbol_type() << std::endl;
        // std::cout << "compiled_seq: " << compiled_seq.to_string() << std::endl;
        switch (op_seq.symbol_type()) {
            case SPLUS: { seq2.add(compiled_seq); break; }
            case SMINUS: { seq2.add(compiled_seq); break; }
            case SSEQ: { seq2.append(compiled_seq); break; }
            case SMERGE: { seq2.merge(compiled_seq); break; }
            case SMERGE2: { seq2.merge(compiled_seq, " "); break; }
        }
    }
    // std::cout << "BracketOp::seq2: " << seq2.to_string() << std::endl;
    return seq2;
}

Sequence BracketOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    Sequence seq2;
    for (const auto &op_seq : op_seq_vec) {
        Sequence compiled_seq = op_seq.Compile(context, seq, label_idx);
        switch (op_seq.symbol_type()) {
            case SPLUS: { seq2.add(compiled_seq); break; }
            case SMINUS: { seq2.add(compiled_seq); break; }
            case SSEQ: { seq2.append(compiled_seq); break; }
            case SMERGE: { seq2.merge(compiled_seq); break; }
            case SMERGE2: { seq2.merge(compiled_seq, " "); break; }
        }
    }
    return seq2;
}
*/

Sequence BracketOperator::Compile(ContextList &context, const Sequence &seq) const {
    Sequence seq2;
    auto sign_vec_iter = sign_vec.cbegin();
    auto op_seq_vec_iter = op_seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_seq_vec_iter != op_seq_vec.end();
           ++sign_vec_iter, ++op_seq_vec_iter) {
        Sequence compiled_seq = op_seq_vec_iter->Compile(context, seq);
        switch (*sign_vec_iter) {
            case SPLUS:
                seq2.add(compiled_seq);  // use move semantics??
                break;
            case SMINUS:
                compiled_seq.multiply(-1);
                seq2.add(compiled_seq);
                break;
            case SSEQ:
                seq2.append(compiled_seq);
                break;
            case SMERGE:
                seq2.merge(compiled_seq);
                break;
            case SMERGE2:
                seq2.merge(compiled_seq, " ");
                break;
        }
    }
    return seq2;
}

Sequence BracketOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx) const {
    Sequence seq2;
    auto sign_vec_iter = sign_vec.cbegin();
    auto op_seq_vec_iter = op_seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_seq_vec_iter != op_seq_vec.end();
           ++sign_vec_iter, ++op_seq_vec_iter) {
        Sequence compiled_seq = op_seq_vec_iter->Compile(context, seq, label_idx);
        switch (*sign_vec_iter) {
            case SPLUS:
                seq2.add(compiled_seq);  // use move semantics??
                break;
            case SMINUS:
                compiled_seq.multiply(-1);
                seq2.add(compiled_seq);
                break;
            case SSEQ:
                seq2.append(compiled_seq);
                break;
            case SMERGE:
                seq2.merge(compiled_seq);
                break;
            case SMERGE2:
                seq2.merge(compiled_seq, " ");
                break;
        }
    }
    return seq2;
}

Sequence BracketOperator::Compile(ContextList &context, const Sequence &seq, const ulong label_idx, const ulong multi_label_idx) const {
    Sequence seq2;
    auto sign_vec_iter = sign_vec.cbegin();
    auto op_seq_vec_iter = op_seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_seq_vec_iter != op_seq_vec.end();
           ++sign_vec_iter, ++op_seq_vec_iter) {
        Sequence compiled_seq = op_seq_vec_iter->Compile(context, seq, label_idx, multi_label_idx);
        switch (*sign_vec_iter) {
            case SPLUS:
                seq2.add(compiled_seq);  // use move semantics??
                break;
            case SMINUS:
                compiled_seq.multiply(-1);
                seq2.add(compiled_seq);
                break;
            case SSEQ:
                seq2.append(compiled_seq);
                break;
            case SMERGE:
                seq2.merge(compiled_seq);
                break;
            case SMERGE2:
                seq2.merge(compiled_seq, " ");
                break;
        }
    }
    return seq2;
}

const std::string BracketOperator::to_string() const {
    std::string s = "(";
    auto sign_vec_iter = sign_vec.cbegin();
    auto op_seq_vec_iter = op_seq_vec.cbegin();
    for (; sign_vec_iter != sign_vec.end() and op_seq_vec_iter != op_seq_vec.end();
           ++sign_vec_iter, ++op_seq_vec_iter) {
        switch (*sign_vec_iter) {
            case SPLUS:
                s += " + ";
                break;
            case SMINUS:
                s += " - ";
                break;
            case SSEQ:
                s += " . ";
                break;
            case SMERGE:
                s += " _ ";
                break;
            case SMERGE2:
                s += " __ ";
                break;
        }
        s += op_seq_vec_iter->to_string();
    }
    return s + " )";
}
