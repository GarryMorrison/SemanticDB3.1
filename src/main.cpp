#include <iostream>
#include <memory>

#include "SDB3_header.h"
#include "src/Parser/Driver.h"

int main() {

    ContextList context("Global context");
    SDB::Driver driver(context);
    driver.parse_string("37\n3.1415\nfoo\n");
    context.print_universe();

    return 0;
}
