%{ /*** C/C++ Declarations ***/

#include <string>
#include <memory>

#include "src/Parser/Scanner.h"
#include "src/SDB3_header.h"

/* import the parser's token type into a local typedef */
typedef SDB::Parser::token token;
typedef SDB::Parser::token_type token_type;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::END

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

ulong get_ket_idx(char *str, yy_size_t len) { // tidy later!!
    // std::string s(str, len);
    // return ket_map.get_idx(s.substr(1, s.size() - 2));
    if (len <= 2) { return ket_map.get_idx(""); } // later swap in the value of get_idx("").
    std::string s(++str, len - 2);
    return ket_map.get_idx(s);
}

ulong get_op_idx(char *str, yy_size_t len) {
    std::string s(str, len);
    return ket_map.get_idx(s);
}

%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "SDBFlexLexer" */
%option prefix="SDB"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput

/* enables the use of start condition stacks */
%option stack

/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

digit           [0-9]

%%


%{
// reset location
yylloc->step();
%}

"--".*"\n" { return token::COMMENT; }

"|_self>" { return token::SELF_KET; }

"|_self"{digit}">" { yylval->integerVal = yytext[6] - '0'; return token::SELF_KETK; }

"|"[^<|>]*">" { yylval->ulongVal = get_ket_idx(yytext, yyleng); return token::KET_LABEL; }

[a-zA-Z!][a-zA-Z0-9\-\+!?\.:]+ { yylval->ulongVal = get_op_idx(yytext, yyleng); return token::OP_LABEL; }

"\""[^\"\[\]<|>]*"\"" { yylval->stringVal = new std::string(yytext, yyleng); return token::STRING; }

"+=>" { yylval->integerVal = RULEADD; return token::LEARN_SYM; }
".=>" { yylval->integerVal = RULESEQ; return token::LEARN_SYM; }
"#=>" { yylval->integerVal = RULESTORED; return token::LEARN_SYM; }
"!=>" { yylval->integerVal = RULEMEMOIZE; return token::LEARN_SYM; }
"=>" { yylval->integerVal = RULENORMAL; return token::LEARN_SYM; }

"+" { yylval->integerVal = SPLUS; return token::INFIX_OP; }
"-" { yylval->integerVal = SMINUS; return token::INFIX_OP; }
"." { yylval->integerVal = SSEQ; return token::INFIX_OP; }
"__" { yylval->integerVal = SMERGE2; return token::INFIX_OP; }
"_" { yylval->integerVal = SMERGE; return token::INFIX_OP; }

"(" { return token::LPAREN; }
")" { return token::RPAREN; }
"[" { return token::LSQUARE; }
"]" { return token::RSQUARE; }

"," { return token::COMMA; }
"^" { return token::POWER; }
"\"" { return token::QUOTE; }
"\*" { return token::STAR; }

[0-9]+ {
        yylval->integerVal = atoi(yytext);
        return token::INTEGER;
    }

[0-9]+"."[0-9]* {
        yylval->doubleVal = atof(yytext);
        return token::DOUBLE;
    }

[A-Za-z][A-Za-z0-9_,.-]* {
        yylval->stringVal = new std::string(yytext, yyleng);
        return token::STRING;
    }

[ \t\r]+ {
        yylloc->step();
        //return token::SPACE;  // tends to cause parse errors!
    }

\n {
        yylloc->lines(yyleng); yylloc->step();
        return token::EOL;
    }


. {
return static_cast<token_type>(*yytext);
}


%% /*** Additional Code ***/

namespace SDB {

    Scanner::Scanner(std::istream* in,
                     std::ostream* out)
            : SDBFlexLexer(in, out)
    {
    }

    Scanner::~Scanner()
    {
    }

    void Scanner::set_debug(bool b)
    {
        yy_flex_debug = b;
    }

}

/* This implementation of ExampleFlexLexer::yylex() is required to fill the
 * vtable of the class ExampleFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int SDBFlexLexer::yylex()
{
    std::cerr << "We shouldn't be here, in SDBFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int SDBFlexLexer::yywrap()
{
    return 1;
}
