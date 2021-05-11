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

const std::string docs_header =
        "<html>\n"
        "<head><title>Semantic DB 3.1 usage information</title></head>\n"
        "<body>\n"
        "<h3>Semantic DB 3.1 usage information</h3>\n"
        "Welcome to the Semantic DB 3.1 usage page. Below are brief descriptions and examples for our operators and sequence functions.\n"
        "The corresponding github repo is <a href=\"https://github.com/GarryMorrison/SemanticDB3.1\">available here</a>.\n";

const std::string docs_footer =
        "<hr>\n"
        "by Garry Morrison<br>\n"
        "email: garry -at- semantic-db.org<br>\n"
        "updated: $datetime$\n"
        "\n</body>\n</html>\n";


std::string convert_usage_to_html(std::map<std::string, std::string> &operator_locations, const std::string &header, const std::string &operator_str) {  // We factored this out, to keep things tidy!
    std::string html_usage;
    html_usage = "<html>\n<head><title>" + header + operator_str + "</title></head>\n<body>\n";
    html_usage += "<h3>" + header + operator_str + "</h3>\n<pre>";
    std::string usage = operator_usage_map.get_usage(operator_str) + "\n";
    std::string sw3_ext = ".sw3";  // Not ideal that we hardwire in the extension here.
    std::string html_link;
    for (const auto &iter: operator_locations) {
        if (std::equal(sw3_ext.rbegin(), sw3_ext.rend(), iter.first.rbegin())) {  // If the iter.first ends with .sw3 then it is an sw3 file, not an operator.
            html_link = "<a href=\"../" + iter.second + "/" + iter.first + "\">" + iter.first + "</a>";
        } else {
            html_link = "<a href=\"../" + iter.second + "/" + iter.first + ".html\">" + iter.first + "</a>";
        }

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
    html_usage += usage;
    html_usage += "\n</pre>\n<hr>\n<a href=\"../index.html\">Home</a><br>\n</body>\n</html>\n";
    return html_usage;
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

            // Write operator usage info to file:
            std::ofstream myfile;
            myfile.open(dest_dir + operator_file);
            if (myfile.is_open()) {
                myfile << convert_usage_to_html(operator_locations, header, str);
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

std::string generate_sw_section(const std::string &header, const std::string &dest_dir, const std::string &dir, std::vector<fs::path> &sw3_files) {
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
        section += "        <dd><a href=\"" + dir + "/" + str + "\">" + str + "</a></dd>\n";
        fs::path source_file = sw3_path_map[str];
        fs::path target_file = working_dir + "/" + str;
        fs::copy(source_file, target_file, fs::copy_options::overwrite_existing);
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
    std::string page = docs_header;

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
    page += generate_operator_usage_docs(operator_locations, "built in operators: ", dest_dir, "built-in", fn_map.built_in);
    page += generate_operator_usage_docs(operator_locations, "built in compound operators: ", dest_dir, "compound-built-in", fn_map.compound_built_in);
    page += generate_operator_usage_docs(operator_locations, "built in compound context operators: ", dest_dir, "compound-context-built-in", fn_map.compound_context_built_in);
    page += generate_operator_usage_docs(operator_locations, "sigmoids: ", dest_dir, "sigmoid", fn_map.sigmoids);
    page += generate_operator_usage_docs(operator_locations, "compound sigmoids: ", dest_dir, "compound-sigmoid", fn_map.compound_sigmoids);
    page += generate_operator_usage_docs(operator_locations, "ket fn: ", dest_dir, "ket-fn", fn_map.ket_fn);
    page += generate_operator_usage_docs(operator_locations, "context ket fn: ", dest_dir, "context-ket-fn", fn_map.context_ket_fn);
    page += generate_operator_usage_docs(operator_locations, "compound ket fn: ", dest_dir, "compound-ket-fn", fn_map.compound_ket_fn);
    page += generate_operator_usage_docs(operator_locations, "sp fn: ", dest_dir, "sp-fn", fn_map.sp_fn);
    page += generate_operator_usage_docs(operator_locations, "compound sp fn: ", dest_dir, "compound-sp-fn", fn_map.compound_sp_fn);
    page += generate_operator_usage_docs(operator_locations, "seq fn: ", dest_dir, "seq-fn", fn_map.seq_fn);
    page += generate_operator_usage_docs(operator_locations, "context seq fn: ", dest_dir, "context-seq-fn", fn_map.context_seq_fn);
    page += generate_operator_usage_docs(operator_locations, "compound seq fn: ", dest_dir, "compound-seq-fn", fn_map.compound_seq_fn);
    page += generate_operator_usage_docs(operator_locations, "compound context sp fn: ", dest_dir, "compound-context-sp-fn", fn_map.compound_context_sp_fn);
    page += generate_operator_usage_docs(operator_locations, "compound context seq fn: ", dest_dir, "compound-context-seq-fn", fn_map.compound_context_seq_fn);
    page += generate_operator_usage_docs(operator_locations, "function 1: ", dest_dir, "function-1", fn_map.whitelist_1);
    page += generate_operator_usage_docs(operator_locations, "function 2: ", dest_dir, "function-2", fn_map.whitelist_2);
    page += generate_operator_usage_docs(operator_locations, "function 3: ", dest_dir, "function-3", fn_map.whitelist_3);
    page += generate_operator_usage_docs(operator_locations, "function 4: ", dest_dir, "function-4", fn_map.whitelist_4);
    page += generate_operator_usage_docs(operator_locations, "context function 1: ", dest_dir, "context-function-1", fn_map.context_whitelist_1);
    page += generate_operator_usage_docs(operator_locations, "context function 2: ", dest_dir, "context-function-2", fn_map.context_whitelist_2);
    page += generate_operator_usage_docs(operator_locations, "context function 3: ", dest_dir, "context-function-3", fn_map.context_whitelist_3);
    page += generate_operator_usage_docs(operator_locations, "context function 4: ", dest_dir, "context-function-4", fn_map.context_whitelist_4);


    // Generate sw-examples section:
    if (!sw3_files.empty()) {
        page += generate_sw_section("sw-examples: ", dest_dir, "sw-examples", sw3_files);
    }

    page += docs_footer;

    // Generate current date:
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%B %d, %G", timeinfo);

    // Now sub it in:
    string_replace_all(page, "$datetime$", buffer);

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
