#include <execution.h>
#include <queue>
#include <string>
#include <utility>
#include <addresses.h>

using namespace roblox::executor::execution;

std::queue<std::string> threadQueue;
std::queue<std::pair<roblox::structs::live_thread_ref*, int>> yieldQueue;

void thread::schedule(const std::string& script) {
	threadQueue.push(script);
}

void thread::scheduleThread(lua_State* state, int nargs) {
	roblox::structs::live_thread_ref* ref = new roblox::structs::live_thread_ref;
	ref->thread = state;
	lua_pushthread(state);
	ref->thread_id = lua_ref(state, -1);
	lua_pop(state, 1);

	auto yielded = std::make_pair(ref, nargs);
	yieldQueue.push(yielded);
}

bool thread::isThreadQueueEmpty() { return threadQueue.empty(); }
bool thread::isYieldQueueEmpty() { return yieldQueue.empty(); }
void thread::clearAllQueue() {
	while (isThreadQueueEmpty()) { threadQueue.pop(); }
	while(isYieldQueueEmpty()) { yieldQueue.pop(); }

}
std::string thread::getThreadTop() {
	auto j = threadQueue.front(); 
	threadQueue.pop();
	return j;
}
std::pair<roblox::structs::live_thread_ref*, int> thread::getYieldTop() {
	auto i = yieldQueue.front();
	yieldQueue.pop();
	return i;
}
