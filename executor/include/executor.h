#include <lstate.h>
#include <cstdint>
#include <string>

namespace roblox::executor {
	static uintptr_t DummyLScript;

	bool start(uintptr_t job);
	bool stop();

	namespace globals {
		uintptr_t getExecutorScriptContext();
		lua_State* getExecutorThread();
	}

	namespace configurations {
		const std::string exploit_name = "MGEx";
		const std::string exploit_luaChunk = "@MB";
		const std::string exploit_version = "0.0.1";
		const std::string roblox_exploit_version = "2.605.660";
	}
}
