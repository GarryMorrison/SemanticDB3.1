//
// Created by Garry Morrison on 6/06/2021.
//

#ifndef SDB3_1_BOUNDFUNCTION_H
#define SDB3_1_BOUNDFUNCTION_H

#include <vector>
#include "../Sequence/BaseSequence.h"

class BoundFunction {
private:
    std::vector<ulong> op_ket_idx_vec;  // make it a reference?
    std::shared_ptr<BaseSequence> bSeq;

public:
    BoundFunction();
    // BoundFunction(const std::vector<ulong> &op_ket_idx_vec2, const std::shared_ptr<BaseSequence> bSeq2)
    // : op_ket_idx_vec(op_ket_idx_vec2), bSeq(bSeq2) {};

    void learn(const std::vector<ulong> &op_ket_idx_vec2, const std::shared_ptr<BaseSequence> bSeq2);
    unsigned int get_params_size();
    std::vector<ulong> get_params() const;
    std::shared_ptr<BaseSequence> get_body() const;
};


#endif //SDB3_1_BOUNDFUNCTION_H
