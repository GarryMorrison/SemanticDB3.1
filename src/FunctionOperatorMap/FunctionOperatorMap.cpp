//
// Created by Garry Morrison on 13/09/2020.
//

#include <iostream>
#include "../KetMap/KetMap.h"
#include "FunctionOperatorMap.h"
#include "../OperatorLibrary/SigmoidLibrary.h"
#include "../OperatorLibrary/OperatorLibrary.h"
#include "../OperatorLibrary/FunctionOperatorLibrary.h"


KetMap ket_map;
FunctionOperatorMap fn_map;

FunctionOperatorMap::FunctionOperatorMap() {
    ulong idx;

    idx = ket_map.get_idx("clean");
    fn_map.sigmoids.emplace(idx, &clean);


    idx = ket_map.get_idx("shuffle");
    fn_map.built_in.emplace(idx, &Sequence::shuffle);

    idx = ket_map.get_idx("sreverse");
    fn_map.built_in.emplace(idx, &Sequence::sreverse);

    idx = ket_map.get_idx("sdrop");
    fn_map.built_in.emplace(idx, &Sequence::sdrop);

    idx = ket_map.get_idx("threshold-filter");
    fn_map.compound_sigmoids.emplace(idx, &op_threshold_filter);

    idx = ket_map.get_idx("not-threshold-filter");
    fn_map.compound_sigmoids.emplace(idx, &op_not_threshold_filter);

    idx = ket_map.get_idx("sigmoid-in-range");
    fn_map.compound_sigmoids.emplace(idx, &op_sigmoid_in_range);

    idx = ket_map.get_idx("split");
    fn_map.ket_fn.emplace(idx, &op_split_ket);
    fn_map.compound_ket_fn.emplace(idx, &op_split);

    idx = ket_map.get_idx("ssplit");
    fn_map.ket_fn.emplace(idx, &op_ssplit_ket);
    fn_map.compound_ket_fn.emplace(idx, &op_ssplit);

    idx = ket_map.get_idx("bar-chart");
    fn_map.compound_sp_fn.emplace(idx, &op_bar_chart);

    idx = ket_map.get_idx("print");
    fn_map.ket_fn.emplace(idx, &print_ket);

    idx = ket_map.get_idx("extract-head");
    fn_map.ket_fn.emplace(idx, &extract_head);

    idx = ket_map.get_idx("extract-tail");
    fn_map.ket_fn.emplace(idx, &extract_tail);

    idx = ket_map.get_idx("extract-category");
    fn_map.ket_fn.emplace(idx, &extract_category);

    idx = ket_map.get_idx("extract-value");
    fn_map.ket_fn.emplace(idx, &extract_value);

    idx = ket_map.get_idx("push-float");
    fn_map.ket_fn.emplace(idx, &push_float);

    idx = ket_map.get_idx("pop-float");
    fn_map.ket_fn.emplace(idx, &pop_float);

    idx = ket_map.get_idx("rank");
    fn_map.sp_fn.emplace(idx, &rank);

    idx = ket_map.get_idx("sp2seq");
    fn_map.sp_fn.emplace(idx, &sp2seq);

    idx = ket_map.get_idx("seq2sp");
    fn_map.seq_fn.emplace(idx, &seq2sp);

    /*
    // double find_min_coeff() const;
    // double find_max_coeff() const;
    Sequence drop_below(const double t) const;
    Sequence drop_above(const double t) const;
    Sequence pick(const ulong n) const;
    */

    idx = ket_map.get_idx("do-you-know");
    fn_map.built_in.emplace(idx, &Sequence::do_you_know);

    idx = ket_map.get_idx("how-many");
    fn_map.built_in.emplace(idx, &Sequence::how_many);

    idx = ket_map.get_idx("measure-currency");
    fn_map.built_in.emplace(idx, &Sequence::measure_currency);

    idx = ket_map.get_idx("rescale");
    fn_map.built_in.emplace(idx, &Sequence::op_rescale0);
    fn_map.compound_built_in.emplace(idx, &Sequence::op_rescale1);

    idx = ket_map.get_idx("normalize");
    fn_map.built_in.emplace(idx, &Sequence::op_normalize0);
    fn_map.compound_built_in.emplace(idx, &Sequence::op_normalize1);

    idx = ket_map.get_idx("reverse");
    fn_map.built_in.emplace(idx, &Sequence::reverse);

    idx = ket_map.get_idx("drop");
    fn_map.built_in.emplace(idx, &Sequence::drop);

    idx = ket_map.get_idx("pick-elt");
    fn_map.built_in.emplace(idx, &Sequence::pick_elt);

    idx = ket_map.get_idx("weighted-pick-elt");
    fn_map.built_in.emplace(idx, &Sequence::weighted_pick_elt);

    idx = ket_map.get_idx("drop-below");
    fn_map.compound_built_in.emplace(idx, &Sequence::op_drop_below1);

    idx = ket_map.get_idx("drop-above");
    fn_map.compound_built_in.emplace(idx, &Sequence::op_drop_above1);

    idx = ket_map.get_idx("pick");
    fn_map.compound_built_in.emplace(idx, &Sequence::op_pick1);

    idx = ket_map.get_idx("ket-sort");
    fn_map.built_in.emplace(idx, &Sequence::ket_sort);

    idx = ket_map.get_idx("coeff-sort");
    fn_map.built_in.emplace(idx, &Sequence::coeff_sort);

    idx = ket_map.get_idx("natural-sort");
    fn_map.built_in.emplace(idx, &Sequence::natural_sort);

    idx = ket_map.get_idx("srange");
    fn_map.whitelist_2.emplace(idx, &op_srange2);
    fn_map.whitelist_3.emplace(idx, &op_srange3);

    idx = ket_map.get_idx("arithmetic");
    fn_map.whitelist_3.emplace(idx, &op_arithmetic3);

    idx = ket_map.get_idx("simm");
    fn_map.whitelist_2.emplace(idx, &op_simm2);

    idx = ket_map.get_idx("intersection");
    fn_map.whitelist_2.emplace(idx, &op_intersection2);

    idx = ket_map.get_idx("select");
    fn_map.compound_built_in.emplace(idx, &Sequence::op_select2);

    idx = ket_map.get_idx("sselect");
    fn_map.compound_built_in.emplace(idx, &Sequence::op_sselect2);

    idx = ket_map.get_idx("rel-kets");
    fn_map.compound_context_seq_fn.emplace(idx, &op_relevant_kets);

    idx = ket_map.get_idx("similar-input");
    fn_map.compound_context_seq_fn.emplace(idx, &op_similar_input);

    idx = ket_map.get_idx("minus");
    fn_map.compound_ket_fn.emplace(idx, &op_minus);

    idx = ket_map.get_idx("plus");
    fn_map.compound_ket_fn.emplace(idx, &op_plus);

    idx = ket_map.get_idx("times-by");
    fn_map.compound_ket_fn.emplace(idx, &op_times_by);

    idx = ket_map.get_idx("divide-by");
    fn_map.compound_ket_fn.emplace(idx, &op_divide_by);

    idx = ket_map.get_idx("mod");
    fn_map.compound_ket_fn.emplace(idx, &op_modulus);

    idx = ket_map.get_idx("to-upper");
    fn_map.ket_fn.emplace(idx, &toupperket);
    fn_map.compound_ket_fn.emplace(idx, &op_toupper);

    idx = ket_map.get_idx("to-lower");
    fn_map.ket_fn.emplace(idx, &tolowerket);
    fn_map.compound_ket_fn.emplace(idx, &op_tolower);

    idx = ket_map.get_idx("table");
    fn_map.compound_context_sp_fn.emplace(idx, &op_table);

    idx = ket_map.get_idx("transpose-table");
    fn_map.compound_context_sp_fn.emplace(idx, &op_transpose_table);

    idx = ket_map.get_idx("filter");
    fn_map.context_whitelist_2.emplace(idx, &op_filter);

    idx = ket_map.get_idx("apply");
    fn_map.context_whitelist_2.emplace(idx, &op_apply);

    idx = ket_map.get_idx("learn");
    fn_map.context_whitelist_3.emplace(idx, &op_learn);

    idx = ket_map.get_idx("add-learn");
    fn_map.context_whitelist_3.emplace(idx, &op_add_learn);

    idx = ket_map.get_idx("seq-learn");
    fn_map.context_whitelist_3.emplace(idx, &op_seq_learn);

}

