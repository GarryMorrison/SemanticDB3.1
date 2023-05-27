cmake_minimum_required(VERSION 3.10)
project(SDB3_1)

set(CMAKE_CXX_STANDARD 17)

# FIND_PACKAGE(BISON 3.3 REQUIRED)
FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)

BISON_TARGET(Parser src/Parser/Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(Lexer src/Parser/Scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} src/tclap/)

file(GLOB sdb3_1_SRC CONFIGURE_DEPENDS "*.h" "*.cpp")
# add_executable(SDB3_1 ${sdb3_1_SRC})

file(GLOB KetMap_SRC CONFIGURE_DEPENDS "src/KetMap/*.h" "src/KetMap/*.cpp")
file(GLOB Function_SRC CONFIGURE_DEPENDS "src/Function/*.h" "src/Function/*.cpp")


# ${KetMap_SRC}

add_executable(SDB3_1

 # BISON
 ${BISON_Parser_OUTPUTS}

 # FLEX
 ${FLEX_Lexer_OUTPUTS}

 # src/
 src/main.cpp src/SDB3_header.h

 # Ketmap
 src/KetMap/KetMap.cpp src/KetMap/KetMap.h

 # Function
 src/Function/split_join.cpp
 src/Function/split_join.h
 src/Function/misc.cpp
 src/Function/misc.h
 src/Function/Timer.cpp
 src/Function/Timer.h
 src/Function/NaturalSort.h

 # Sequence
 src/Sequence/Ket.cpp
 src/Sequence/Ket.h
 src/Sequence/Superposition.cpp
 src/Sequence/Superposition.h
 src/Sequence/Sequence.cpp
 src/Sequence/Sequence.h
 src/Sequence/SuperpositionIter.cpp
 src/Sequence/SuperpositionIter.h
 src/Sequence/BaseSequence.h
 src/Sequence/OperatorWithSequence.cpp
 src/Sequence/OperatorWithSequence.h
 src/Sequence/SelfKet.cpp
 src/Sequence/SelfKet.h
 src/Sequence/IfElseStatement.cpp
 src/Sequence/IfElseStatement.h
 src/Sequence/ForStatement.cpp
 src/Sequence/ForStatement.h
 src/Sequence/WhileStatement.cpp
 src/Sequence/WhileStatement.h
 src/Sequence/LearnRule.cpp
 src/Sequence/LearnRule.h
 src/Sequence/MultiLearnRule.cpp
 src/Sequence/MultiLearnRule.h
 src/Sequence/MultiSelfKet.cpp
 src/Sequence/MultiSelfKet.h

 # Context
 src/Context/Frame.cpp
 src/Context/Frame.h
 src/Context/NewContext.cpp
 src/Context/NewContext.h
 src/Context/ContextList.cpp
 src/Context/ContextList.h
 src/Context/BoundFunction.cpp
 src/Context/BoundFunction.h

 # Operator
 src/Operator/SimpleOperator.cpp
 src/Operator/SimpleOperator.h
 src/Operator/NumericOperator.cpp
 src/Operator/NumericOperator.h
 src/Operator/BaseOperator.h
 src/Operator/PoweredOperator.cpp
 src/Operator/PoweredOperator.h
 src/Operator/OperatorSequence.cpp
 src/Operator/OperatorSequence.h
 src/Operator/BracketOperator.cpp
 src/Operator/BracketOperator.h
 src/Operator/CompoundOperator.cpp
 src/Operator/CompoundOperator.h
 src/Operator/EmptyOperator.h
 src/Operator/FunctionOperator.cpp
 src/Operator/FunctionOperator.h

 # OperatorLibrary
 src/OperatorLibrary/SigmoidLibrary.cpp
 src/OperatorLibrary/SigmoidLibrary.h
 src/OperatorLibrary/OperatorLibrary.cpp
 src/OperatorLibrary/OperatorLibrary.h
 src/OperatorLibrary/FunctionOperatorLibrary.cpp
 src/OperatorLibrary/FunctionOperatorLibrary.h

 # FunctionsOperatorMap
 src/FunctionOperatorMap/FunctionOperatorMap.cpp
 src/FunctionOperatorMap/FunctionOperatorMap.h

 # CompoundConstant
 src/CompoundConstant/CompoundConstant.cpp
 src/CompoundConstant/CompoundConstant.h
 src/CompoundConstant/ConstantInteger.cpp
 src/CompoundConstant/ConstantInteger.h
 src/CompoundConstant/ConstantFloat.cpp
 src/CompoundConstant/ConstantFloat.h
 src/CompoundConstant/ConstantOperator.cpp
 src/CompoundConstant/ConstantOperator.h
 src/CompoundConstant/ConstantString.cpp
 src/CompoundConstant/ConstantString.h

 # Parser
 src/Parser/Driver.cpp
 src/Parser/Driver.h
 src/Parser/Scanner.h

 # OperatorUsageMap
 src/OperatorUsageMap/OperatorUsageMap.cpp
 src/OperatorUsageMap/OperatorUsageMap.h

 # DocsGenerator
 src/DocsGenerator/DocsGenerator.cpp
 src/DocsGenerator/DocsGenerator.h

 )