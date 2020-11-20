//
// Created by Garry Morrison on 21/09/2020.
//

#ifndef SDB3_1_FUNCTIONOPERATORLIBRARY_H
#define SDB3_1_FUNCTIONOPERATORLIBRARY_H

#include "../Sequence/Sequence.h"

Sequence op_srange2(const Sequence& input_seq, const Sequence& start, const Sequence& stop);
Sequence op_srange3(const Sequence& input_seq, const Sequence& start, const Sequence& stop, const Sequence& step);
Sequence op_arithmetic3(const Sequence &input_seq, const Sequence &one, const Sequence &symbol_ket, const Sequence &two);
double simm(const Superposition &sp1, const Superposition &sp2);
double scaled_simm(const Superposition &sp1, const Superposition &sp2);
double simm(const Sequence &seq1, const Sequence &seq2);
Sequence op_simm2(const Sequence &input_seq, const Sequence &seq1, const Sequence &seq2);
Superposition intersection(const Superposition &sp1, const Superposition &sp2);
Sequence op_intersection2(const Sequence &input_seq, const Sequence &one, const Sequence &two);
Sequence op_filter(ContextList &context, const Sequence &input_seq, const Sequence &one, const Sequence &two);

#endif //SDB3_1_FUNCTIONOPERATORLIBRARY_H
