//
// Created by Garry Morrison on 2/06/2021.
//

#include "WhileStatement.h"

const size_t WhileStatement::size() const {
    return while_rules.size();
}

const std::string WhileStatement::to_string() const {
    std::string s = "while( " + condition.to_string() + " ):" + while_rules.to_string("    ");
    return s + "    end:";
}

const std::string WhileStatement::to_string(const std::string &prefix) const {
    std::string s = prefix + "while( " + condition.to_string() + " ):" + while_rules.to_string(prefix + "    ");  // prefix + "    " or "    " + prefix?
    return s + prefix + "    end:";
}


Sequence WhileStatement::Compile(ContextList &context) const {
    // std::cout << "WhileStatement::Compile(context):\n";
    ulong yes_idx = ket_map.get_idx("yes");
    ulong condition_idx = condition.Compile(context).to_ket().label_idx();
    while (condition_idx == yes_idx) {
        while_rules.Compile(context);
        condition_idx = condition.Compile(context).to_ket().label_idx();
    }
    return Sequence();
}

Sequence WhileStatement::Compile(ContextList &context, const Ket &label_ket) const {
    // std::cout << "WhileStatement::Compile(context, label_ket):\n";
    ulong yes_idx = ket_map.get_idx("yes");
    ulong condition_idx = condition.Compile(context, label_ket).to_ket().label_idx();
    while (condition_idx == yes_idx) {
        while_rules.Compile(context, label_ket);
        condition_idx = condition.Compile(context, label_ket).to_ket().label_idx();
    }
    return Sequence();
}

Sequence WhileStatement::Compile(ContextList &context, const Ket &label_ket, const Ket &multi_label_ket) const {
    // std::cout << "WhileStatement::Compile(context, label_ket, multi_label_ket):\n";
    ulong yes_idx = ket_map.get_idx("yes");
    ulong condition_idx = condition.Compile(context, label_ket, multi_label_ket).to_ket().label_idx();
    while (condition_idx == yes_idx) {
        while_rules.Compile(context, label_ket, multi_label_ket);
        condition_idx = condition.Compile(context, label_ket, multi_label_ket).to_ket().label_idx();
    }
    return Sequence();
}

Sequence WhileStatement::Compile(ContextList &context, const Ket &label_ket, const std::vector<Sequence> &args) const {
    // std::cout << "WhileStatement::Compile(context, label_ket, args):\n";
    ulong yes_idx = ket_map.get_idx("yes");
    ulong condition_idx = condition.Compile(context, label_ket, args).to_ket().label_idx();
    while (condition_idx == yes_idx) {
        while_rules.Compile(context, label_ket, args);
        condition_idx = condition.Compile(context, label_ket, args).to_ket().label_idx();
    }
    return Sequence();
}

