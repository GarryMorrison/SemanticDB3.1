#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <deque>

#include "SDB3_header.h"
#include "tclap/CmdLine.h"


std::string standard_introduction_text = "Welcome to the Semantic DB version 3.1.1\n";
std::string interactive_introduction_text = "\nWelcome to the Semantic DB version 3.1.1 shell.\n"
                                            "Last updated 30th May 2021.\n"
                                            "Type h for help.\n"
                                            "Online usage info at http://semantic-db.org/docs/usage3/\n";

std::string help_string = "\n    q, quit, exit          quit the semantic agent\n"
                          "    h, help                print this message\n"
                          "    dump                   print current context\n"
                          "    dump multi             print context list\n"
                          "    dump verbose           print current context including supported-ops\n"
                          "    dump multi verbose     print context list including supported-ops\n"
                          "    create inverse         create inverse for current context\n"
                          "    create multi inverse   create inverse for all context's\n"
                          "    load file.sw3          load the given file\n"
                          "    save file.sw3          save the current context to file\n"
                          "    save multi file.sw3    save context list to file\n"
                          "    context                show known context's\n"
                          "    context string         set context to string\n"
                          "    icontext               interactively choose context\n"
                          "    .                      repeat last command\n"
                          "    i                      interactive history\n"
                          "    history                show console history\n"
                          "    quiet on               switch time-taken messages off\n"
                          "    quiet off              switch time-taken messages on\n"
                          "    set decimal places p   set default decimal places\n"
                          "    get decimal places     get default decimal places\n"
                          "    usage                  show available operators\n"
                          "    usage op               show op usage information\n"
                          "    reset                  Completely erase all knowledge\n"
                          "    -- comment             ignore, this is just a comment line\n";

unsigned int default_decimal_places;
extern OperatorUsageMap operator_usage_map;
extern DocsGenerator docs_generator;

