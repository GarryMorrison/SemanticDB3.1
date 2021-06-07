//
// Created by Garry Morrison on 12/09/2020.
//



#ifndef SDB3_1_MISC_H
#define SDB3_1_MISC_H

#include <string>
#include <vector>

extern unsigned int default_decimal_places;

const double EPSILON = 0.0001;
bool double_eq(const double v1, const double v2);
bool long_double_eq(const long double v1, const long double v2);
std::string float_to_int(const long double v, unsigned int places);
void string_replace_all(std::string &str, const std::string &from, const std::string &to);
bool string_starts_with_token(const std::string &str, const std::vector<std::string> &tokens);
bool string_ends_with_token(const std::string &str, const std::vector<std::string> &tokens);
std::string string_repeat(const std::string& input, unsigned num);
bool is_number(const std::string &str);

#endif //SDB3_1_MISC_H