template <class T>
void print_map(const std::string& s, T& our_map) {
    std::cout << s << std::endl;
    std::vector<std::string> tmp_sorted;
    for (const auto &it: our_map) {
        tmp_sorted.push_back(ket_map.get_str(it.first));
    }
    std::sort(tmp_sorted.begin(), tmp_sorted.end());
    for (const auto &str: tmp_sorted) {
        std::cout << "    " << str << std::endl;
    }
}

void FunctionOperatorMap::print() const { // Doesn't need to be optimized at all.
    print_map("built_in:", built_in);
    print_map("compound_built_in:", compound_built_in);
    print_map("sigmoid:", sigmoids);
    print_map("compound_sigmoid:", compound_sigmoids);
    print_map("ket_fn:", ket_fn);
    print_map("compound_ket_fn:", compound_ket_fn);
    print_map("sp_fn:", sp_fn);
    print_map("compound_sp_fn:", compound_sp_fn);
    print_map("seq_fn:", seq_fn);
    print_map("compound_context_sp_fn:", compound_context_sp_fn);
    print_map("compound_context_seq_fn:", compound_context_seq_fn);
    print_map("function_1:", whitelist_1);
    print_map("function_2:", whitelist_2);
    print_map("function_3:", whitelist_3);
    print_map("function_4:", whitelist_4);
    print_map("context_function_1:", context_whitelist_1);
    print_map("context_function_2:", context_whitelist_2);
    print_map("context_function_3:", context_whitelist_3);
    print_map("context_function_4:", context_whitelist_4);
}