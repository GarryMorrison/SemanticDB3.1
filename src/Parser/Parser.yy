%{ /*** C/C++ Declarations ***/

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>

#include "src/SDB3_header.h"
#include "src/Parser/Driver.h"

%}

/*** yacc/bison Declarations ***/

/* Require bison 3.3 or later */
%require "3.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
// %debug

/* start symbol is named "start" */
%start start

/* write out a header file containing the token defines */
%defines

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%define api.namespace {SDB}

/* set the parser's class identifier */
// For Bison earlier than 3.3:
// %define "parser_class_name" {Parser}
// For Bison 3.3 and later:
%define api.parser.class {Parser}


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
// Switch off for release version:
// %define parse.error verbose


%union {
    int  			integerVal;
    ulong           ulongVal;
    double 			doubleVal;
    std::string*		stringVal;
    class BaseSequence* bSeq;
    class Ket* ketVal;
    class Superposition* spVal;
    class Sequence* seqVal;
    class OperatorSequence* opSeqVal;
    class BaseOperator* baseOpVal;
    class OperatorWithSequence* opWithSeqVal;
    class LearnRule* learnRuleVal;
    class CompoundConstant* constVal;
    std::vector<std::shared_ptr<CompoundConstant> >* constVec;
    class CompoundOperator* compOpVal;
    class BracketOperator* bracketOpVal;
    class MultiLearnRule* multiLearnRuleVal;
    class IfElseStatement* ifElseStatementVal;
    class ForStatement* forStatementVal;
}

%token			END	     0	"end of file"
%token			EOL		"end of line"
%token          SPACE   "white space char"
%token          COMMENT "comment string"
%token          CONTEXT "context learn rule"
%token          SELF_KET "self ket"
%token          MULTI_SELF_KET "multi self ket"
%token <integerVal>     SELF_KETK   "self ket k"
%token <integerVal>     MULTI_SELF_KETK   "multi self ket k"
%token <integerVal> 	INTEGER		"integer"
%token <doubleVal> 	DOUBLE		"double"
%token <stringVal> 	STRING		"string"
%token <ulongVal>   KET_LABEL   "ket label idx"
%token <ulongVal>   OP_LABEL    "operator label idx"
%token <ulongVal>   OP_LABEL_SPACE    "operator label idx with space"
%token <ulongVal>   FN_LPAREN   "function label with left parenthesis"
%token <integerVal> LEARN_SYM   "learn symbol"
// %token          ADD_LEARN_SYM   "add learn symbol"
// %token          SEQ_LEARN_SYM   "sequence learn symbol"
// %token          STORE_LEARN_SYM   "store learn symbol"
// %token          MEM_LEARN_SYM   "memoize learn symbol"
// %token          LEARN_SYM       "learn symbol"
%token <integerVal> FN_SYM      "function rule symbol"
%token <integerVal> INFIX_OP    "infix operator"
%token <integerVal> MINUS_OP    "minus operator"
// %token          PLUS_OP         "infix plus operator"
// %token          MINUS_OP         "infix minus operator"
// %token          SEQ_OP         "infix sequence operator"
// %token          MERGE2_OP         "infix merge2 operator"
// %token          MERGE_OP         "infix merge operator"
%token          LPAREN          "left parenthesis"
%token          RPAREN          "right parenthesis"
%token          LSQUARE          "left square parenthesis"
%token          RSQUARE          "right square parenthesis"
%token          COMMA           "comma"
%token          POWER           "power"
%token          QUOTE           "quotation symbol"
%token          STAR            "*"
%token          DIVIDE          "divide symbol"
%token          COLON           "colon symbol"
%token          EOL_SPACE4      "end of line with space 4"
%token <integerVal>  INFIX_DOUBLE_OP "infix double operator"
%token          OPEN_IF         "open if statement"
%token          OPEN_ELSE       "open else statement"
%token          CLOSE           "close statement"
%token <integerVal>  EOL_INDENT "end of line followed by indent"
%token <integerVal>  EOL_SAME "end of line followed by same depth"
%token <integerVal>  EOL_UNDENT "end of line followed by undent"
%token <integerVal>  OPEN_FOR   "open for statement"
%token               INFIX_IN   "infix in"
%token               DOLLAR "dollar symbol"


// %type <ketVal> ket
// %type <spVal> superposition
// %type <seqVal> sequence
%type <bSeq> ket
%type <opSeqVal> operator_sequence
%type <baseOpVal> operator compound_operator function_operator general_operator infix_operator
%type <opWithSeqVal> operator_with_sequence general_sequence operator_or_general_sequence
%type <learnRuleVal> learn_rule general_learn_rule
%type <constVal> constant
%type <constVec> parameters
// %type <compOpVal> compound_operator
%type <bracketOpVal> bracket_operator bracket_parameters
%type <multiLearnRuleVal> multi_learn_rule
%type <ifElseStatementVal> if_else_statement
%type <forStatementVal> for_statement


