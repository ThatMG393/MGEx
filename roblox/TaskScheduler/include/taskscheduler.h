#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <lua.h>

// this was increased by + 112.
// so datamodeljobs also now have sc at + 360 ( 248 + 112 = 360 )
// this not used because string name issues but kept as reference 
namespace roblox::taskscheduler {
	struct rbxjob_t {
		uint8_t gap0[120];
		std::string name; // now at 120
		uint8_t gap1[16];
		double time;
		uint8_t gap2[16];
		double time_spend;
		uint8_t gap3[8];
		uintptr_t state;
	};
    
    auto getJobs() -> std::vector<uintptr_t>;
    auto logJobs() -> void;
    auto getJobByName(const std::string& name) -> uintptr_t;
    auto hookToJob(uintptr_t job, void* hook) -> uintptr_t;
    
    auto findFPSCapOffset() -> uintptr_t;
    auto setFPSCap(double cap) -> void;
    auto getFPSCap() -> double;

	auto getMainState() -> lua_State*;
    auto getMainScriptContext() -> uintptr_t; 
}
