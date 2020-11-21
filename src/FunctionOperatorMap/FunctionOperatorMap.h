//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_FUNCTIONOPERATORMAP_H
#define SDB3_1_FUNCTIONOPERATORMAP_H

#include <unordered_map>
#include <functional>
#include "../Sequence/Sequence.h"
#include "../Context/ContextList.h"

template <class T>
void print_map(const std::string& s, T& our_map);

class FunctionOperatorMap {
private:
public:
    FunctionOperatorMap();  // this is where we register our functions and operators.

    // below are our tables:
    std::unordered_map<ulong, std::function<Sequence(const Sequence*)> > built_in;
    std::unordered_map<ulong, std::function<Sequence(const Sequence*, const std::vector<std::shared_ptr<CompoundConstant> >&)> > compound_built_in;

    std::unordered_map<ulong, std::function<double(const double)> > sigmoids;
    std::unordered_map<ulong, std::function<Sequence(const Ket&)> > ket_fn;
    std::unordered_map<ulong, std::function<Sequence(const Superposition&)> > sp_fn;
    std::unordered_map<ulong, std::function<Sequence(const Sequence&)> > seq_fn;

    std::unordered_map<ulong, std::function<double(const double, const std::vector<std::shared_ptr<CompoundConstant> >&) > > compound_sigmoids;
    std::unordered_map<ulong, std::function<Sequence(const Ket, const std::vector<std::shared_ptr<CompoundConstant> >&) > > compound_ket_fn;
    std::unordered_map<ulong, std::function<Sequence(const Superposition&, const std::vector<std::shared_ptr<CompoundConstant> >&) > > compound_sp_fn;
    std::unordered_map<ulong, std::function<Sequence(const Sequence&, const std::vector<std::shared_ptr<CompoundConstant> >&) > > compound_seq_fn;

    std::unordered_map<ulong, std::function<Sequence(const Superposition&, ContextList&, const std::vector<std::shared_ptr<CompoundConstant> >&) > > compound_context_sp_fn;
    std::unordered_map<ulong, std::function<Sequence(const Sequence&, ContextList&, const std::vector<std::shared_ptr<CompoundConstant> >&) > > compound_context_seq_fn;

    std::unordered_map<ulong, std::function<Sequence(const Sequence&, const Sequence&)> > whitelist_1;
    std::unordered_map<ulong, std::function<Sequence(const Sequence&, const Sequence&, const Sequence&)> > whitelist_2;
    std::unordered_map<ulong, std::function<Sequence(const Sequence&, const Sequence&, const Sequence&, const Sequence&)> > whitelist_3;
    std::unordered_map<ulong, std::function<Sequence(const Sequence&, const Sequence&, const Sequence&, const Sequence&, const Sequence&)> > whitelist_4;

    std::unordered_map<ulong, std::function<Sequence(ContextList&, const Sequence&, const Sequence&)> > context_whitelist_1;
    std::unordered_map<ulong, std::function<Sequence(ContextList&, const Sequence&, const Sequence&, const Sequence&)> > context_whitelist_2;
    std::unordered_map<ulong, std::function<Sequence(ContextList&, const Sequence&, const Sequence&, const Sequence&, const Sequence&)> > context_whitelist_3;
    std::unordered_map<ulong, std::function<Sequence(ContextList&, const Sequence&, const Sequence&, const Sequence&, const Sequence&, const Sequence&)> > context_whitelist_4;

    void print() const;
};

extern FunctionOperatorMap fn_map;

#endif //SDB3_1_FUNCTIONOPERATORMAP_H
