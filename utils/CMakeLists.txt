cmake_minimum_required(VERSION 3.10)

file(GLOB UTILS_SRCS "include/*.h")

add_library(UtilsLib ${UTILS_SRCS})
add_library(UtilsLib.Dobby SHARED Dobby/dobby.h)

set_target_properties(UtilsLib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(UtilsLib.Dobby PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(UtilsLib PUBLIC include/)
# target_link_libraries(UtilsLib PUBLIC)

target_include_directories(UtilsLib.Dobby PUBLIC Dobby/)