%destructor { delete $$; } STRING
%destructor { delete $$; } ket
// %destructor { delete $$; } superposition
// %destructor { delete $$; } sequence



%{

#include <memory>
#include "src/Parser/Driver.h"
#include "src/Parser/Scanner.h"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex


std::string operator_sequence_string(std::vector<std::string> vec) {
    std::string s;
    for (const auto &s2: vec) {
        s += s2 + ", ";
    }
    return s;
}

%}

%% /*** Grammar Rules ***/

start : END
      | line END;

line : item
     | line item
     ;

//item  : sequence { std::cout << "sequence: " << $1->to_string() << std::endl; }
//      | learn_rule
//      | operator_with_sequence

item : operator_or_general_sequence EOL { std::cout << "\n" << $1->Compile(driver.context).to_string() << "\n"; }
//      | general_sequence EOL { std::cout << "general sequence:\n" << $1->Compile(driver.context).to_string() << std::endl; }
//      | operator_sequence EOL { Sequence seq(""); std::cout << "operator sequence:\n" << $1->Compile(driver.context, seq).to_string() << std::endl; }
     | learn_rule EOL { /* std::cout << "learn rule: " << $1->to_string() << std::endl; */ $1->Compile(driver.context); }
     | general_learn_rule EOL { /* std::cout << "multi learn rule:\n" << $1->to_string() << std::endl; */ $1->Compile(driver.context); }
     | function_learn_rule EOL
     | COMMENT EOL
     | CONTEXT KET_LABEL EOL { driver.context.set(ket_map.get_str($2)); }
     | DOLLAR EOL_INDENT multi_learn_rule EOL { std::cout << "\n" << $3->Compile(driver.context).to_string() << "\n"; }
     | EOL
//     | compound_operator { std::cout << "compound operator: " << $1->to_string() << std::endl; }
//     | bracket_operator
     ;


ket   : KET_LABEL { $$ = new Ket($1); }
      | SELF_KET { $$ = new SelfKet(); }
      | MULTI_SELF_KET { $$ = new MultiSelfKet(); }
      | MULTI_SELF_KETK { $$ = new MultiSelfKet($1); }
//      | INTEGER KET_LABEL { $$ = new Ket($2, $1); }
//      | DOUBLE KET_LABEL { $$ = new Ket($2, $1); }
      ;

/*
superposition : ket { $$ = new Superposition(*$1); }
              | superposition PLUS_OP ket { $1->add(*$3); $$ = $1; }
              | superposition MINUS_OP ket { $3->multiply(-1); $1->add(*$3); $$ = $1; }
              ;

sequence : superposition { $$ = new Sequence(*$1); }
         | sequence SEQ_OP superposition { $1->append(*$3); $$ = $1; }
         | sequence MERGE2_OP superposition { $1->merge(*$3, " "); $$ = $1; }
         | sequence MERGE_OP superposition { $1->merge(*$3); $$ = $1; }
//         | LPAREN sequence RPAREN { $$ = $2; }
         ;


learn_rule : OP_LABEL KET_LABEL LEARN_SYM general_sequence { driver.context.learn($1, $2, std::make_shared<Sequence>(*$4)); }
           | OP_LABEL KET_LABEL ADD_LEARN_SYM general_sequence { driver.context.add_learn($1, $2, std::make_shared<Sequence>(*$4)); }
           | OP_LABEL KET_LABEL SEQ_LEARN_SYM general_sequence { driver.context.seq_learn($1, $2, std::make_shared<Sequence>(*$4)); }
           | OP_LABEL KET_LABEL STORE_LEARN_SYM general_sequence { driver.context.stored_learn($1, $2, std::make_shared<Sequence>(*$4)); }
           | OP_LABEL KET_LABEL MEM_LEARN_SYM general_sequence { driver.context.memoize_learn($1, $2, std::make_shared<Sequence>(*$4)); }
           ;
*/

learn_rule : operator_with_sequence LEARN_SYM operator_or_general_sequence { $$ = new LearnRule(*$1, $2, *$3); }
           ;

general_learn_rule : operator_with_sequence LEARN_SYM EOL_INDENT multi_learn_rule { $$ = new LearnRule(*$1, $2, *$4); }
                   ;

if_else_statement : OPEN_IF operator_or_general_sequence RPAREN COLON EOL_INDENT multi_learn_rule EOL_UNDENT CLOSE {
                       MultiLearnRule *empty_rule = new MultiLearnRule();  // Not sure about this at all! Memory leak? Crashes?
                       $$ = new IfElseStatement(*$2, *$6, *empty_rule);
                   }
                   | OPEN_IF operator_or_general_sequence RPAREN COLON EOL_INDENT multi_learn_rule EOL_UNDENT OPEN_ELSE EOL_INDENT multi_learn_rule EOL_UNDENT CLOSE {
                       $$ = new IfElseStatement(*$2, *$6, *$10);
                   }
                   ;

