//
// Created by Garry Morrison on 5/10/2020.
//
// Sourced from here:
// https://github.com/bingmann/flex-bison-cpp-example
// Also inspired by code here:
// https://github.com/jonathan-beard/simple_wc_example


#ifndef SDB3_1_DRIVER_H
#define SDB3_1_DRIVER_H

#include <string>
#include "src/SDB3_header.h"
class ContextList;

namespace SDB {
    class Driver {
    public:
        Driver(class ContextList &context);

        /// enable debug output in the flex scanner
        bool trace_scanning;

        /// enable debug output in the bison parser
        bool trace_parsing;

        /// stream name (file or input stream) used for error messages.
        std::string streamname;

        /** Invoke the scanner and parser for a stream.
     * @param in	input stream
     * @param sname	stream name for error messages
     * @return		true if successfully parsed
     */
        bool parse_stream(std::istream& in,
                          const std::string& sname = "stream input");

        /** Invoke the scanner and parser on an input string.
         * @param input	input string
         * @param sname	stream name for error messages
         * @return		true if successfully parsed
         */
        bool parse_string(const std::string& input,
                          const std::string& sname = "string stream");

        /** Invoke the scanner and parser on a file. Use parse_stream with a
         * std::ifstream if detection of file reading errors is required.
         * @param filename	input file name
         * @return		true if successfully parsed
         */
        bool parse_file(const std::string& filename);

    /** Error handling with associated line number. This can be modified to
     * output the error e.g. to a dialog box. */
        void error(const class location& l, const std::string& m);

        /** General error handling. This can be modified to output the error
         * e.g. to a dialog box. */
        void error(const std::string& m);

        /** Pointer to the current lexer instance, this is used to connect the
         * parser to the scanner. It is used in the yylex macro. */
        class Scanner* lexer;

        /** Reference to the calculator context filled during parsing of the
         * expressions. */
        class ContextList& context;
    };
}

#endif //SDB3_1_DRIVER_H
