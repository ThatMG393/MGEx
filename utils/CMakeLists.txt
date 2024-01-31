cmake_minimum_required(VERSION 3.10)

file(GLOB UTILS_SRCS "include/*.h")

add_library(UtilsLib ${UTILS_SRCS})

set_target_properties(UtilsLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(UtilsLib PUBLIC include/)
target_link_libraries(UtilsLib PUBLIC log)

