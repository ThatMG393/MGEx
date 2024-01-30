# This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0091 NEW)

add_library(RobloxLib INTERFACE)
add_library(RobloxLib.Addresses SHARED)
add_library(RobloxLib.Hooks SHARED)
add_library(RobloxLib.TaskScheduler SHARED)

target_include_directories(RobloxLib.Addresses PUBLIC Addresses/include)
target_link_libraries(RobloxLib.Addresses PUBLIC Luau.VM Luau.VM.Internals UtilsLib)
file(GLOB ADDR_SRC "Addresses/include/*.h" "Addresses/src/*.cpp")
target_sources(RobloxLib.Addresses PRIVATE ${ADDR_SRC})

target_include_directories(RobloxLib.Hooks PUBLIC Hooks/include)
target_link_libraries(RobloxLib.Hooks PUBLIC UtilsLib.Dobby RobloxLib.Addresses ExecutorLib ExecutorLib.Execution Luau.Compiler RobloxLib.TaskScheduler)
file(GLOB HOOKS_SRC "Hooks/include/*.h" "Hooks/src/*.cpp")
target_sources(RobloxLib.Hooks PRIVATE ${HOOKS_SRC})

target_include_directories(RobloxLib.TaskScheduler PUBLIC TaskScheduler/include)
target_link_libraries(RobloxLib.TaskScheduler PUBLIC Luau.VM RobloxLib.Addresses)
file(GLOB TS_SRC "TaskScheduler/include/*.h" "TaskScheduler/src/*.cpp")
target_sources(RobloxLib.TaskScheduler PRIVATE ${TS_SRC})

