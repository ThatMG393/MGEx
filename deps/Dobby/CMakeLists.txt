cmake_minimum_required(VERSION 3.10)

add_library(Dobby include/Dobby/dobby.h)

set_target_properties(Dobby PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Dobby PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(Dobby PUBLIC include/)
link_directories(${CMAKE_SOURCE_DIR}/deps/Dobby/src/libdobby.a)
target_link_libraries(Dobby PRIVATE ${CMAKE_SOURCE_DIR}/deps/Dobby/src/libdobby.a)
