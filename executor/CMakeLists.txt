file(GLOB EXECUTOR_SRCS "include/*.h" "src/*.cpp")
file(GLOB EXECUTION_SRC "Execution/include/*.h" "Execution/src/*.cpp")

add_library(ExecutorLib ${EXECUTOR_SRCS})
add_library(ExecutorLib.Execution ${EXECUTION_SRC})

target_include_directories(ExecutorLib PUBLIC include/)
target_link_libraries(ExecutorLib PRIVATE
	RobloxLib.TaskScheduler
	RobloxLib.Addresses
	Luau.VM.Internals
	Luau.Common
	Luau.Compiler
	UtilsLib
	Luau.VM
	ExecutorLib.Execution
)

target_compile_features(ExecutorLib.Execution PUBLIC cxx_std_20)
target_include_directories(ExecutorLib.Execution PUBLIC Execution/include/)
target_link_libraries(ExecutorLib.Execution PRIVATE 
	ExecutorLib
	Luau.Compiler
	Luau.Ast
	Luau.VM.Internals
	Luau.VM
	UtilsLib
	RobloxLib.Addresses
	RobloxLib.TaskScheduler
)
