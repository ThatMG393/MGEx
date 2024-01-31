#include <lualib.h>
#include <Luau/Compiler.h>
#include <lua.h>
#include <addresses.h>
#include <execution.h>
#include <string>
#include <taskscheduler.h>
#include <executor.h>

using namespace roblox::executor::execution;

bool lua::executeLua(const std::string& script) {
	lua_State* executorState = roblox::executor::globals::getExecutorThread();
	lua_State* scriptThread = roblox::functions::rlua_newthread(executorState);
	lua_pop(executorState, 1);

	bytecode_encoder_t encoder = bytecode_encoder_t();
	std::string bytecode = Luau::compile(script, { 2, 1, 2 }, { true, true }, &encoder);

	roblox::functions::set_identity(scriptThread, 8);

	lua_getglobal(scriptThread, "task");
	lua_getfield(scriptThread, -1, "defer");
	if (luau_load(
		scriptThread,
		roblox::executor::configurations::exploit_luaChunk.c_str(),
		bytecode.c_str(),
		bytecode.size(),
		0
	) != 1) {
		auto err = lua_tostring(scriptThread, -1);

		lua_getglobal(scriptThread, "warn");
		lua_pushstring(scriptThread, err);
		lua_call(scriptThread, 1, 0);

		return false;
	}

	lua_pcall(scriptThread, 1, 0, 0);

	return true;
}

