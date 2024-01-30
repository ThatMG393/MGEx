#include <execution.h>
#include <log.h>
#include <string>
#include <taskscheduler.h>
#include <Luau/Common.h>
#include <addresses.h>
#include <lualib.h>

namespace roblox::executor {
	static bool start(uintptr_t job) {
		taskscheduler* taskScheduler = taskscheduler::get_singleton();
		taskScheduler->log_jobs();

		uintptr_t scriptCtx = *reinterpret_cast<uintptr_t*>(job + 360);
		if (!scriptCtx) {
			LOGE("Failed to get script context!");
			return false;
		}

		lua_State* rbxLstate = reinterpret_cast<lua_State*>(roblox::addresses::rbxLuaEnc(scriptCtx));
		if (!rbxLstate) {
			LOGE("Failed to get Roblox Lua State!");
			return false;
		}

		for (Luau::FValue<bool>* flag = Luau::FValue<bool>::list; flag; flag = flag->next)
			if (strncmp(flag->name, "Luau", 4) == 0)
				flag->value = true;

		taskScheduler->set_CurrentSC(scriptCtx);
		taskScheduler->set_ExploitState(roblox::functions::rlua_newthread(rbxLstate));
		lua_ref(rbxLstate, -1);
		lua_pop(rbxLstate, 1);

		lua_State* exploitLstate = taskScheduler->get_ExploitState();
		if (!exploitLstate) {
			LOGE("Failed to create new thread!");
			return false;
		}

		roblox::functions::set_identity(exploitLstate, 8);
		luaL_sandboxthread(exploitLstate);

		lua_newtable(exploitLstate);
		lua_setglobal(exploitLstate, "_G");

		lua_newtable(exploitLstate);
		lua_setglobal(exploitLstate, "shared");

		lua_settop(exploitLstate, 0);

		return true;
	}

	static void stop() {
		auto taskScheduler = taskscheduler::get_singleton();

		taskScheduler->set_CurrentSC(0);
		taskScheduler->set_ExploitState(nullptr);

		execution::get_singleton()->clearqueues();
		DummyLScript = 0;
	}

	namespace configurations {
		const std::string exploit_name = "MGEx";
		const std::string exploit_luaChunk = "@MB";
		const std::string exploit_version = "0.0.1";
		const std::string roblox_exploit_version = "2.605.660"; 	
	}
}
