#include <addresses.h>
#include <utils.h>

#define RBX_FUNC_ADDR roblox::addresses::functions
#define GET_ADDR(type, addr) reinterpret_cast<decltype(type)>(utils::memory::getAddress(addr))

int get_context_level_permissions(int identity) {
	unsigned int v1;
	int result;

	v1 = identity - 1;
	if (v1 <= 8) {
		switch (v1) {
			case 0u: case 3u: return 3;
			case 1u: return 0;
			case 2u: case 5u: return 11;
			case 4u: return 1;
			case 6u: case 7u: return 63;
			case 8u: return 12;
		}
	}

	return 0;
}


auto roblox::addresses::rbxLuaEnc(mem_addr sc) -> mem_addr {
	return *(mem_addr*)(sc + 276) + (sc + 276);
}

auto roblox::functions::get_taskscheduler() -> mem_addr {
	return *reinterpret_cast<mem_addr*>(utils::memory::getAddress(addresses::objects::taskscheduler));
}

auto roblox::functions::init() -> void {
	RBX_FUNC_ADDR::pushinstance_registry_rebased = utils::memory::getAddress(RBX_FUNC_ADDR::pushinstance_registry);

	scriptcontext_resume = GET_ADDR(scriptcontext_resume, RBX_FUNC_ADDR::scriptcontext_resume);
	sandboxthreadandsetidentity = GET_ADDR(sandboxthreadandsetidentity, RBX_FUNC_ADDR::sandboxthreadandsetidentity);

	rbx_spawn = GET_ADDR(rbx_spawn, RBX_FUNC_ADDR::rbx_spawn);
	rbx_getthreadcontext = GET_ADDR(rbx_getthreadcontext, RBX_FUNC_ADDR::rbx_getthreadcontext);
	
	rlua_newthread = GET_ADDR(rlua_newthread, RBX_FUNC_ADDR::rlua_newthread);
	rlua_pushinstance = GET_ADDR(rlua_pushinstance, RBX_FUNC_ADDR::rlua_pushinstance);
	rlua_pushinstanceSP = GET_ADDR(rlua_pushinstanceSP, RBX_FUNC_ADDR::rlua_pushinstance);
	
	fireclickdetector = GET_ADDR(fireclickdetector, RBX_FUNC_ADDR::fireclickdetector);
	firetouchinterest = GET_ADDR(firetouchinterest, RBX_FUNC_ADDR::firetouchinterest);
	fireproximityprompt = GET_ADDR(fireproximityprompt, RBX_FUNC_ADDR::fireproximityprompt);
}

auto roblox::functions::set_identity(lua_State* lstate, int identity) -> void {
	int permissions = get_context_level_permissions(identity);
	int ES = reinterpret_cast<mem_addr>(lstate->userdata);

	if (ES) {
		*reinterpret_cast<mem_addr*>(ES + 48) = permissions;
		*reinterpret_cast<mem_addr*>(ES + 24) = identity;
		*reinterpret_cast<mem_addr*>(ES + 72) = 0xFFFFFFFF;																 //     
	}

	auto context = reinterpret_cast<mem_addr>(rbx_getthreadcontext(lstate));
	if (context) {
		int result = 0;
		switch (identity) {
			case 1: case 4:
				result = 3;
				break;
			case 3: case 6:
				result = 11;
				break;
			case 5:
				result = 1;
				break;
			case 7: case 8:
				result = 63;
				break;
			case 9:
				result = 12;
				break;
		}

		mem_addr v18 = result | permissions & 0xFFFFFF00;
		*reinterpret_cast<mem_addr*>(context + 32) = v18; // capabilities
		// *reinterpret_cast<uintptr_t*>(context + 36) = 0; // ES + 52
		*reinterpret_cast<mem_addr*>(context + 40) = 0; // function
		*reinterpret_cast<mem_addr*>(context) = identity; // identity 
	}
}



