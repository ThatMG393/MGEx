#include <taskscheduler.h>
#include <cstdint>
#include <executor.h>
#include <addresses.h>
#include <Luau/Common.h>
#include <lua.h>
#include <execution.h>
#include <lualib.h>

using namespace roblox;

uintptr_t executorScriptContext;
lua_State* executorThread;

bool executor::start(uintptr_t job) {
	taskscheduler::logJobs();

	uintptr_t executorSC = *reinterpret_cast<uintptr_t*>(job + 360);
	if (!executorSC) {
		return false;
	}
	executorScriptContext = executorSC;

	lua_State* executorLS = reinterpret_cast<lua_State*>(addresses::rbxGetLuaState(executorSC));
	if (!executorLS) {
		return false;
	}

	executorThread = functions::rlua_newthread(executorLS);
	if (!executorThread) {
		return false;
	}

	for (Luau::FValue<bool>* flag = Luau::FValue<bool>::list; flag; flag = flag->next)
		if (strncmp(flag->name, "Luau", 4) == 0)
			flag->value = true;

	lua_ref(executorLS, -1);
	lua_pop(executorLS, 1);

	execution::hookScriptJob(job);
	functions::set_identity(executorThread, 8);
	
	luaL_sandboxthread(executorThread);

	lua_newtable(executorThread);
	lua_setglobal(executorThread, "_G");

	lua_newtable(executorThread);
	lua_setglobal(executorThread, "shared");

	lua_settop(executorThread, 0);
	return true;
}

bool executor::stop() {
	executorThread = nullptr;
	executorScriptContext = 0;

	execution::thread::clearAllQueue();
	// DummyLScript = 0;
	
	return true;
}

lua_State* executor::globals::getExecutorThread() {
	return executorThread;
}

uintptr_t executor::globals::getExecutorScriptContext() {
	return executorScriptContext;
}
