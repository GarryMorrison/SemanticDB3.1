//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_NEWCONTEXT_H
#define SDB3_1_NEWCONTEXT_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../Sequence/Superposition.h"
#include "Frame.h"
#include "../Sequence/BaseSequence.h"


class NewContext {
private:
    std::string name;
    std::unordered_map<ulong, Frame> rules_dict;
    std::vector<ulong> sort_order;
    std::unordered_map<ulong, Frame> fn_rules_dict;
    std::vector<ulong> fn_sort_order;

public:
    // NewContext(const std::string& s) { name = s; };
    // NewContext(const std::string s) { name = s; };
    NewContext(const std::string& s);
    std::string get_name() const { return name; };

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
//        BaseRule* active_recall(const ulong op_idx, const ulong label_idx);
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

    unsigned int recall_descent_type(const std::string& op, const std::string& label);
    unsigned int recall_descent_type(const ulong op_idx, const ulong label_idx);

    std::vector<ulong> relevant_kets(const ulong op_idx);
    std::vector<ulong> relevant_kets(const std::string &op);


    // void fn_learn(const ulong op_idx, const ulong param_size, BaseRule* brule);
    // BaseRule* fn_recall(const ulong op_idx, const ulong param_size);
    void print_universe() const;
};


#endif //SDB3_1_NEWCONTEXT_H
