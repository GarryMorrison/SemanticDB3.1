//
// Created by Garry Morrison on 12/09/2020.
//

#ifndef SDB3_1_SPLIT_JOIN_H
#define SDB3_1_SPLIT_JOIN_H

std::vector<std::string> split(const std::string& s, const std::string& delimiter);
std::vector<std::string> view_split(std::string_view s, std::string_view delimiter);
std::string join(const std::vector<std::string>& v, const std::string& delimiter);

#endif //SDB3_1_SPLIT_JOIN_H
