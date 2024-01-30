file(GLOB EXECUTOR_SRCS "include/*.h")

add_library(ExecutorLib ${EXECUTOR_SRCS})
add_library(ExecutorLib.Execution SHARED)

target_include_directories(ExecutorLib PUBLIC include/)
target_link_libraries(ExecutorLib PRIVATE
	RobloxLib.TaskScheduler
	RobloxLib.Addresses
	Luau.VM.Internals
	Luau.Common
	UtilsLib
	Luau.VM
)

target_include_directories(ExecutorLib.Execution PUBLIC Execution/include/)
target_link_libraries(ExecutorLib.Execution PRIVATE 
	ExecutorLib
	Luau.Compiler
	Luau.VM.Internals
	Luau.VM
	UtilsLib
	RobloxLib.Addresses
	RobloxLib.TaskScheduler
)
file(GLOB EXECUTION_SRC "Execution/include/*.h" "Execution/src/*.cpp")
target_sources(ExecutorLib.Execution PRIVATE ${EXECUTION_SRC})
