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
Ket ket_length(const Ket k);
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
Superposition op_strict_similar_input(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Superposition op_equal_input(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_minus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_plus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_times_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_divide_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_int_divide_by(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_round(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_modulus(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_toupper(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_tolower(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket toupperket(const Ket k); // rename these two ...
Ket tolowerket(const Ket k);
Ket op_table(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_transpose_table(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_smerge(const Sequence &seq);
Ket op_smerge1(const Sequence &seq, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);

// Not sure this is the best parameter structure for this function, but will do for now.
Ket op_learn_grid(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_display_grid(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_less_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_less_equal_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_equal(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_greater_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_greater_equal_than(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_in_range(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_is_prime(const Ket k);
Superposition op_prime_factors(const Ket k);
// Ket op_such_that(const Ket k, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters); // We don't have the backend to use this function prototype.
Sequence op_such_that(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Sequence op_smap(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Superposition op_Gaussian(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_find_inverse(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_remove_suffix(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_has_suffix(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_remove_prefix(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_has_prefix(const Ket k, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_to_comma_number(const Ket k);
Superposition digit2sp(const Superposition &sp, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_unlearn(const Superposition &sp, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Ket op_scompress(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Superposition starts_with(const Ket k, ContextList &context);
Sequence op_inherit(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Sequence op_inherit_path(const Sequence &seq, ContextList &context, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Sequence op_sprint(const Sequence &seq);
Sequence op_sprint1(const Sequence &seq, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
Sequence op_compile(const Sequence& seq, ContextList &context);

#endif //SDB3_1_OPERATORLIBRARY_H
