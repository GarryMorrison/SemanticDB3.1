//
// Created by Garry Morrison on 29/05/2021.
//

#include "IfElseStatement.h"


const size_t IfElseStatement::size() const {
    return if_rules.size() + else_rules.size();  // Not sure the best way to return size here.
}

const std::string IfElseStatement::to_string() const {
    std::string s = "if( " + condition.to_string() + " ):" + if_rules.to_string("    ");
    if (else_rules.size() > 0) { s += "    else:" + else_rules.to_string("    "); }
    return s + "    end:";
}

const std::string IfElseStatement::to_string(const std::string &prefix) const {
    std::string s = prefix + "if( " + condition.to_string() + " ):" + if_rules.to_string(prefix + "    ");  // prefix + "    " or "    " + prefix?
    if (else_rules.size() > 0) { s += prefix + "    else:" + else_rules.to_string(prefix + "    "); }
    return s + prefix + "    end:";
}


Sequence IfElseStatement::Compile(ContextList &context) const {
    // std::cout << "IfElseStatement::Compile(context):\n";
    std::string condition_str = condition.Compile(context).to_ket().label();
    if (condition_str == "yes") {
        return if_rules.Compile(context);
    } else {
        return else_rules.Compile(context);
    }
    return Sequence();
}

Sequence IfElseStatement::Compile(ContextList &context, const Ket &label_ket) const {
    // std::cout << "IfElseStatement::Compile(context, label_ket):\n";
    std::string condition_str = condition.Compile(context, label_ket).to_ket().label();
    if (condition_str == "yes") {
        return if_rules.Compile(context, label_ket);
    } else {
        return else_rules.Compile(context, label_ket);
    }
    return Sequence();
}

Sequence IfElseStatement::Compile(ContextList &context, const Ket &label_ket, const Ket &multi_label_ket) const {
    // std::cout << "IfElseStatement::Compile(context, label_ket, multi_label_ket):\n";
    std::string condition_str = condition.Compile(context, label_ket, multi_label_ket).to_ket().label();
    if (condition_str == "yes") {
        return if_rules.Compile(context, label_ket, multi_label_ket);
    } else {
        return else_rules.Compile(context, label_ket, multi_label_ket);
    }
    return Sequence();
}

Sequence IfElseStatement::Compile(ContextList &context, const Ket &label_ket, const std::vector<Sequence> &args) const {
    // std::cout << "IfElseStatement::Compile(context, label_ket, args):\n";
    std::string condition_str = condition.Compile(context, label_ket, args).to_ket().label();
    if (condition_str == "yes") {
        return if_rules.Compile(context, label_ket, args);
    } else {
        return else_rules.Compile(context, label_ket, args);
    }
    return Sequence();
}

