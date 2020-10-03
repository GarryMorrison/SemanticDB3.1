//
// Created by Garry Morrison on 26/09/2020.
//

#include "Timer.h"
#include <vector>
#include "split_join.h"


std::string display_time(const long long time) {  // time_interval_map assumes times are in milliseconds.
    std::vector<std::string > result;
    long long working_time = time;

    for (auto iter = time_interval_map.rbegin(); iter != time_interval_map.rend(); ++iter) {
        long long value = working_time / iter->first;
        if (value > 0 ) {
            working_time -= value * iter->first;
            std::string plural_char = "s";
            if (value == 1) { plural_char = ""; }
            result.push_back(std::to_string(value) + " " + iter->second + plural_char);
        }
    }
    if (result.empty()) { return "0"; }
    return join(result, ", ");
}
