#include <hooks.h>
#include <cstdint>
#include <jni.h>
#include <unordered_set>
#include <utils.h>
#include <dobby.h>
#include <executor.h>
#include <addresses.h>

using namespace roblox;

typedef uintptr_t mem_addr;

static std::unordered_set<mem_addr> startedJobs;

static mem_addr startScript_og = 0x0;
static mem_addr onGameLoaded_og = 0x0;
static mem_addr onGameLeave_og = 0x0;
static mem_addr jobStart_og = 0x0;
static mem_addr jobStop_og = 0x0;

static mem_addr RobloxContextSystem_CapabilityErr_og = 0x0;
static mem_addr RobloxContextSystem_CheckCapabilities_og = 0x0;
static mem_addr MemAdviceinit_og = 0x0;

static mem_addr startScript_hk(mem_addr sc, mem_addr script) {
	const auto orig = *reinterpret_cast<decltype(startScript_hk)*>(startScript_og);
	return orig(sc, script);
}

static mem_addr onGameLoaded_hk(mem_addr a1, int a2, int placeId, int a4) {
	const auto orig = *reinterpret_cast<decltype(onGameLoaded_hk)*>(onGameLoaded_og);
	return orig(a1, a2, placeId, a4);
}

static mem_addr onGameLeave_hk(mem_addr a1, int placeId) {
	const auto orig = *reinterpret_cast<decltype(onGameLeave_hk)*>(onGameLeave_og);
	return orig(a1, placeId);
}

static mem_addr jobStart_hk(mem_addr job) {
	const auto orig = *reinterpret_cast<decltype(jobStart_hk)*>(jobStart_og);
	
	auto job_name = *reinterpret_cast<const char**>(job + 128);
	if (!(*(uint8_t*)(job + 120) << 31)) job_name = (const char*)(job + 121);

	if (memcmp(job_name, "WaitingHybridScriptsJob", 23) == 0) {
		if (startedJobs.find(job) == startedJobs.end()) {
			startedJobs.insert(job);
			roblox::executor::start(job);
		}
	}

	return orig(job);
}

static mem_addr jobStop_hk(mem_addr job) {
	const auto orig = *reinterpret_cast<decltype(jobStop_hk)*>(jobStop_og);

	auto job_name = *reinterpret_cast<const char**>(job + 128);
	if (!(*(uint8_t*)(job + 120) << 31)) job_name = (const char*)(job + 121);

	if (memcmp(job_name, "WaitingHybridScriptsJob", 23) == 0) {
		executor::stop();
	}

	return orig(job);
}

static void RobloxContextSystem_CapabilityErr_hk(int a1, int a2, const char* a3, const char* a4) {
	const auto orig = *reinterpret_cast<decltype(RobloxContextSystem_CapabilityErr_hk)*>(RobloxContextSystem_CapabilityErr_og);
}

static int RobloxContextSystem_CheckCapabilities_hk(int64_t a1, int a2) {
	const auto orig = *reinterpret_cast<decltype(RobloxContextSystem_CheckCapabilities_hk)*>(RobloxContextSystem_CheckCapabilities_og);

	int inst = a1;
	auto required_cpbs = *reinterpret_cast<uint8_t*>(inst + 37);

	LOGD(" [RobloxContextSystem_CheckCapabilities_hook]: Required capabilities -> 0x%X", required_cpbs);

	return orig(a1, a2);
}

static int MemAdviceInit_hk(JNIEnv* env, jobject gContext) {
	const auto orig = *reinterpret_cast<decltype(MemAdviceInit_hk)*>(MemAdviceinit_og);

	utils::Android::AndroidContext = gContext;

	return orig(env, gContext);
}

auto hooks::init() -> void {
	const mem_addr jobStart = utils::memory::getAddress(addresses::functions::hooks::jobstart);
	const mem_addr onGameLeave = utils::memory::getAddress(addresses::functions::hooks::ongameleave);
	const mem_addr RobloxContextSystem_CapabilityErr = utils::memory::getAddress(addresses::functions::hooks::robloxcontextsystem_capabilityerr);
	
	DobbyHook(reinterpret_cast<void*>(jobStart), reinterpret_cast<void*>(jobStop_hk), reinterpret_cast<void**>(jobStart_og));
	DobbyHook(reinterpret_cast<void*>(onGameLeave), reinterpret_cast<void*>(onGameLeave_hk), reinterpret_cast<void**>(onGameLeave_og));
	DobbyHook(reinterpret_cast<void*>(RobloxContextSystem_CapabilityErr), reinterpret_cast<void*>(RobloxContextSystem_CapabilityErr_hk), reinterpret_cast<void**>(RobloxContextSystem_CapabilityErr_og));
}

