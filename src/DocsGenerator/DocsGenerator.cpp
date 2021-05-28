//
// Created by Garry Morrison on 22/04/2021.
//

#include <iostream>
#include <fstream>
#include <time.h>
#include <filesystem>
#include <map>
#include "DocsGenerator.h"
#include "../FunctionOperatorMap/FunctionOperatorMap.h"
#include "../OperatorUsageMap/OperatorUsageMap.h"
#include "../Function/misc.h"


namespace fs = std::filesystem;

DocsGenerator docs_generator;
extern FunctionOperatorMap fn_map;
extern OperatorUsageMap operator_usage_map;

const std::string docs_index_page =
        "<html>\n"
        "<head><title>Semantic DB 3.1 usage information</title></head>\n"
        "<body>\n"
        "<h3>Semantic DB 3.1 usage information</h3>\n"
        "Welcome to the Semantic DB 3.1 usage page. Below are brief descriptions and examples for our operators and sequence functions.\n"
        "The corresponding github repo is <a href=\"https://github.com/GarryMorrison/SemanticDB3.1\">available here</a>.\n"
        "<hr>\n"
        "$body$\n"
        "<hr>\n"
        "by Garry Morrison<br>\n"
        "email: garry -at- semantic-db.org<br>\n"
        "updated: $date-time$\n"
        "\n</body>\n</html>\n";

const std::string docs_operator_page =
        "<html>\n"
        "<head><title>$operator-type$: $operator-name$</title></head>\n"
        "<body>\n"
        "<h3>$operator-type$: $operator-name$</h3>\n"
        "<hr>\n"
        "<pre>\n"
        "$operator-usage$"
        "</pre>\n"
        "<hr>\n"
        "<a href=\"../index.html\">Home</a><br>\n"
        "</body>\n"
        "</html>";

const std::string docs_sw3_page =
        "<html>\n"
        "<head><title>$sw3-type$: $sw3-name$</title></head>\n"
        "<body>\n"
        "<h3>$sw3-type$: $sw3-name$</h3>\n"
        "\nRaw file <a href=\"$sw3-name$\">here.</a>\n"
        "<hr>\n"
        "<pre>"
        "$sw3-code$"
        "</pre>\n"
        "<hr>\n"
        "<a href=\"../index.html\">Home</a><br>\n"
        "</body>\n"
        "</html>";


std::string linkify_operators(std::map<std::string, std::string> &operator_locations, const std::string &source_usage) {
    std::string usage = source_usage;  // Just copy it, since we want to modify it in place.
    for (const auto &iter: operator_locations) {
        std::string html_link = "<a href=\"../" + iter.second + "/" + iter.first + ".html\">" + iter.first + "</a>";

        std::string from = " " + iter.first + " ";  // Potentially add more of these later. Or do it in a more intelligent way?
        std::string to = " " + html_link + " ";
        string_replace_all(usage, from, to);

        from = " " + iter.first + "[";
        to = " " + html_link + "[";
        string_replace_all(usage, from, to);

        from = " " + iter.first + "(";
        to = " " + html_link + "(";
        string_replace_all(usage, from, to);

        from = "(" + iter.first + " ";
        to = "(" + html_link + " ";
        string_replace_all(usage, from, to);

        from = " " + iter.first + ",";
        to = " " + html_link + ",";
        string_replace_all(usage, from, to);

        from = " " + iter.first + "\n";  // We need usage + "\n" for this to work consistently.
        to = " " + html_link + "\n";
        string_replace_all(usage, from, to);

        from = " " + iter.first + "^";
        to = " " + html_link + "^";
        string_replace_all(usage, from, to);
    }
    return usage;
}