for_statement : OPEN_FOR OP_LABEL KET_LABEL INFIX_IN operator_or_general_sequence RPAREN COLON EOL_INDENT multi_learn_rule EOL_UNDENT CLOSE {
                  $$ = new ForStatement($1, $2, $3, *$5, *$9);
              }
              ;

multi_learn_rule : learn_rule { $$ = new MultiLearnRule(*$1);  /* std::cout << "indent: " << $1 << "\n"; */ }
                 | operator_or_general_sequence { $$ = new MultiLearnRule(*$1); }
                 | if_else_statement { $$ = new MultiLearnRule(*$1); }
                 | for_statement { $$ = new MultiLearnRule(*$1); }
                 | multi_learn_rule EOL_SAME learn_rule { $$->append(*$3); }
                 | multi_learn_rule EOL_SAME operator_or_general_sequence { $$->append(*$3); }
                 | multi_learn_rule EOL_SAME COMMENT { }
                 | multi_learn_rule EOL_SAME if_else_statement { $$->append(*$3); }
                 | multi_learn_rule EOL_SAME for_statement { $$->append(*$3); }
                 ;

function_learn_rule : OP_LABEL FN_SYM LEARN_SYM operator_or_general_sequence { std::shared_ptr<BaseSequence> tmp_ptr($4); driver.context.fn_learn($1, $2, tmp_ptr); }
                    | OP_LABEL FN_SYM LEARN_SYM EOL_INDENT multi_learn_rule { std::shared_ptr<BaseSequence> tmp_ptr($5); driver.context.fn_learn($1, $2, tmp_ptr); }
                    ;

operator_with_sequence : ket {
                            // std::cout << "naked ket: " << $1->to_string() << std::endl;
                            std::shared_ptr<BaseOperator> tmp_op_ptr = std::make_shared<SimpleOperator>("");
                            std::shared_ptr<BaseSequence> tmp_seq_ptr($1);
                            $$ = new OperatorWithSequence(tmp_op_ptr, tmp_seq_ptr);
                       }
                       | operator_sequence ket {
                            // std::cout << $1->to_string() << $2->to_string() << std::endl;
                            std::shared_ptr<BaseOperator> tmp_op_ptr($1);
                            std::shared_ptr<BaseSequence> tmp_seq_ptr($2);
                            $$ = new OperatorWithSequence(tmp_op_ptr, tmp_seq_ptr);
                       }
                       | operator_sequence LPAREN general_sequence RPAREN {
                            std::shared_ptr<BaseOperator> tmp_op_ptr($1);
                            std::shared_ptr<BaseSequence> tmp_seq_ptr($3);
                            $$ = new OperatorWithSequence(tmp_op_ptr, tmp_seq_ptr);
                       }
                       ;

operator_sequence : general_operator { std::shared_ptr<BaseOperator> tmp_ptr($1); $$ = new OperatorSequence(tmp_ptr); }
                  | operator_sequence general_operator { $$ = $1; std::shared_ptr<BaseOperator> tmp_ptr($2); $$->append(tmp_ptr); }
                  ;

/*
operator : OP_LABEL LSQUARE parameters RSQUARE { $$ = new CompoundOperator($1, *$3); }
         | OP_LABEL LPAREN general_sequence RPAREN { $$ = new FunctionOperator($1, *$3); }
         | OP_LABEL LPAREN general_sequence COMMA general_sequence RPAREN { $$ = new FunctionOperator($1, *$3, *$5); }
         | OP_LABEL LPAREN general_sequence COMMA general_sequence COMMA general_sequence RPAREN { $$ = new FunctionOperator($1, *$3, *$5, *$7); }
         | OP_LABEL LPAREN general_sequence COMMA general_sequence COMMA general_sequence COMMA general_sequence RPAREN { $$ = new FunctionOperator($1, *$3, *$5, *$7, *$9); }
         | constant {
                switch ($1->type()) {
                    case COPERATOR : { $$ = new SimpleOperator($1->get_operator()); break; }
                    case CINT : { $$ = new NumericOperator($1->get_int()); break; }
                    case CFLOAT : { $$ = new NumericOperator($1->get_float()); break; }
                    default : { $$ = new EmptyOperator(); }
                }
         }
         ;
*/

compound_operator : OP_LABEL LSQUARE parameters RSQUARE { $$ = new CompoundOperator($1, *$3); }
                  ;

