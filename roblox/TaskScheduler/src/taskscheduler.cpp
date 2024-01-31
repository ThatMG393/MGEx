#include <cstdint>
#include <taskscheduler.h>
#include <addresses.h>
#include <log.h>

uintptr_t new_vftable[7];
int fpscap_offset = -1;

using namespace roblox;

/*
Major issue:
	fails to get new jobs
	resulting in old jobs being gotten
	so game switching fails.
Cause: no idea
*/

auto taskscheduler::getJobs() -> std::vector<std::uintptr_t> {
    std::vector<std::uintptr_t> job_list{ };
    
    auto current_job = *reinterpret_cast<uintptr_t**>(roblox::functions::get_taskscheduler() + 236);
    do {
        job_list.push_back(*current_job);
        current_job += 2;
    } while (current_job != *reinterpret_cast<uintptr_t**>(roblox::functions::get_taskscheduler() + 240));
    
    return job_list;
}

// testing purposes 
auto taskscheduler::logJobs() -> void {
    auto job_list = getJobs();
    
    int i = 0;
    for (uintptr_t job : job_list) {
        auto job_name = *reinterpret_cast<const char**>(job + 128);

        if (!(*(uint8_t*)(job + 120) << 31)) {
            job_name = (const char*)(job + 121);
        }
        
        LOGI("taskscheduler::log_jobs | Job[ %s ] 0x%X ", job_name, job);
        i++;
    }
    
    LOGI("taskscheduler::log_jobs | Logged %i Jobs ", i);
}

auto taskscheduler::getJobByName(const std::string& name) -> uintptr_t {
    auto job_list = taskscheduler::getJobs();
    
    for (auto job : job_list) {
        auto job_name = *reinterpret_cast<const char**>(job + 128);

        if ( !(*(std::uint8_t*)(job + 120) << 31) ) {
            job_name = (const char*)(job + 121);
        }
        
        if (memcmp(job_name, name.c_str(), name.size()) == 0) {
            LOGI(" Found Job %s, 0x%X", name.c_str(), job);
            return job;
        }
    }
    
    return 0;
}

auto taskscheduler::hookToJob(uintptr_t job, void* dst) -> uintptr_t {
    memcpy(new_vftable, *reinterpret_cast<void**>(job), 4u * 7u);

    auto old_vftable = *reinterpret_cast<uintptr_t**>(job);
    new_vftable[6] = reinterpret_cast<uintptr_t>(dst);

    *reinterpret_cast<uintptr_t**>(job) = new_vftable;
    return old_vftable[6];
}

auto taskscheduler::findFPSCapOffset() -> uintptr_t {
    if (fpscap_offset != -1) return fpscap_offset;
    auto TS = roblox::functions::get_taskscheduler();

	for ( int i = 0; i < 123456; i += 4 ) {
		auto val = *reinterpret_cast<double*>(TS + i);
		if (1.0 / val == 60.0) {
			LOGD(" FPS Cap Offset: %i", i);
			fpscap_offset = i;
			break;
		}
	}
    
    
    return fpscap_offset;
}

auto taskscheduler::setFPSCap(double cap) -> void {
    static auto off = taskscheduler::findFPSCapOffset();
    
    static const double min_frame_delay = 1.0 / 1000.0;
    double frame_delay = cap <= 0.0 ? min_frame_delay : 1.0 / cap;
    *reinterpret_cast<double*>(roblox::functions::get_taskscheduler() + off) = frame_delay;
}

auto taskscheduler::getFPSCap() -> double {
    static auto off = taskscheduler::findFPSCapOffset();
    
    auto cap = *reinterpret_cast<double*>(roblox::functions::get_taskscheduler() + off);
    return 1.0 / cap;
}

auto taskscheduler::getMainState() -> lua_State* {
	return reinterpret_cast<lua_State*>(addresses::rbxGetLuaState(taskscheduler::getMainScriptContext()));
}

auto taskscheduler::getMainScriptContext() -> uintptr_t {
    return *reinterpret_cast<uintptr_t*>(getJobByName("WaitingHybridScriptsJob") + 360);
}


