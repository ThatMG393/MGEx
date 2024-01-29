#include <addresses.h>
#include <cinttypes>
#include <csignal>
#include "jni.h"
#include <utils.h>
#include <executor.h>
#include <hooks.h>

void signal_handler(int signal) {
	utils::backtrace::logcat(30);
	exit(signal);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {
	LOGI("Loading Executor...");

	LOGI("Getting JNIEnv (Java Native Interface Environment)...");
	if (jvm->GetEnv(reinterpret_cast<void**>(&utils::Android::ApplicationEnv), JNI_VERSION_1_6) != JNI_OK) {
		LOGE("Failed to get JNIEnv!");
		return -1;
	}

	LOGI("Registering signal handler...");
	signal(SIGSEGV, signal_handler);

	LOGI("Now waiting for 'libroblox.so'");
	utils::memory::waitForLib(OBFUSCATE("libroblox.so"));
	LOGI("'libroblox.so' has been loaded!");
	LOGI("libroblox.so base addr -> 0x%" PRIXPTR, utils::memory::findLibrary("libroblox.so"));

	LOGI("Initiating Executor...");

	roblox::functions::init();
	roblox::hooks::init();

	LOGI("Executor initiated!");
	return JNI_VERSION_1_6;
}

