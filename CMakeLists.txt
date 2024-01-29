cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 30)

if ("${TARGET_ARCH}" STREQUAL "armeabi-v7a")
	message("Arm!")
	set(ANDROID_ARCH armv7a)  # or armv8a
	set(ANDROID_ABI ${ANDROID_ARCH}-linux-androideabi)
else()
	message("No Arm! :(")
	set(ANDROID_ARCH arm64)  # or armv8a
	set(ANDROID_ABI ${ANDROID_ARCH}-linux-androideabi)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target ${ANDROID_ABI} -march=${ANDROID_ARCH} -mfloat-abi=softfp -mfpu=vfpv3-d16")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target ${ANDROID_ABI} -march=${ANDROID_ARCH} -mfloat-abi=softfp -mfpu=vfpv3-d16")

set(ANDROID_PLATFORM android-30)

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

target_link_libraries(MGEx
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
