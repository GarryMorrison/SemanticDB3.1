//
// Created by Garry Morrison on 21/09/2020.
//

#ifndef SDB3_1_FUNCTIONOPERATORLIBRARY_H
#define SDB3_1_FUNCTIONOPERATORLIBRARY_H

#include "../Sequence/Sequence.h"

Sequence op_srange2(const Sequence& input_seq, const Sequence& start, const Sequence& stop);
Sequence op_srange3(const Sequence& input_seq, const Sequence& start, const Sequence& stop, const Sequence& step);
Superposition op_range2(const Sequence& input_seq, const Sequence& start, const Sequence& stop);
Superposition op_range3(const Sequence& input_seq, const Sequence& start, const Sequence& stop, const Sequence& step);
Sequence op_arithmetic3(const Sequence &input_seq, const Sequence &one, const Sequence &symbol_ket, const Sequence &two);
double simm(const Superposition &sp1, const Superposition &sp2);
double scaled_simm(const Superposition &sp1, const Superposition &sp2);
double simm(const Sequence &seq1, const Sequence &seq2);
double strict_simm(const Sequence &seq1, const Sequence &seq2);
Sequence op_simm2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2);
Sequence op_strict_simm2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2);
Superposition sp_intersection(const Superposition &sp1, const Superposition &sp2);
Sequence op_intersection2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Superposition sp_union(const Superposition &sp1, const Superposition &sp2);
Sequence op_union2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Superposition sp_sum(const Superposition &sp1, const Superposition &sp2);
Sequence op_sum2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_filter(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_not_filter(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_apply(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_learn(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_add_learn(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_seq_learn(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_zip(ContextList &context, const Sequence &input_seq, const Sequence &one);
Sequence op_if(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_value_if(const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_is_mbr(const Sequence &input_seq, const Sequence &one);
Sequence op_mbr(const Sequence &input_seq, const Sequence &one);
Sequence op_is_subset(const Sequence &input_seq, const Sequence &one);
Sequence op_and1(const Sequence &input_seq, const Sequence &one);
Sequence op_or1(const Sequence &input_seq, const Sequence &one);
Sequence op_xor1(const Sequence &input_seq, const Sequence &one);
Sequence op_and2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_or2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_xor2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_smap3(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_is_equal2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2);
Sequence op_find_path_between(ContextList &context, const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2);
Sequence op_find_operators_between(ContextList &context, const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2);
Sequence op_sread(const Sequence &input_seq, const Sequence &one);
Sequence op_not_sread(const Sequence &input_seq, const Sequence &one);
Sequence op_read(const Sequence &input_seq, const Sequence &one);
Sequence op_not_read(const Sequence &input_seq, const Sequence &one);
Sequence op_string_replace(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_for2(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_for3(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_for4(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three, const Sequence &four);
Sequence op_sfor2(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_sfor3(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three);
Sequence op_sfor4(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two, const Sequence &three, const Sequence &four);


#endif //SDB3_1_FUNCTIONOPERATORLIBRARY_H
