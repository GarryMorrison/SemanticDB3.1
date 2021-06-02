//
// Created by Garry Morrison on 31/05/2021.
//

#include "ForStatement.h"

const size_t ForStatement::size() const {
    return for_rules.size();
}

const std::string ForStatement::to_string() const {
    std::string type;
    if (for_type == SFOR_TYPE) { type = "s"; }
    std::string s = type + "for( " + ket_map.get_str(op_idx) + "|" + ket_map.get_str(ket_idx) + "> in " + iterables.to_string() + " ):" + for_rules.to_string("    ");
    return s + "    end:";
}

const std::string ForStatement::to_string(const std::string &prefix) const {
    std::string type;
    if (for_type == SFOR_TYPE) { type = "s"; }
    std::string s = prefix + type + "for( " + ket_map.get_str(op_idx) + "|" + ket_map.get_str(ket_idx) + "> in " + iterables.to_string() + " ):" + for_rules.to_string(prefix + "    ");
    return s + prefix + "    end:";
}


Sequence ForStatement::Compile(ContextList &context) const {
    // std::cout << "ForStatement::Compile(context):\n";
    Sequence iter_vars = iterables.Compile(context);
    if (for_type == SFOR_TYPE) {
        for (const auto &sp: iter_vars) {
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Superposition>(sp);
            context.learn(op_idx, ket_idx, bSeq);
            for_rules.Compile(context);
        }
    } else if (for_type == FOR_TYPE) {
        for (const auto &sp: iter_vars) {
            for (const auto &k: sp) {  // Potentially very slow!
                std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(k);
                context.learn(op_idx, ket_idx, bSeq);
                for_rules.Compile(context);
            }
        }
    }
    return Sequence();
}

Sequence ForStatement::Compile(ContextList &context, const Ket &label_ket) const {
    // std::cout << "ForStatement::Compile(context, label_ket):\n";
    Sequence iter_vars = iterables.Compile(context, label_ket);
    if (for_type == SFOR_TYPE) {
        for (const auto &sp: iter_vars) {
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Superposition>(sp);
            context.learn(op_idx, ket_idx, bSeq);
            for_rules.Compile(context, label_ket);
        }
    } else if (for_type == FOR_TYPE) {
        for (const auto &sp: iter_vars) {
            for (const auto &k: sp) {  // Potentially very slow!
                std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(k);
                context.learn(op_idx, ket_idx, bSeq);
                for_rules.Compile(context, label_ket);
            }
        }
    }
    return Sequence();
}

Sequence ForStatement::Compile(ContextList &context, const Ket &label_ket, const Ket &multi_label_ket) const {
    // std::cout << "ForStatement::Compile(context, label_ket, multi_label_ket):\n";
    Sequence iter_vars = iterables.Compile(context, label_ket, multi_label_ket);
    if (for_type == SFOR_TYPE) {
        for (const auto &sp: iter_vars) {
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Superposition>(sp);
            context.learn(op_idx, ket_idx, bSeq);
            for_rules.Compile(context, label_ket, multi_label_ket);
        }
    } else if (for_type == FOR_TYPE) {
        for (const auto &sp: iter_vars) {
            for (const auto &k: sp) {  // Potentially very slow!
                std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(k);
                context.learn(op_idx, ket_idx, bSeq);
                for_rules.Compile(context, label_ket, multi_label_ket);
            }
        }
    }
    return Sequence();
}


Sequence ForStatement::Compile(ContextList &context, const Ket &label_ket, const std::vector<Sequence> &args) const {
    // std::cout << "ForStatement::Compile(context, label_ket, args):\n";
    Sequence iter_vars = iterables.Compile(context, label_ket, args);
    if (for_type == SFOR_TYPE) {
        for (const auto &sp: iter_vars) {
            std::shared_ptr<BaseSequence> bSeq = std::make_shared<Superposition>(sp);
            context.learn(op_idx, ket_idx, bSeq);
            for_rules.Compile(context, label_ket, args);
        }
    } else if (for_type == FOR_TYPE) {
        for (const auto &sp: iter_vars) {
            for (const auto &k: sp) {  // Potentially very slow!
                std::shared_ptr<BaseSequence> bSeq = std::make_shared<Ket>(k);
                context.learn(op_idx, ket_idx, bSeq);
                for_rules.Compile(context, label_ket, args);
            }
        }
    }
    return Sequence();
}