int main(int argc, char** argv) {

    bool show_command_line_parse_details = false;
    std::deque<std::string> sa_history;
    bool parse_success = true;
    unsigned int history_index = 0;
    std::string file_name;
    ContextList context("Global context");
    SDB::Driver driver(context);

    try {
        TCLAP::CmdLine cmd("The Semantic DB 3.1.1 shell", ' ', "3.1.1");

        // TCLAP::SwitchArg generate_docs_switch("g", "generate", "Generate html documentation (currently unimplemented).", cmd, false);
        TCLAP::ValueArg<std::string> generate_docs_arg("g", "generate", "Generate html documentation in the given directory (currently unimplemented).", false, "", "directory");
        cmd.add(generate_docs_arg);
        TCLAP::ValueArg<unsigned int> decimal_places_arg("p", "places", "Set the default number of decimal places used in kets.", false, 5, "integer");
        cmd.add( decimal_places_arg );
        TCLAP::SwitchArg quiet_switch("q","quiet","Switch to quiet mode. Ie, switch off time-taken messages.", cmd, false);
        TCLAP::SwitchArg dump_switch("d","dump","Dump context list after file(s) are loaded.", cmd, false);
        TCLAP::SwitchArg interactive_switch("i","interactive","Enter interactive mode. Ie, enter the SDB shell.", cmd, false);
        TCLAP::UnlabeledMultiArg<std::string> multi("f", "List of file names to load.", false, "file names", cmd);

        cmd.parse( argc, argv ); // parse the commandline input

        bool interactive_mode = interactive_switch.getValue();
        bool dump_context = dump_switch.getValue();
        bool quiet_mode = quiet_switch.getValue();
        default_decimal_places = decimal_places_arg.getValue();
        // bool generate_docs = generate_docs_switch.getValue();
        std::string generate_docs_starting_directory = generate_docs_arg.getValue();
        std::vector<std::string> file_names = multi.getValue();

        if (show_command_line_parse_details) {
            std::cout << "interactive mode: " << interactive_mode << std::endl;
            std::cout << "dump context: " << dump_context << std::endl;
            std::cout << "quiet mode: " << quiet_mode << std::endl;
            std::cout << "default_decimal_places: " << default_decimal_places << std::endl;
            std::cout << "generate_docs_starting_directory: " << generate_docs_starting_directory << std::endl;

            int k = 0;
            for (const auto &file_name: file_names) {
                k++;
                std::cout << "file name " << k << ": " << file_name << std::endl;
            }
        }

        if (!generate_docs_starting_directory.empty()) {
            std::cout << "\n    About to generate html documentation in directory: " << generate_docs_starting_directory << "\n    Do you wish to proceed? (y/n): ";
            char answer;
            std::cin >> answer;
            if (answer == 'y') {
                std::cout << "\n    Working ... \n" << std::endl;
                docs_generator.generate(generate_docs_starting_directory);
            }
            return 0;
        }

        if (!interactive_mode && file_names.empty()) {
            std::cout << standard_introduction_text;
            std::cout << "Invoke the shell with: " << argv[0] << " --interactive\n";
            std::cout << "Or print the help message: " << argv[0] << " --help\n\n";
            return 0;
        }
        if (!interactive_mode) {
            for (const auto &file_name: file_names) {
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
        {
            Timer_ms timer("\n    Time taken", quiet_mode || file_names.empty());  // Time the load time, but only if there are files to load.
            for (const auto &file_name: file_names) {  // load up the files specified on the command line:
                parse_success = driver.parse_file(file_name);
                if (!parse_success) {
                    std::cout << "Parse failed for file: " << file_name << std::endl;
                }
            }
            if (dump_context) {  // if dump specified, then do the context dump:
                context.print_multiverse();
            }
        }
        std::string shell_input;
        unsigned int current_indentation = 0;
        unsigned int indentation_delta = 4;  // Note that 4 is (currently) hard-wired into our parser.
        std::string multi_line_suffix = "=>";  // String used to indicate the start of a multi line rule.
        std::vector<std::string> indent_line_tokens{ "if("};
        std::vector<std::string> middle_tokens{ "else:" };
        std::vector<std::string> undent_line_tokens{ "end:"};
        std::string indentation_prefix;

        while (true) {
            std::cout << "\nsa: ";
            getline(std::cin, shell_input);
            if (std::equal(multi_line_suffix.rbegin(), multi_line_suffix.rend(), shell_input.rbegin())) {  // Multi line input is currently incomplete. It needs to handle if-else statements.
                std::string multi_line_shell_input = shell_input + "\n";
                current_indentation += indentation_delta;
                bool inside_multi_line = true;
                while (inside_multi_line) {
                    indentation_prefix = repeat(" ", current_indentation);
                    std::cout << indentation_prefix + ": ";
                    getline(std::cin, shell_input);
                    if (!shell_input.empty() && shell_input != "\n") {
                        if (string_starts_with_token(shell_input, undent_line_tokens)) {
                            current_indentation -= indentation_delta;
                            indentation_prefix = repeat(" ", current_indentation);
                        } else if (string_starts_with_token(shell_input, middle_tokens)) {
                            indentation_prefix = repeat(" ", current_indentation - indentation_delta);
                        }
                        multi_line_shell_input += indentation_prefix + shell_input + "\n";
                        if (string_starts_with_token(shell_input, indent_line_tokens)) {
                            current_indentation += indentation_delta;
                        }
                    } else {
                        current_indentation -= indentation_delta;
                        inside_multi_line = false;
                    }
                }
                shell_input = multi_line_shell_input + "\n";
            }
            if (shell_input == ".") {
                if (sa_history.empty()) {
                    continue;
                }
                shell_input = sa_history.back();
                // sa_history.emplace_back(".");       // This bugs out if . is invoked twice in a row.
                // sa_history.emplace_back(shell_input);  // This solves the bug, but . will now not be recorded in the history. Instead it will be the full command.
            }
            sa_history.emplace_back(shell_input);
            if (shell_input == "i") {
                history_index = 0;
                std::cout << "\nInteractive history:\n";
                for (const auto &line: sa_history) {
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
            } else if (shell_input == "dump verbose") {
                context.print_universe(false);
            } else if (shell_input == "dump multi verbose") {
                context.print_multiverse(false);
            } else if (shell_input == "create inverse") {
                ulong star_idx = ket_map.get_idx("*");
                context.find_inverse(star_idx);
            } else if (shell_input == "create multi inverse") {
                ulong star_idx = ket_map.get_idx("*");
                context.find_multi_inverse(star_idx);
            } else if (shell_input.substr(0, 5) == "load ") {
                file_name = shell_input.substr(5);
                Timer_ms timer("\n    Time taken", quiet_mode);  // Time the load time.
                parse_success = driver.parse_file(file_name);  // Need security check on file we are loading?
                if (!parse_success) {
                    std::cout << "Parse failed for file: " << file_name << std::endl;
                }
            } else if (shell_input.substr(0, 11) == "save multi ") {
                file_name = shell_input.substr(11);
                std::ofstream file(file_name.c_str());
                std::cout << "\n    Writing context list to file: " << file_name;
                Timer_ms timer("    Time taken", quiet_mode);  // Time the save time.
                context.print_multiverse(true, file);
            } else if (shell_input.substr(0, 5) == "save ") {
                file_name = shell_input.substr(5);
                std::ofstream file(file_name.c_str());
                std::cout << "\n    Writing context to file: " << file_name << std::endl;  // I don't understand why this needs extra new lines
                Timer_ms timer("\n    Time taken", quiet_mode);  // Time the save time.    // that "save multi" does not.
                context.print_universe(true, file);
            } else if (shell_input.substr(0, 2) == "--") {
                continue;
            } else if (shell_input == "history") {
                std::cout << "\nSemantic agent history:\n";
                for (const auto &line: sa_history) {
                    std::cout << "    " << line << std::endl;
                }
            } else if (shell_input == "quiet on") {
                quiet_mode = true;
                std::cout << "Switched to quiet mode." << std::endl;
            } else if (shell_input == "quiet off") {
                quiet_mode = false;
                std::cout << "Switched off quiet mode." << std::endl;
            } else if (shell_input == "get decimal places") {
                std::cout << "\nDefault decimal places: " << default_decimal_places << std::endl;
            } else if (shell_input.substr(0, 18) == "set decimal places") {
                default_decimal_places = std::stoi(shell_input.substr(18));  // crashes if not given an integer.
                std::cout << "\nDefault decimal places set to: " << default_decimal_places << std::endl;
            } else if (shell_input == "context") {
                context.show_context_list();
            } else if (shell_input.substr(0, 8) == "context ") {
                context.set(shell_input.substr(8));
            } else if (shell_input == "icontext") {
                context.show_context_list();
                std::cout << "\nEnter your selection: ";
                std::cin >> history_index;
                std::cin.ignore();
                context.set(history_index);
            } else if (shell_input == "usage") {
                std::cout << "\nBelow is a list of known operators, and their invocation type." << std::endl;
                std::cout << "Those marked with a * have more detailed usage information defined." << std::endl;
                std::cout << "This is accessed using: usage op\n" << std::endl;
                fn_map.print();
            } else if (shell_input.substr(0, 6) == "usage ") {
                std::cout << operator_usage_map.get_usage(shell_input.substr(6)) << std::endl;
            } else if (shell_input == "reset") {
                std::cout << "\n    Warning! All knowledge will be erased. Proceed? (y/n): ";
                char input_char;
                std::cin >> input_char;
                std::cin.ignore();
                if (input_char == 'y') {
                    context.reset();
                    std::cout << "    Gone ..." << std::endl;
                }
            } else {
                Timer_ms timer("\n    Time taken", quiet_mode);  // Time the execution of the command. The destructor prints the results.
                parse_success = driver.parse_string(shell_input + "\n");  // Is there a cleaner way than adding \n here?
                if (!parse_success) {
                    std::cout << "Parse failed for command:\n" << shell_input << std::endl;
                }
            }
        }
        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    return 0;
}
