#include <iostream>
#include <memory>

#include "SDB3_header.h"
#include "tclap/CmdLine.h"


std::string introduction_text = "Welcome to the SDB 3.1! Last updated 29th October 2020. Type h for help.\n";


int main() {

    ContextList context("Global context");
    SDB::Driver driver(context);
    std::cout << introduction_text;

    return 0;
}
