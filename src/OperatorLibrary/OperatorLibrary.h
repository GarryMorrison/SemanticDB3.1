//
// Created by Garry Morrison on 21/09/2020.
//

#ifndef SDB3_1_OPERATORLIBRARY_H
#define SDB3_1_OPERATORLIBRARY_H

#include "../Sequence/Sequence.h"
#include "../Sequence/Ket.h"

Superposition split(const Ket k);
Superposition split(const Ket k, const std::string& delimiter);
Superposition op_split(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);  // should this return Sequence instead?
Sequence ssplit(const Ket k);
Sequence ssplit(const Ket k, const std::string& delimiter);
// Sequence ssplit(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Sequence op_ssplit(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Superposition op_split_ket(const Ket k);
Sequence op_ssplit_ket(const Ket k);

Ket extract_head(const Ket k);
Ket extract_tail(const Ket k);
Ket extract_category(const Ket k);
Ket extract_value(const Ket k);
Ket push_float(const Ket k);
Ket pop_float(const Ket k);
Superposition rank(const Superposition& sp);
Sequence sp2seq(const Superposition &sp);
Superposition seq2sp(const Sequence &seq);
Ket bar_chart(const Superposition &sp, const unsigned int width);
Ket op_bar_chart(const Superposition &sp, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket print_ket(const Ket k);
Superposition op_relevant_kets(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Superposition op_similar_input(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_minus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_plus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_times_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_divide_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_modulus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_toupper(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_tolower(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket toupperket(const Ket k);
Ket tolowerket(const Ket k);

#endif //SDB3_1_OPERATORLIBRARY_H