template <class T>
std::string generate_operator_usage_docs(std::map<std::string, std::string> &operator_locations, const std::string &header, const std::string &dest_dir, const std::string &dir, T& our_map) {
    std::string section;

    // Create destination directory:
    std::string working_dir = dest_dir + dir;
    try {
        std::cout << "Creating: " << working_dir << std::endl;
        fs::create_directories(working_dir);
    } catch (std::exception& e) {
        std::cout << "Failed to create: " << working_dir << std::endl;
        std::cout << "Reason: " << e.what() << std::endl;
        return section;
    }

    section = "<dl>\n    <dt><b>" + header + "</b></dt>\n";

    // Now learn and sort the corresponding operators:
    std::vector<std::string> tmp_sorted;
    for (const auto &it: our_map) {
        tmp_sorted.push_back(ket_map.get_str(it.first));
    }
    std::sort(tmp_sorted.begin(), tmp_sorted.end());
    for (const auto &str: tmp_sorted) {
        if (operator_usage_map.usage_is_defined(str)) {
            std::string operator_file = dir + "/" + str + ".html";
            section += "        <dd><a href=\"" + operator_file + "\">" + str + "</a></dd>\n";

            std::string usage = operator_usage_map.get_usage(str) + "\n";

            // Write operator usage info to file:
            std::ofstream myfile;
            myfile.open(dest_dir + operator_file);
            if (myfile.is_open()) {
                std::string html_usage = docs_operator_page;
                std::string linked_operator_usage = linkify_operators(operator_locations, usage);
                string_replace_all(html_usage, "$operator-type$", header);
                string_replace_all(html_usage, "$operator-name$", str);
                string_replace_all(html_usage, "$operator-usage$", linked_operator_usage);

                myfile << html_usage;
                myfile.close();
            } else {
                std::cout << "Unable to open file: " << dest_dir + operator_file << std::endl;
            }
        } else {
            section += "        <dd>" + str + "</dd>\n";
        }
    }
    return section + "</dl>\n";
}

std::string generate_sw_section(std::map<std::string, std::string> &operator_locations, const std::string &header, const std::string &dest_dir, const std::string &dir, std::vector<fs::path> &sw3_files) {
    std::string section;

    // Create destination directory:
    std::string working_dir = dest_dir + dir;
    try {
        std::cout << "Creating: " << working_dir << std::endl;
        fs::create_directories(working_dir);
    } catch (std::exception& e) {
        std::cout << "Failed to create: " << working_dir << std::endl;
        std::cout << "Reason: " << e.what() << std::endl;
        return section;
    }

    section = "<dl>\n    <dt><b>" + header + "</b></dt>\n";

    std::vector<std::string> tmp_sorted;
    std::map<std::string, fs::path> sw3_path_map;
    for (const auto &file: sw3_files) {
        std::string my_filename = file.filename().c_str();
        tmp_sorted.push_back(my_filename);
        sw3_path_map[my_filename] = file;
    }
    std::sort(tmp_sorted.begin(), tmp_sorted.end());
    for (const auto &str: tmp_sorted) {
        section += "        <dd><a href=\"" + dir + "/" + str + ".html\">" + str + "</a></dd>\n";
        fs::path source_file = sw3_path_map[str];
        fs::path target_file = working_dir + "/" + str;
        fs::copy(source_file, target_file, fs::copy_options::overwrite_existing);

        std::string code;
        std::ifstream source_stream;
        source_stream.open(source_file);
        if (source_stream.is_open()) {
            std::stringstream buffer;
            buffer << source_stream.rdbuf();
            code = buffer.str();
            source_stream.close();
        } else {
            std::cout << "Unable to open file: " << source_file << std::endl;
        }

        // Now linkify the code:
        std::string linked_code = linkify_operators(operator_locations, code);

        // Now sub in the variables:
        std::string html_usage = docs_sw3_page;
        string_replace_all(html_usage, "$sw3-type$", header);
        string_replace_all(html_usage, "$sw3-name$", str);
        string_replace_all(html_usage, "$sw3-code$", linked_code);

        // Write operator usage info to file:
        std::string dot_html = ".html";
        std::ofstream myfile;
        myfile.open(target_file.c_str() + dot_html);
        if (myfile.is_open()) {
            myfile << html_usage;
            myfile.close();
        } else {
            std::cout << "Unable to open file: " << target_file.c_str() + dot_html << std::endl;
        }
    }
    return section + "</dl>\n";
}

