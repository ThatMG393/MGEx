# This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
cmake_minimum_required(VERSION 3.10)
project("Luau" LANGUAGES CXX C)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0091 NEW)

add_library(Luau INTERFACE)
add_library(Luau.Common INTERFACE)
add_library(Luau.Ast STATIC)
add_library(Luau.Compiler STATIC)
add_library(Luau.VM STATIC)

# Proxy target to make it possible to depend on private VM headers
add_library(Luau.VM.Internals INTERFACE)

include(Sources.cmake)

target_include_directories(Luau.Common INTERFACE Common/include)

target_compile_features(Luau.Ast PUBLIC cxx_std_17)
target_include_directories(Luau.Ast PUBLIC Ast/include)
target_link_libraries(Luau.Ast PUBLIC Luau.Common)

target_compile_features(Luau.Compiler PUBLIC cxx_std_17)
target_include_directories(Luau.Compiler PUBLIC Compiler/include)
target_link_libraries(Luau.Compiler PUBLIC Luau.Ast)

target_compile_features(Luau.VM PRIVATE cxx_std_11)
target_include_directories(Luau.VM PUBLIC VM/include)
target_link_libraries(Luau.VM PUBLIC Luau.Common)

target_include_directories(Luau.VM.Internals INTERFACE VM/src)

set(LUAU_OPTIONS)

list(APPEND LUAU_OPTIONS -Wall) # All warnings
list(APPEND LUAU_OPTIONS -Wsign-compare) # This looks to be included in -Wall for GCC but not clang

# Some gcc versions treat var in `if (type var = val)` as unused
# Some gcc versions treat variables used in constexpr if blocks as unused
list(APPEND LUAU_OPTIONS -Wno-unused)

# Enabled in CI; we should be warning free on our main compiler versions but don't guarantee being warning free everywhere
if(LUAU_WERROR)
	list(APPEND LUAU_OPTIONS -Werror) # Warnings are errors
endif()

target_compile_options(Luau.Ast PRIVATE ${LUAU_OPTIONS})
target_compile_options(Luau.VM PRIVATE ${LUAU_OPTIONS})

# disable support for math_errno which allows compilers to lower sqrt() into a single CPU instruction
target_compile_options(Luau.VM PRIVATE -fno-math-errno)

# validate dependencies for internal libraries
foreach(LIB Luau.Ast Luau.Compiler Luau.VM)
    if(TARGET ${LIB})
        get_target_property(DEPENDS ${LIB} LINK_LIBRARIES)
        if(LIB MATCHES "VM" AND DEPENDS MATCHES "Ast|Compiler")
            message(FATAL_ERROR ${LIB} " is a runtime component but it depends on one of the offline components")
        endif()
        if(LIB MATCHES "Ast|Compiler" AND DEPENDS MATCHES "VM")
            message(FATAL_ERROR ${LIB} " is an offline component but it depends on one of the runtime components")
        endif()
    endif()
endforeach()
