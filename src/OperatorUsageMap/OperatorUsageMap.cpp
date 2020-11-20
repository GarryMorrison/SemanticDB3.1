//
// Created by Garry Morrison on 20/11/2020.
//

#include "OperatorUsageMap.h"

OperatorUsageMap operator_usage_map;

OperatorUsageMap::OperatorUsageMap() {
    operator_usage_map.map["empty"] =
            "\nempty:\n"
            "    description:\n\n"
            "    examples:\n\n"
            "    see also:\n";

    operator_usage_map.map["clean"] =
            "\nclean:\n"
            "    description:\n"
            "        clean ket\n"
            "        clean the coefficients of the given superposition\n"
            "        if x < 0, return 0, else return 1\n\n"
            "    examples:\n"
            "        clean (3|a> + 2.2|b> - 3 |c> + |d>)\n"
            "            |a> + |b> + 0|c> + |d>\n\n"
            "        drop clean (3|a> + 2.2|b> - 3 |c> + |d>)\n"
            "            |a> + |b> + |d>\n\n"
            "    see also:\n"
            "        drop\n";

    operator_usage_map.map["drop"] =
            "\ndrop:\n"
            "    description:\n"
            "        drop sp\n"
            "        drop all kets with coeff <= 0 from the given superposition\n\n"
            "    examples:\n"
            "        drop (|a> + 0|b> - 2|c> + 7.1|d>)\n"
            "            |a> + 7.100000|d>\n\n"
            "    see also:\n"
            "        drop-below, drop-above, sdrop\n";

}

std::string OperatorUsageMap::get_usage(const std::string &s) const {
    auto it = map.find(s);
    if (it == map.end()) { return ""; }
    return it->second;
}