template <class T>
void learn_locations(std::map<std::string, std::string> &operator_locations, const std::string &location, T& our_map) {
    for (const auto &iter: our_map) {
        std::string op = ket_map.get_str(iter.first);
        if (operator_usage_map.usage_is_defined(op)) {  // Only store locations of operators with usage info. This prevents links to 404 pages.
            operator_locations[op] = location;
        }
    }
}

void DocsGenerator::generate(const std::string& dir) {
    std::cout << "Generating docs in: " << dir << std::endl;

    // Find .sw3 files to include in the html documentation:
    std::cout << "Would you like to include .sw3 files? (y/n): ";
    std::vector<fs::path> sw3_files;
    std::map<std::string, std::string> operator_locations;
    char answer;
    std::cin >> answer;
    if (answer == 'y') {
        std::string sw3_dir;
        std::cout << "Please enter the sw3 directory: ";
        std::cin >> sw3_dir;

        std::cout << "Found:" << std::endl;
        for (const auto &entry: fs::directory_iterator(sw3_dir)) {
            if (entry.path().extension() == ".sw3") {
                std::cout << "    " << entry.path() << std::endl;
                sw3_files.push_back(entry.path());
                operator_locations[entry.path().filename().c_str()] = "sw-examples";
            }
        }
        std::cout << std::endl;
    }

    std::string dest_dir = dir + "/docs/usage3/";

    // Create destination directory:
    try {
        std::cout << "Creating: " << dest_dir << std::endl;
        fs::create_directories(dest_dir);
    } catch (std::exception& e) {
        std::cout << "Failed to create: " << dest_dir << std::endl;
        std::cout << "Reason: " << e.what() << std::endl;
        return;
    }

    // Generate html page:
    std::string body;

    // Learn operator locations:
    // NB: the locations must match those used in the next section.
    // std::map<std::string, std::string> operator_locations;
    learn_locations(operator_locations, "built-in", fn_map.built_in);
    learn_locations(operator_locations, "compound-built-in", fn_map.compound_built_in);
    learn_locations(operator_locations, "compound-context-built-in", fn_map.compound_context_built_in);
    learn_locations(operator_locations, "sigmoid", fn_map.sigmoids);
    learn_locations(operator_locations, "compound-sigmoid", fn_map.compound_sigmoids);
    learn_locations(operator_locations, "ket-fn", fn_map.ket_fn);
    learn_locations(operator_locations, "context-ket-fn", fn_map.context_ket_fn);
    learn_locations(operator_locations, "compound-ket-fn", fn_map.compound_ket_fn);
    learn_locations(operator_locations, "sp-fn", fn_map.sp_fn);
    learn_locations(operator_locations, "compound-sp-fn", fn_map.compound_sp_fn);
    learn_locations(operator_locations, "seq-fn", fn_map.seq_fn);
    learn_locations(operator_locations, "context-seq-fn", fn_map.context_seq_fn);
    learn_locations(operator_locations, "compound-seq-fn", fn_map.compound_seq_fn);
    learn_locations(operator_locations, "compound-context-sp-fn", fn_map.compound_context_sp_fn);
    learn_locations(operator_locations, "compound-context-seq-fn", fn_map.compound_context_seq_fn);
    learn_locations(operator_locations, "function-1", fn_map.whitelist_1);
    learn_locations(operator_locations, "function-2", fn_map.whitelist_2);
    learn_locations(operator_locations, "function-3", fn_map.whitelist_3);
    learn_locations(operator_locations, "function-4", fn_map.whitelist_4);
    learn_locations(operator_locations, "context-function-1", fn_map.context_whitelist_1);
    learn_locations(operator_locations, "context-function-2", fn_map.context_whitelist_2);
    learn_locations(operator_locations, "context-function-3", fn_map.context_whitelist_3);
    learn_locations(operator_locations, "context-function-4", fn_map.context_whitelist_4);

    // Generate sections:
    body += generate_operator_usage_docs(operator_locations, "built in operators", dest_dir, "built-in", fn_map.built_in);
    body += generate_operator_usage_docs(operator_locations, "built in compound operators", dest_dir, "compound-built-in", fn_map.compound_built_in);
    body += generate_operator_usage_docs(operator_locations, "built in compound context operators", dest_dir, "compound-context-built-in", fn_map.compound_context_built_in);
    body += generate_operator_usage_docs(operator_locations, "sigmoids", dest_dir, "sigmoid", fn_map.sigmoids);
    body += generate_operator_usage_docs(operator_locations, "compound sigmoids", dest_dir, "compound-sigmoid", fn_map.compound_sigmoids);
    body += generate_operator_usage_docs(operator_locations, "ket fn", dest_dir, "ket-fn", fn_map.ket_fn);
    body += generate_operator_usage_docs(operator_locations, "context ket fn", dest_dir, "context-ket-fn", fn_map.context_ket_fn);
    body += generate_operator_usage_docs(operator_locations, "compound ket fn", dest_dir, "compound-ket-fn", fn_map.compound_ket_fn);
    body += generate_operator_usage_docs(operator_locations, "sp fn", dest_dir, "sp-fn", fn_map.sp_fn);
    body += generate_operator_usage_docs(operator_locations, "compound sp fn", dest_dir, "compound-sp-fn", fn_map.compound_sp_fn);
    body += generate_operator_usage_docs(operator_locations, "seq fn", dest_dir, "seq-fn", fn_map.seq_fn);
    body += generate_operator_usage_docs(operator_locations, "context seq fn", dest_dir, "context-seq-fn", fn_map.context_seq_fn);
    body += generate_operator_usage_docs(operator_locations, "compound seq fn", dest_dir, "compound-seq-fn", fn_map.compound_seq_fn);
    body += generate_operator_usage_docs(operator_locations, "compound context sp fn", dest_dir, "compound-context-sp-fn", fn_map.compound_context_sp_fn);
    body += generate_operator_usage_docs(operator_locations, "compound context seq fn", dest_dir, "compound-context-seq-fn", fn_map.compound_context_seq_fn);
    body += generate_operator_usage_docs(operator_locations, "function 1", dest_dir, "function-1", fn_map.whitelist_1);
    body += generate_operator_usage_docs(operator_locations, "function 2", dest_dir, "function-2", fn_map.whitelist_2);
    body += generate_operator_usage_docs(operator_locations, "function 3", dest_dir, "function-3", fn_map.whitelist_3);
    body += generate_operator_usage_docs(operator_locations, "function 4", dest_dir, "function-4", fn_map.whitelist_4);
    body += generate_operator_usage_docs(operator_locations, "context function 1", dest_dir, "context-function-1", fn_map.context_whitelist_1);
    body += generate_operator_usage_docs(operator_locations, "context function 2", dest_dir, "context-function-2", fn_map.context_whitelist_2);
    body += generate_operator_usage_docs(operator_locations, "context function 3", dest_dir, "context-function-3", fn_map.context_whitelist_3);
    body += generate_operator_usage_docs(operator_locations, "context function 4", dest_dir, "context-function-4", fn_map.context_whitelist_4);


    // Generate sw-examples section:
    if (!sw3_files.empty()) {
        body += generate_sw_section(operator_locations, "sw-examples", dest_dir, "sw-examples", sw3_files);
    }

    // Load up the index page, and insert the body:
    std::string page = docs_index_page;
    string_replace_all(page, "$body$", body);

    // Generate current date:
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%B %d, %G", timeinfo);

    // Now sub the date-time in:
    string_replace_all(page, "$date-time$", buffer);

    // Write index page to stdout:
    std::cout << "\nNew html index page:\n" << page << std::endl;


    // Write index page to file:
    std::ofstream myfile;
    myfile.open(dest_dir + "index.html");
    if (myfile.is_open()) {
        myfile << page;
        myfile.close();
    } else {
        std::cout << "Unable to open file: " << dest_dir + "index.html" << std::endl;
    }
}
