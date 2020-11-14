#include <iostream>
#include <memory>

#include "SDB3_header.h"
#include "tclap/CmdLine.h"


std::string introduction_text = "Welcome to the SDB 3.1! Last updated 14th November 2020. Type h for help.\n";


int main(int argc, char** argv) {

    ContextList context("Global context");
    SDB::Driver driver(context);
    std::cout << introduction_text;

    try {
        TCLAP::CmdLine cmd("The Semantic DB 3.1 shell", ' ', "3.1");

        TCLAP::SwitchArg interactive_switch("i","interactive","Enter interactive mode", cmd, false);
        TCLAP::SwitchArg dump_switch("d","dump","Dump context", cmd, false);
        TCLAP::SwitchArg quiet_switch("q","quiet","Switch to quiet mode", cmd, false);
        TCLAP::UnlabeledMultiArg<std::string> multi("f", "List of file names", false, "file names", cmd);

        cmd.parse( argc, argv ); // parse the commandline input

        bool interactive_mode = interactive_switch.getValue();
        bool dump_context = dump_switch.getValue();
        bool quiet_mode = quiet_switch.getValue();
        std::vector<std::string> file_names = multi.getValue();

        std::cout << "interactive mode: " << interactive_mode << std::endl;
        std::cout << "dump context: " << dump_context << std::endl;
        std::cout << "quiet mode: " << quiet_mode << std::endl;

        int k = 0;
        for (auto s: file_names) {
            k++;
            std::cout << "file name " << k << ": " << s << std::endl;
        }

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}
