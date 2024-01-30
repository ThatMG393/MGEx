cmake_minimum_required(VERSION 3.10)

add_library(Dobby include/Dobby/dobby.h)

set_target_properties(Dobby PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Dobby PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(Dobby PUBLIC include/)
target_link_directories(Dobby PUBLIC src/)
