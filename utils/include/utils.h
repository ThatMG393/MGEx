#pragma once

#include <cstdint>
#include <iomanip>
#include <jni.h>
#include <obfuscate.h>
#include <sstream>
#include <thread>
#include <unwind.h>
#include <dlfcn.h>
#include <log.h>

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
	
	namespace backtrace {
		struct state {
			void** current;
			void** end;
		};

		static _Unwind_Reason_Code unwind_callback(struct _Unwind_Context* context, void* arg) {
			state* st = static_cast< state* >(arg);
			uintptr_t pc = _Unwind_GetIP(context);

			if (pc) {
				const void* addr = reinterpret_cast<void*>(pc);
				const char* symbol = "";

				Dl_info info;
				if (dladdr(addr, &info) && info.dli_sname) symbol = info.dli_sname;

				std::ostringstream os;
				os << OBFUSCATE("  #")
				   << std::setw(2)
				   << st->end - st->current
				   << OBFUSCATE(": ") << addr
				   << OBFUSCATE(" (")
				   << reinterpret_cast< void* >(
						reinterpret_cast< std::uintptr_t >(
							addr
						) - memory::getAddress(0)
					) << OBFUSCATE(") ") << symbol << OBFUSCATE("\n");
				LOGE("%s", os.str().c_str());

				if (st->current == st->end) return _URC_NORMAL_STOP;
				else *st->current++ = reinterpret_cast< void* >(pc);
			}

			return _URC_NO_REASON;
		}

		static void logcat(size_t max) {
			char* buffer = new char[max];
			state s = { reinterpret_cast< void** >( &buffer ), reinterpret_cast< void** >( &buffer ) + max };
			_Unwind_Backtrace(unwind_callback, &s);
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

