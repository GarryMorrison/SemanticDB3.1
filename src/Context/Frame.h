//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_FRAME_H
#define SDB3_1_FRAME_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "../KetMap/KetMap.h"
#include "../Sequence/BaseSequence.h"
#include "../Sequence/Superposition.h"

#define RULEUNDEFINED 20
#define RULENORMAL 21
#define RULESTORED 22
#define RULEMEMOIZE 23
#define RULEADD 24
#define RULESEQ 25

class Frame {
private:
    std::vector<ulong> supported_operators;
    // std::unordered_map<ulong, BaseSequence*> rules;  // later comment this one out.
    std::unordered_map<ulong, std::shared_ptr<BaseSequence> > ptr_rules;
    std::unordered_map<ulong, unsigned int> rule_types;

public:
    Frame() {};
    std::vector<ulong> supported_ops() const { return supported_operators; };
    // void learn(const ulong op_idx, BaseSequence* bSeq);
    // void add_learn(const ulong op_idx, BaseSequence* bSeq);
    // void seq_learn(const ulong op_idx, BaseSequence* bSeq);
    // BaseSequence* recall(const ulong op_idx);

    void learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq);
    void add_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq);
    void seq_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq);
    void stored_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq);
    void memoize_learn(const ulong op_idx, std::shared_ptr<BaseSequence> bSeq);
    std::shared_ptr<BaseSequence> recall(const ulong op_idx);
    unsigned int recall_type(const ulong op_idx);
    void unlearn(const ulong op_idx);

    void print();

};


#endif //SDB3_1_FRAME_H
