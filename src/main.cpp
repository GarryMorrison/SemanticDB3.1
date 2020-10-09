#include <iostream>
#include <memory>

#include "SDB3_header.h"
#include "src/Parser/Driver.h"

int main() {

    ContextList context("Global context");
    SDB::Driver driver(context);
    // driver.parse_string("37\n3.1415\n|_self3>\nfoo \n-- just a random comment\n|some ket>\n");
    driver.parse_string("5.7|bah> + 3|bah> - 2.1|bah>\n");
    driver.parse_string(" (3|a> + 2.2|b> . |c> . |x> . 2|y> - 3|z> ) \n");
    driver.parse_string("3.1|x> __ 2|y>\n");
    driver.parse_string("op |foo> => 2.7|bah>\n");
    driver.parse_string("op1 |*> #=> |op1:> __ |_self>\n");
    driver.parse_string("op2 op1|X> #=> 7.2|Y>\n");
    driver.parse_string("op3 op2 op1 |bah> #=> 7|foo>\n");
    driver.parse_string("op |foo>\n");
    driver.parse_string("op2 |bah>\n");
    driver.parse_string("op3 op2 7 3.2 op1\n");
    driver.parse_string("|bah>\n");
    driver.parse_string("5|bah>\n");
    driver.parse_string("op3 2 op2 5.1 op1|bah>\n");
    driver.parse_string("op2 op1|foo> + 3.7|bah> - 9|_self> . |x> _ |y> __ 2.1|z>\n");
    driver.parse_string("fn[*, \"str\"]\n");
    context.print_universe();

    return 0;
}
