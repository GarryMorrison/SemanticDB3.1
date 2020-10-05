#include <iostream>
#include <memory>

#include "SDB3_header.h"
#include "src/Parser/Driver.h"

int main() {

    ContextList context("Global context");
    SDB::Driver driver(context);
    driver.parse_string("37\n3.1415\n|_self3>\nfoo\n-- just a random comment\n|some ket>\n");
    driver.parse_string("5.7|bah> + 3|bah> - 2.1|bah>");
    driver.parse_string("3|a> + 2.2|b> . |c> . |x> . 2|y> - 3|z>");
    driver.parse_string("3.1|x> __ 2|y>");
    driver.parse_string("op |foo> => 2.7|bah>");
    driver.parse_string("op2 |bah> #=> 7|foo>");
    driver.parse_string("op |foo>");
    driver.parse_string("op2 |bah>");
    context.print_universe();

    return 0;
}
