//
// Created by Garry Morrison on 20/11/2020.
//

#include "OperatorUsageMap.h"

OperatorUsageMap operator_usage_map;

OperatorUsageMap::OperatorUsageMap() {
    operator_usage_map.map["empty"] =
            "empty:\n"
            "    description:\n"
            "                \n"
            "    examples:\n"
            "             \n"
            "    see also:\n"
            "             \n";

}

std::string OperatorUsageMap::get_usage(const std::string &s) const {
    auto it = map.find(s);
    if (it == map.end()) { return ""; }
    return it->second;
}

