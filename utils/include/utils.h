#pragma once

#include <cstdint>
#include <cstring>
#include <jni.h>
#include "obfuscate.h"
#include <thread>
#include <execinfo.h>
#include <dlfcn.h>
#include "log.h"

typedef uintptr_t DWORD;

namespace utils {
	namespace memory {
		static DWORD findLibrary(const char *libraryName) {
			char filename[0xFF] = {0}, buffer[1024] = {0};
			FILE *fp = NULL;
			DWORD address = 0;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
			snprintf(filename, sizeof(filename), OBFUSCATE("/proc/self/maps"));
#pragma clang diagnostic pop

			fp = fopen(filename, OBFUSCATE("rt"));
			if (fp == NULL) {
				perror(OBFUSCATE("fopen"));
				goto done;
			}

			while (fgets(buffer, sizeof(buffer), fp)) {
				if (strstr(buffer, libraryName)) {
					address = (DWORD) strtoul(buffer, NULL, 16);
					goto done;
				}
			}

			done:
				if (fp) fclose(fp);

			return address;
		}


		static bool isLibraryLoaded(const char *libraryName) { 
			return findLibrary(libraryName) != 0;
		}

		static void waitForLib(const char* libName) {
			while (!isLibraryLoaded(libName)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}

		static DWORD getRelativeAddress(const char *libraryName, DWORD relativeAddr) {
			waitForLib(libraryName);
			DWORD libBase = findLibrary(libraryName);
			if (libBase == 0) return 0;
			return libBase + relativeAddr;
		}

		static DWORD getAddress(DWORD relativeAddr) {
			return getRelativeAddress(OBFUSCATE("libroblox.so"), relativeAddr);
		}
	}
	
	namespace stacktrace {
		static void print(int signal, int maxFrames, siginfo_t* info) {
			void* backtraceArray[maxFrames];
			int frames = backtrace(backtraceArray, maxFrames);	

			char** actualFrames = backtrace_symbols(backtraceArray, frames);
			LOGE("App crashed with signal %d (means %s) at address %p)", 
				signal, strsignal(signal), info->si_addr);

			for (int i = 1; i < frames && actualFrames != NULL; ++i) {
				LOGE("[Backtrace #%d]: %s", i, actualFrames[i]);
			}

			free(actualFrames);
		}
	}

	namespace Android {
		static JNIEnv* ApplicationEnv = { };
		static jobject AndroidContext = nullptr;

		static jobject getApplicationContext() {
			jclass ActivityThread = ApplicationEnv->FindClass(OBFUSCATE("android/app/ActivityThread"));
			jmethodID GetActivityThread = ApplicationEnv->GetStaticMethodID(ActivityThread, OBFUSCATE("currentActivityThread"), OBFUSCATE("()Landroid/app/ActivityThread;"));
			jobject CurrentActivityThread = ApplicationEnv->CallStaticObjectMethod(ActivityThread, GetActivityThread);
			jmethodID GetApplication = ApplicationEnv->GetMethodID(ActivityThread, OBFUSCATE("getApplication"), OBFUSCATE("()Landroid/app/Application;"));

			return ApplicationEnv->CallObjectMethod(ActivityThread, GetApplication);
		}

		static void showToast(const char* message, const int length) {
			jclass Toast = ApplicationEnv->FindClass("android/widget/Toast");
			jmethodID MakeText = ApplicationEnv->GetStaticMethodID(Toast, OBFUSCATE("makeText"), "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");

			jobject CreatedToast = ApplicationEnv->CallStaticObjectMethod(Toast, MakeText, getApplicationContext(), message, length);

			jmethodID Show = ApplicationEnv->GetMethodID(Toast, OBFUSCATE("show"), "()V;");
			ApplicationEnv->CallObjectMethod(CreatedToast, Show);
		}
	}

	static jboolean isGameLibLoaded(JNIEnv *env, jobject thiz) {
		return memory::isLibraryLoaded(OBFUSCATE("libroblox.so"));
	}
}

