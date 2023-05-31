cmake_minimum_required(VERSION 3.10)
project(SDB3_1)

set(CMAKE_CXX_STANDARD 17)

# compiler settings for use of gprof
# invoke using: cmake -DUSE_GPROF=ON ..
if (USE_GPROF)
    message("Configuring for gprof")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

# find build requirements
FIND_PACKAGE(BISON 3.3 REQUIRED)
FIND_PACKAGE(FLEX)

## INCLUDES
# include root source directory for includes search
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
# include binary build directory for includes search
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
# include 3rd party headers for includes search
INCLUDE_DIRECTORIES(src/tclap/)


## SDB CORE
# Parser: generated bison/flex outputs for Parser
BISON_TARGET(Parser src/Parser/Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(Lexer src/Parser/Scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

# Parser: set parser header and source files explicitly to avoid any generated bison/flex outputs
set(Parser_SRC "src/Parser/Driver.h" "src/Parser/Driver.cpp" "src/Parser/Scanner.h")

# collect all files ending in .h and .cpp
file(GLOB CompoundConstant_SRC CONFIGURE_DEPENDS "src/CompoundConstant/*.h" "src/CompoundConstant/*.cpp")
file(GLOB Context_SRC CONFIGURE_DEPENDS "src/Context/*.h" "src/Context/*.cpp")
file(GLOB DocsGenerator_SRC CONFIGURE_DEPENDS "src/DocsGenerator/*.h" "src/DocsGenerator/*.cpp")
file(GLOB Function_SRC CONFIGURE_DEPENDS "src/Function/*.h" "src/Function/*.cpp")
file(GLOB FunctionOperatorMap_SRC CONFIGURE_DEPENDS "src/FunctionOperatorMap/*.h" "src/FunctionOperatorMap/*.cpp")
file(GLOB KetMap_SRC CONFIGURE_DEPENDS "src/KetMap/*.h" "src/KetMap/*.cpp")
file(GLOB Operator_SRC CONFIGURE_DEPENDS "src/Operator/*.h" "src/Operator/*.cpp")
file(GLOB OperatorLibrary_SRC CONFIGURE_DEPENDS "src/OperatorLibrary/*.h" "src/OperatorLibrary/*.cpp")
file(GLOB OperatorUsageMap_SRC CONFIGURE_DEPENDS "src/OperatorUsageMap/*.h" "src/OperatorUsageMap/*.cpp")
file(GLOB Sequence_SRC CONFIGURE_DEPENDS "src/Sequence/*.h" "src/Sequence/*.cpp")

# SDB CORE library
set(CORE_SOURCE_FILES
     ${Parser_SRC}
     ${BISON_Parser_OUTPUTS}
     ${FLEX_Lexer_OUTPUTS}
     ${CompoundConstant_SRC}
     ${Context_SRC}
     ${DocsGenerator_SRC}
     ${Function_SRC}
     ${FunctionOperatorMap_SRC}
     ${KetMap_SRC}
     ${Operator_SRC}
     ${OperatorLibrary_SRC}
     ${OperatorUsageMap_SRC}
     ${Sequence_SRC}
)
add_library(sdb_core STATIC ${CORE_SOURCE_FILES})


# BUILD TARGETS
# FIXME: add a GUI target here

# CLI binary
add_executable(SDB3_1 src/main.cpp src/SDB3_header.h)
target_link_libraries(SDB3_1 sdb_core)

# test main binary
add_executable(test_main src/test_main.cpp src/SDB3_header.h)
target_link_libraries(test_main sdb_core)

# test parser binary
add_executable(test_parser src/test_parser_main.cpp src/SDB3_header.h)
target_link_libraries(test_parser sdb_core)