/*
function_operator : OP_LABEL LPAREN general_sequence RPAREN { $$ = new FunctionOperator($1, *$3); }
                  | OP_LABEL LPAREN general_sequence COMMA general_sequence RPAREN { $$ = new FunctionOperator($1, *$3, *$5); }
                  | OP_LABEL LPAREN general_sequence COMMA general_sequence COMMA general_sequence RPAREN { $$ = new FunctionOperator($1, *$3, *$5, *$7); }
                  | OP_LABEL LPAREN general_sequence COMMA general_sequence COMMA general_sequence COMMA general_sequence RPAREN { $$ = new FunctionOperator($1, *$3, *$5, *$7, *$9); }
                  ;
*/

function_operator : FN_LPAREN operator_or_general_sequence RPAREN { $$ = new FunctionOperator($1, *$2); }
                  | FN_LPAREN operator_or_general_sequence COMMA operator_or_general_sequence RPAREN { $$ = new FunctionOperator($1, *$2, *$4); }
                  | FN_LPAREN operator_or_general_sequence COMMA operator_or_general_sequence COMMA operator_or_general_sequence RPAREN { $$ = new FunctionOperator($1, *$2, *$4, *$6); }
                  | FN_LPAREN operator_or_general_sequence COMMA operator_or_general_sequence COMMA operator_or_general_sequence COMMA operator_or_general_sequence RPAREN { $$ = new FunctionOperator($1, *$2, *$4, *$6, *$8); }
                  ;

infix_operator : LPAREN operator_or_general_sequence INFIX_DOUBLE_OP operator_or_general_sequence RPAREN { $$ = new InfixOperator(*$2, $3, *$4); }
               ;

operator : function_operator { $$ = $1; }
         | compound_operator { $$ = $1; }
         | bracket_operator { $$ = $1; }
         | infix_operator { $$ = $1; }
         | constant {
                switch ($1->type()) {
                    case COPERATOR : { $$ = new SimpleOperator($1->get_operator()); break; }
                    case CINT : { $$ = new NumericOperator($1->get_int()); break; }
                    case CFLOAT : { $$ = new NumericOperator($1->get_float()); break; }
                    default : { $$ = new EmptyOperator(); }
                }
         }
         ;

general_operator : operator { $$ = $1; }
                 | operator POWER INTEGER { std::shared_ptr<BaseOperator> tmp_ptr($1); $$ = new PoweredOperator(tmp_ptr, $3); }
                 ;

general_sequence : operator_with_sequence { $$ = $1; }
                 | general_sequence INFIX_OP operator_with_sequence { $$ = $1; $$->append($2, *$3); }
                 | general_sequence operator_with_sequence { $$ = $1; $$->append(SPLUS, *$2); }
                 | LPAREN general_sequence RPAREN { $$ = $2; }
                 ;

operator_or_general_sequence : general_sequence { $$ = $1; }
                             | operator_sequence {
                                 std::shared_ptr<BaseOperator> tmp_op_ptr($1);
                                 std::shared_ptr<BaseSequence> tmp_seq_ptr = std::make_shared<Ket>();
                                 $$ = new OperatorWithSequence(tmp_op_ptr, tmp_seq_ptr);
                             }
                             ;

constant : STRING { $$ = new ConstantString(*$1); }
         | STAR { $$ = new ConstantOperator("*"); }
         | OP_LABEL { $$ = new ConstantOperator($1); }
         | INTEGER { $$ = new ConstantInteger($1); }
         | DOUBLE { $$ = new ConstantFloat($1); }
         | INTEGER DIVIDE INTEGER { $$ = new ConstantFloat((double)$1 / (double)$3); }
         | INTEGER DIVIDE DOUBLE { $$ = new ConstantFloat((double)$1 / $3); }
         | DOUBLE DIVIDE INTEGER { $$ = new ConstantFloat($1 / (double)$3); }
         | MINUS_OP { $$ = new ConstantFloat(-1); }
         ;

parameters : constant { $$ = new std::vector<std::shared_ptr<CompoundConstant>>;
                        std::shared_ptr<CompoundConstant> tmp_ptr($1);
                        $$->push_back(tmp_ptr);
                        }
           | parameters constant { $$ = $1;
                                   std::shared_ptr<CompoundConstant> tmp_ptr($2);
                                   $$->back()->append(tmp_ptr);
           }
           | parameters COMMA constant { $$ = $1; std::shared_ptr<CompoundConstant> tmp_ptr($3); $$->push_back(tmp_ptr); }
           ;

bracket_parameters : operator_sequence { $$ = new BracketOperator(*$1); }
                   | INFIX_OP operator_sequence { $$ = new BracketOperator($1, *$2); }
                   | bracket_parameters INFIX_OP operator_sequence { $$ = $1; $$->append($2, *$3); }
                   ;

bracket_operator : LPAREN bracket_parameters RPAREN { $$ = $2; }
                 ;

%% /*** Additional Code ***/

void SDB::Parser::error(const Parser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}

