# This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0091 NEW)

file(GLOB ADDR_SRC "Addresses/include/*.h" "Addresses/src/*.cpp")
file(GLOB HOOKS_SRC "Hooks/include/*.h" "Hooks/src/*.cpp")
file(GLOB TS_SRC "TaskScheduler/include/*.h" "TaskScheduler/src/*.cpp")

add_library(RobloxLib INTERFACE)
add_library(RobloxLib.Addresses ${ADDR_SRC})
add_library(RobloxLib.Hooks ${HOOKS_SRC})
add_library(RobloxLib.TaskScheduler ${TS_SRC})


# if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Android")
# 	target_link_libraries(UtilsLib PUBLIC ${log-lib})
# 	find_library(log-lib log)
# endif()


target_include_directories(RobloxLib.Addresses PUBLIC Addresses/include)
target_link_libraries(RobloxLib.Addresses PUBLIC Luau.VM Luau.VM.Internals UtilsLib)

target_include_directories(RobloxLib.Hooks PUBLIC Hooks/include)
target_link_libraries(RobloxLib.Hooks PUBLIC UtilsLib Dobby RobloxLib.Addresses ExecutorLib.Execution ExecutorLib Luau.Compiler Luau.VM Luau.VM.Internals RobloxLib.TaskScheduler) 

target_include_directories(RobloxLib.TaskScheduler PUBLIC TaskScheduler/include)
target_link_libraries(RobloxLib.TaskScheduler PUBLIC Luau.VM RobloxLib.Addresses UtilsLib)
