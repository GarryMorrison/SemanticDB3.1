//
// Created by Garry Morrison on 12/09/2020.
//

#include <iostream>
#include "KetMap.h"
#include "../Function/split_join.h"

// KetMap ket_map;  // This is now defined in FunctionOperatorMap, just before fn_map.

KetMap::KetMap() {
    std::string s = "";
    our_map[s] = 0;  // EMPTYKETIDX
    our_inverse_map.push_back(s);
    s = "supported-ops";
    our_map[s] = 1; // SUPPORTEDOPSIDX
    our_inverse_map.push_back(s);
    s = "*";
    our_map[s] = 2; // STARIDX
    our_inverse_map.push_back(s);
    map_count = 3;
}

ulong KetMap::get_idx(const std::string& s) {
    ulong result;

    if (our_map.find(s) != our_map.end()) {
        result = our_map[s];
    }
    else {
        our_map[s] = map_count;
        result = map_count;
        map_count++;
        our_inverse_map.push_back(s);

        std::vector<ulong> split_result;
        auto svec = split(s, ": ");
        for (const auto token: svec) {
            if (our_map.find(token) != our_map.end()) {
                split_result.push_back(our_map[token]);
            }
            else {
                our_map[token] = map_count;
                split_result.push_back(map_count);
                map_count++;
                our_inverse_map.push_back(token);
            }
        }
        if (split_result.size() > 1) {
            uvec_map[split_result] = result;
            inverse_uvec_map[result] = split_result;
        }
    }
    return result;
}

ulong KetMap::get_idx(const std::vector<ulong>& uvec) {
    ulong result;
    if (uvec.size() == 0) { return 0; }
    if (uvec.size() == 1) { return uvec[0]; }

    if (uvec_map.find(uvec) != uvec_map.end()) {
        result = uvec_map[uvec];
    }
    else {
        std::string s;
        bool first_pass = true;
        for (const ulong idx: uvec) {
            if (first_pass) {
                s = get_str(idx);
                first_pass = false;
            }
            else {
                s += ": " + get_str(idx);
            }
        }
        return get_idx(s);  // later, swap in code so we aren't merging and then splitting our string.
    }
    return result;
}

std::string KetMap::get_str(const ulong idx) {  // what should we return if idx is not in our_inverse_map?
    std::string result;
    if (idx < our_inverse_map.size()) {
        result = our_inverse_map[idx];
    }
    return result;
}

std::string KetMap::get_str(const std::vector<ulong>& uvec) {
    std::string result;
    ulong idx = get_idx(uvec);
    result = get_str(idx);
    return result;
}

std::vector<ulong> KetMap::get_split_idx(const ulong idx) {
    std::vector<ulong> result;

    if (inverse_uvec_map.find(idx) != inverse_uvec_map.end()) {
        result = inverse_uvec_map[idx];
    }
    else if (idx < map_count) {
        result.push_back(idx);
    }
    else {
        result.push_back(0);
    }
    return result;
}

std::vector<ulong> KetMap::get_split_idx(const std::string& s) {
    std::vector<ulong> result;
    ulong idx = get_idx(s);
    result = get_split_idx(idx);
    return result;
}



ulong KetMap::get_head_idx(const ulong idx) {
    ulong result = 0;
    std::vector<ulong> uvect = get_split_idx(idx);
    if (uvect.size() == 0) { return result; };
    result = uvect[0];
    return result;
}

ulong KetMap::get_tail_idx(const ulong idx) {
    ulong result = 0;
    std::vector<ulong> uvec = get_split_idx(idx);
    if (uvec.size() == 0) { return result; };
    uvec.erase(uvec.begin());
    result = get_idx(uvec);
    return result;
}

ulong KetMap::get_category_idx(const ulong idx) {
    ulong result = 0;
    std::vector<ulong> uvec = get_split_idx(idx);
    if (uvec.size() == 0) { return result; };
    // uvec.erase(uvec.end());
    uvec.pop_back();
    result = get_idx(uvec);
    return result;
}

ulong KetMap::get_value_idx(const ulong idx) {
    ulong result = 0;
    std::vector<ulong> uvect = get_split_idx(idx);
    if (uvect.size() == 0) { return result; };
    result = uvect.back();
    return result;
}

void KetMap::print() {
    std::cout << "map_count: " << map_count << std::endl;
    std::cout << "our_map:" << std::endl;
    for (auto x: our_map) {
        std::cout << "    " << x.first << "\t\t" << x.second << std::endl;
    }
    std::cout << "our_inverse_map:" << std::endl;
    for (auto x: our_inverse_map) {
        std::cout << "    " << x << std::endl;
    }
    std::cout << "uvec_map size: " << uvec_map.size() << std::endl;
    std::cout << "uvec_map:" << std::endl;
    for (auto x: uvec_map) {
        std::cout << "    ";
        for (auto y: x.first) {
            std::cout << y << " ";
        }
        std::cout << ": " << x.second << std::endl;
    }
    std::cout << "inverse_uvec_map size: " << inverse_uvec_map.size() << std::endl;
    std::cout << "inverse_uvec_map:" << std::endl;
    for (auto x: inverse_uvec_map) {
        std::cout << "    " << x.first << ": ";
        for (auto y: x.second) {
            std::cout << y << " ";
        }
        std::cout << std::endl;
    }
}