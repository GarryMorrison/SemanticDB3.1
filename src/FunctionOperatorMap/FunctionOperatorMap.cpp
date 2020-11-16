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
}

void FunctionOperatorMap::print() const {
    std::cout << "built_in:" << std::endl;
    for (const auto &it : built_in) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "compound_built_in:" << std::endl;
    for (const auto &it : compound_built_in) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "sigmoid:" << std::endl;
    for (const auto &it : sigmoids) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "compound_sigmoid:" << std::endl;
    for (const auto &it : compound_sigmoids) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "ket_fn:" << std::endl;
    for (const auto &it : ket_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "compound_ket_fn:" << std::endl;
    for (const auto &it : compound_ket_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "sp_fn:" << std::endl;
    for (const auto &it : sp_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "compound_sp_fn:" << std::endl;
    for (const auto &it : compound_sp_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "seq_fn:" << std::endl;
    for (const auto &it : seq_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "compound_context_sp_fn:" << std::endl;
    for (const auto &it : compound_context_sp_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "compound_context_seq_fn:" << std::endl;
    for (const auto &it : compound_context_seq_fn) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "whitelist_1:" << std::endl;
    for (const auto &it : whitelist_1) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
    std::cout << "whitelist_2:" << std::endl;
    for (const auto &it : whitelist_2) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
    std::cout << "whitelist_3:" << std::endl;
    for (const auto &it : whitelist_3) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
    std::cout << "whitelist_4:" << std::endl;
    for (const auto &it : whitelist_4) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }

    std::cout << "context_whitelist_1:" << std::endl;
    for (const auto &it : context_whitelist_1) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
    std::cout << "context_whitelist_2:" << std::endl;
    for (const auto &it : context_whitelist_2) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
    std::cout << "context_whitelist_3:" << std::endl;
    for (const auto &it : context_whitelist_3) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
    std::cout << "context_whitelist_4:" << std::endl;
    for (const auto &it : context_whitelist_4) {
        std::cout << "    " << ket_map.get_str(it.first) << std::endl;
    }
}