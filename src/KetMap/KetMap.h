//
// Author: Garry Morrison
// Created: 12/9/2020
//

#ifndef SDB3_1_KETMAP_H
#define SDB3_1_KETMAP_H

#define EMPTYKETIDX 0
#define SUPPORTEDOPSIDX 1
#define STARIDX 2

typedef unsigned long ulong;
#include <unordered_map>
#include <map>
#include <string>
#include <vector>

class KetMap {
private:
    ulong map_count;
    std::unordered_map<std::string, ulong> our_map;
    std::vector<std::string> our_inverse_map;
    std::map<std::vector<ulong>, ulong > uvec_map;
    std::map<ulong, std::vector<ulong> > inverse_uvec_map;

public:
    KetMap();

    ulong get_idx(const std::string& s);
    ulong get_idx(const std::vector<ulong>& uvec);

    std::string get_str(const ulong idx);
    std::string get_str(const std::vector<ulong>& uvec);

    std::vector<ulong> get_split_idx(const ulong idx);
    std::vector<ulong> get_split_idx(const std::string& s);

    ulong get_head_idx(const ulong idx);
    ulong get_tail_idx(const ulong idx);
    ulong get_category_idx(const ulong idx);
    ulong get_value_idx(const ulong idx);

    void print();
};

extern KetMap ket_map;

#endif //SDB3_1_KETMAP_H
