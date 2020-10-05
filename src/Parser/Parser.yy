%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>

#include "src/SDB3_header.h"
#include "src/Parser/Driver.h"

%}

/*** yacc/bison Declarations ***/

/* Require bison 3.0 or later */
%require "3.0"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
%start start

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%define api.namespace {SDB}

/* set the parser's class identifier */
%define "parser_class_name" {Parser}

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = &driver.streamname;
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. */
%parse-param { class Driver& driver }

/* verbose error messages */
%error-verbose

 /*** BEGIN EXAMPLE - Change the example grammar's tokens below ***/

%union {
    int  			integerVal;
    double 			doubleVal;
    std::string*		stringVal;
    class BaseSequence* bSeq;
}

%token			END	     0	"end of file"
%token			EOL		"end of line"
%token <integerVal> 	INTEGER		"integer"
%token <doubleVal> 	DOUBLE		"double"
%token <stringVal> 	STRING		"string"


%destructor { delete $$; } STRING


 /*** END EXAMPLE - Change the example grammar's tokens above ***/

%{

#include "src/Parser/Driver.h"
#include "src/Parser/Scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

%}

%% /*** Grammar Rules ***/

start : END | list END;

list : item
     | list item
     ;

item  : INTEGER EOL { std::cout << "INT: " << $1 << std::endl; }
      | DOUBLE EOL { std::cout << "DOUBLE: " << $1 << std::endl; }
      | STRING EOL { std::cout << "STRING: " << *$1 << std::endl; }
      ;

%% /*** Additional Code ***/

void SDB::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}
