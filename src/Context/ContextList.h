//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_CONTEXTLIST_H
#define SDB3_1_CONTEXTLIST_H

#include <string>
#include <vector>
#include "NewContext.h"


class ContextList {
private:
    std::string name;
    ulong index;
    ulong max_index;
    std::vector<NewContext> data;

public:
    ContextList(const std::string& s);
    void set(const std::string& s);

/*
    void learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq);
    void learn(const std::string& op, const std::string& label, BaseSequence* bSeq);
    void learn(const std::string& op, const std::string& label, const std::string& srule);

    void add_learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq);
    void add_learn(const std::string& op, const std::string& label, BaseSequence* bSeq);
    void add_learn(const std::string& op, const std::string& label, const std::string& srule);

    void seq_learn(const ulong op_idx, const ulong label_idx, BaseSequence* bSeq);
    void seq_learn(const std::string& op, const std::string& label, BaseSequence* bSeq);
    void seq_learn(const std::string& op, const std::string& label, const std::string& srule);

    BaseSequence* recall(const std::string& op, const std::string& label);
    BaseSequence* recall(const ulong op_idx, const ulong label_idx);
*/

    void learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq);
    void learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq);
    void learn(const std::string& op, const std::string& label, const std::string& srule);

    void add_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq);
    void add_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq);
    void add_learn(const std::string& op, const std::string& label, const std::string& srule);

    void seq_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq);
    void seq_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq);
    void seq_learn(const std::string& op, const std::string& label, const std::string& srule);

    void stored_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq);
    void stored_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq);
    void stored_learn(const std::string& op, const std::string& label, const std::string& srule);

    void memoize_learn(const ulong op_idx, const ulong label_idx, std::shared_ptr<BaseSequence> bSeq);
    void memoize_learn(const std::string& op, const std::string& label, std::shared_ptr<BaseSequence> bSeq);
    void memoize_learn(const std::string& op, const std::string& label, const std::string& srule);

    std::shared_ptr<BaseSequence> recall(const std::string& op, const std::string& label);
    std::shared_ptr<BaseSequence> recall(const ulong op_idx, const ulong label_idx);

    unsigned int recall_type(const std::string& op, const std::string& label);
    unsigned int recall_type(const ulong op_idx, const ulong label_idx);

    std::vector<ulong> relevant_kets(const ulong op_idx);
    std::vector<ulong> relevant_kets(const std::string &op);

    Sequence active_recall(const std::string& op, const std::string& label);
    Sequence active_recall(const ulong op_idx, const ulong label_idx);

    void print_universe() const;
    void print_multiverse() const;
};


#endif //SDB3_1_CONTEXTLIST_H
