//
// Created by Garry Morrison on 6/06/2021.
//

#include "BoundFunction.h"

BoundFunction::BoundFunction() {

}

void BoundFunction::learn(const std::vector<ulong> &op_ket_idx_vec2, const std::shared_ptr<BaseSequence> bSeq2) {
    op_ket_idx_vec = op_ket_idx_vec2;
    bSeq = bSeq2;
}

unsigned int BoundFunction::get_params_size() {
    return op_ket_idx_vec.size();  // divide by 2 here?
}

std::vector<ulong> BoundFunction::get_params() const {
    return op_ket_idx_vec;
}

std::shared_ptr<BaseSequence> BoundFunction::get_body() const {
    return bSeq;
}

