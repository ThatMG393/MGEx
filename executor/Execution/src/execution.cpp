#include <addresses.h>
#include <cstdint>
#include <execution.h>
#include <map>
#include <mutex>
#include <taskscheduler.h>
#include <executor.h>
#include <utility>

using namespace roblox::executor;

std::map<uintptr_t, uintptr_t> jobCache { };
std::mutex mutex;

uintptr_t jobHook(uintptr_t job) {
	std::unique_lock<std::mutex> guard { mutex };
	if (!execution::thread::isThreadQueueEmpty()) {
		guard.unlock();
		execution::lua::executeLua(
			execution::thread::getThreadTop()
		);
	}

	if (!execution::thread::isYieldQueueEmpty()) {
		auto yielded = execution::thread::getYieldTop();
		uintptr_t unk[4] = { 0, 0, 0 };
		roblox::functions::scriptcontext_resume(
			reinterpret_cast<uintptr_t>(unk),
			roblox::executor::globals::getExecutorScriptContext(),
			reinterpret_cast<uintptr_t*>(&yielded.first),
			yielded.second, 0, 0
		);
	}

	return reinterpret_cast<uintptr_t(*)(uintptr_t)>(jobCache.at(job))(job);
}

void execution::hookScriptJob(uintptr_t job) {
	if (!jobCache.contains(job)) {
		uintptr_t jobHk = taskscheduler::hookToJob(job, (void*)&jobHook);
		jobCache.insert(std::make_pair(job, jobHk));
	}
}
