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
    driver.parse_string("testfn[2, 3.14, op3, \"...\", *]\n");
    driver.parse_string("testfn[2, 3.14, op3, op2]\n");
    driver.parse_string("testfn[2, 3.14, *, \"str\", op3]\n");
    driver.parse_string("op3 |Y> => ssplit[\", \"] |a, b, c, d, e>\n");
    driver.parse_string("op4 |Z> => rel-kets[*] |>\n");
    driver.parse_string("op5 |Z> => simm(|a> + |b> + |c>, |b>) |>\n");
    driver.parse_string("op6 |Z> => seq2sp srange(|1>, |5>) |>\n");
    driver.parse_string("op7 |Z> => op1 ( + 1 - 2 op1 . 3) |x>\n");
    driver.parse_string("op8 |Z> => 5^3 op1^2 |z>\n");
    driver.parse_string("op9 |Z> #=>\n    print |Hello ... >\n    print |... there!>\n    |return value>\n");
    context.print_universe();

    return 0;
}
