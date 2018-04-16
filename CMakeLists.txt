cmake_minimum_required(VERSION 3.10)
project(MyCompiler)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(st)

add_executable(MyCompiler
        st/st.c
        st/st.h
        ast.c
        ast.h
        grammar.tab.c
        grammar.tab.h
        grammar.y
        lex.l
        lex.yy.c
        main.c
        makefile
        parser.y
        scanner.l)
