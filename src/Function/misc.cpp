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
    std::ostringstream buffer;
    unsigned int tens = pow(10, places);
    buffer << round(v*tens)/tens;
    return buffer.str();
}
