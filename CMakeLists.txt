cmake_minimum_required(VERSION 3.10)

if (TARGET_ARCH == "armeabi-v7a")
	message("Arm!")
else()
	message("No Arm! :(")
endif()

project("MGex")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(deps/luau)
add_subdirectory(roblox)
add_subdirectory(executor)
add_subdirectory(utils)

add_library(MGEx SHARED main.cpp)

set_target_properties(RobloxLib.Addresses PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(RobloxLib.Hooks PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(RobloxLib.TaskScheduler PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(UtilsLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(UtilsLib.Dobby PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(ExecutorLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(ExecutorLib.Execution PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Luau.VM PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Luau.Common PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Luau.Compiler PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Luau.Ast PROPERTIES POSITION_INDEPENDENT_CODE ON)

find_package(JNI REQUIRED)    
include_directories(${JNI_INCLUDE_DIRS})

target_link_libraries(
	MGEx
	UtilsLib
	UtilsLib.Dobby
	RobloxLib.Addresses
	RobloxLib.Hooks
	RobloxLib.TaskScheduler
	ExecutorLib
	ExecutorLib.Execution
	Luau.VM
	Luau.VM.Internals
	Luau.Common
	Luau.Compiler
	Luau.Ast
)
