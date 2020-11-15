#include <iostream>
#include <memory>
#include <deque>

#include "SDB3_header.h"
#include "tclap/CmdLine.h"


std::string standard_introduction_text = "Welcome to the Semantic DB version 3.1\n";
std::string interactive_introduction_text = "\nWelcome to the Semantic DB version 3.1 shell.\nLast updated 14th November 2020.\nType h for help.\n";

std::string help_string = "\n    q, quit, exit        quit the semantic agent\n"
                          "    h, help              print this message\n"
                          "    dump                 print current context\n"
                          "    dump multi           print context list\n"
                          "    load file.sw3        load the given file\n"
                          "    .                    repeat last command\n"
                          "    i                    interactive history\n"
                          "    history              show console history\n"
                          "    -- comment           ignore, this is just a comment line\n";


int main(int argc, char** argv) {

    std::deque<std::string> sa_history;
    bool parse_success = true;
    unsigned int history_index = 0;
    std::string file_name;
    ContextList context("Global context");
    SDB::Driver driver(context);

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
        for (const auto file_name: file_names) {
            k++;
            std::cout << "file name " << k << ": " << file_name << std::endl;
        }

        if (!interactive_mode && file_names.empty()) {
            std::cout << standard_introduction_text;
            return 0;
        }
        if (!interactive_mode) {
            for (const auto file_name: file_names) {
                parse_success = driver.parse_file(file_name);
                if (!parse_success) {
                    std::cout << "Parse failed for file: " << file_name << std::endl;
                }
            }
            if (dump_context) {
                context.print_multiverse();
            }
            return 0;
        }

        // Start the semantic agent:
        std::cout << interactive_introduction_text;
        for (const auto file_name: file_names) {  // load up the files specified on the command line:
            parse_success = driver.parse_file(file_name);
            if (!parse_success) {
                std::cout << "Parse failed for file: " << file_name << std::endl;
            }
        }
        if (dump_context) {  // if dump specified, then do the context dump:
            context.print_multiverse();
        }
        std::string shell_input;
        while (true) {
            std::cout << "\nsa: ";
            getline(std::cin, shell_input);
            if (shell_input == ".") {
                if (sa_history.empty()) {
                    continue;
                }
                shell_input = sa_history.back();
                sa_history.emplace_back(".");
            } else {
                sa_history.emplace_back(shell_input);
            }
            if (shell_input == "i") {
                history_index = 0;
                std::cout << "\nInteractive history:\n";
                for (const auto line: sa_history) {
                    std::cout << "    " << history_index << ")    " << line << std::endl;
                    history_index++;
                }
                std::cout << "\nEnter your selection: ";
                std::cin >> history_index;
                std::cin.ignore();
                if (history_index < 0 || history_index >= sa_history.size()) {
                    continue;
                }
                shell_input = sa_history.at(history_index);
                std::cout << "Your selection: " << shell_input << std::endl;
            }
            if (shell_input.empty()) { continue; }
            if (shell_input == "q" || shell_input == "quit" || shell_input == "exit") {
                std::cout << "Exiting ... " << std::endl;
                return 0;
            } else if (shell_input == "h" || shell_input == "help") {
                std::cout << help_string;
            } else if (shell_input == "dump") {
                context.print_universe();
            } else if (shell_input == "dump multi") {
                context.print_multiverse();
            } else if (shell_input.substr(0, 5) == "load ") {
                file_name = shell_input.substr(5);
                Timer_ms timer("\n    Time taken", quiet_mode);  // Time the load time.
                parse_success = driver.parse_file(file_name);  // Need security check on file we are loading?
                if (!parse_success) {
                    std::cout << "Parse failed for file: " << file_name << std::endl;
                }
            } else if (shell_input.substr(0, 2) == "--") {
                continue;
            } else if (shell_input == "history") {
                std::cout << "\nSemantic agent history:\n";
                for (const auto line: sa_history) {
                    std::cout << "    " << line << std::endl;
                }
            } else {
                Timer_ms timer("\n    Time taken", quiet_mode);  // Time the execution of the command. The destructor prints the results.
                parse_success = driver.parse_string(shell_input + "\n");  // Is there a cleaner way than adding \n here?
                if (!parse_success) {
                    std::cout << "Parse failed for command: " << shell_input << std::endl;
                }
            }
        }
        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}
