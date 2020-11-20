//
// Created by Garry Morrison on 20/11/2020.
//

#ifndef SDB3_1_OPERATORUSAGEMAP_H
#define SDB3_1_OPERATORUSAGEMAP_H

#include <map>
#include <string>


class OperatorUsageMap {
private:
public:
    std::map<std::string, std::string> map;
    OperatorUsageMap();
    std::string get_usage(const std::string &s) const;
};

#endif //SDB3_1_OPERATORUSAGEMAP_H
