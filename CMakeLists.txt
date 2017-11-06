cmake_minimum_required (VERSION 3.2)
project(ColoringClassifer)

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wredundant-decls")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -mssse3 -msse4.1 -msse4.2 -mavx -mbmi -march=native")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif(CMAKE_COMPILER_IS_GNUCXX)

ADD_DEFINITIONS(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

SET(SOURCE_FILES 
        src/coloring_classifier.h 
        src/BOB_hash.h
        src/multi_bloom_filter.h
        src/coded_bloom_filter.h
        src/shifting_bloom_filter.h
        src/multi_way_bf.h
        src/shift_coloring_classifier.h
)

add_executable(demo src/main.cpp ${SOURCE_FILES})