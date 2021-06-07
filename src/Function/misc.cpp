//
// Created by Garry Morrison on 12/09/2020.
//

#include <math.h>
#include <iostream>
#include <sstream>
#include "misc.h"

bool double_eq(const double v1, const double v2) {
    if (fabs(v1 - v2) < EPSILON ) {
        return true;
    }
    return false;
}

bool long_double_eq(const long double v1, const long double v2) {
    if (fabs(v1 - v2) < EPSILON ) {
        return true;
    }
    return false;
}


std::string float_to_int(const long double v, unsigned int places) {
    if (long_double_eq(v, round(v))) {
        return std::to_string(static_cast<long long int>(round(v)));
    }
    // return std::to_string(v); // use decimal places here later!
    // std::cout << "float_to_int, decimal places: " << places << "\n";
    std::ostringstream buffer;
    unsigned int tens = pow(10, places);
    buffer << round(v*tens)/tens;
    return buffer.str();
}

void string_replace_all(std::string &str, const std::string &from, const std::string &to) {
    if (from.empty()) { return; }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

bool string_starts_with_token(const std::string &str, const std::vector<std::string> &tokens) {
    for (const std::string &token: tokens) {
        if (str.size() < token.size()) {
            continue;
        }
        if (str.substr(0, token.size()) == token) {
            return true;
        }
    }
    return false;
}

bool string_ends_with_token(const std::string &str, const std::vector<std::string> &tokens) {
    for (const std::string &token: tokens) {
        if (str.size() < token.size()) {
            continue;
        }
        if (std::equal(token.rbegin(), token.rend(), str.rbegin())) {
            return true;
        }
    }
    return false;
}


std::string string_repeat(const std::string& input, unsigned num) {
    std::string ret;
    ret.reserve(input.size() * num);
    while (num--)
        ret += input;
    return ret;
}

bool is_number(const std::string &str) {  // a quick check if a string is a number or not. NB: not perfect, since invalid numbers will return true.
    return !str.empty() && str.find_first_not_of("-.0123456789") == std::string::npos;
}
