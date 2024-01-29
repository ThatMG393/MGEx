#pragma once

#include <cstdint>
#include <lstate.h>
#include <memory>

typedef uintptr_t mem_addr;

#define CreateMemAddrForRbx(addr) (addr + 1)

namespace roblox {
	namespace addresses {
		namespace objects {
			// DONT USE CreateMemAddrForRbx here unless 
			// you want your memory addresses to be off by 1
			constexpr mem_addr taskscheduler = 0x41DC86C;
			constexpr mem_addr prop_table = 0x405F324;
		}

		namespace functions {
			namespace hooks {
				constexpr mem_addr startscript = CreateMemAddrForRbx(0x1920834);

				constexpr mem_addr ongameloaded = CreateMemAddrForRbx(0x131EAC8);
				constexpr mem_addr ongameleave = CreateMemAddrForRbx(0x131ED10);

				constexpr mem_addr jobstart = CreateMemAddrForRbx(0x3A4C48C);
				constexpr mem_addr jobstop = CreateMemAddrForRbx(0x3A4C4FC);

				constexpr mem_addr robloxcontextsystem_capabilityerr = CreateMemAddrForRbx(0x38BDC04);
			}

			constexpr mem_addr scriptcontext_resume = CreateMemAddrForRbx(0x18F4760);
			constexpr mem_addr sandboxthreadandsetidentity = CreateMemAddrForRbx(0x18E51E8);

			constexpr mem_addr rbx_spawn = CreateMemAddrForRbx(0x1932D30);
			constexpr mem_addr rbx_getthreadcontext = CreateMemAddrForRbx(0x0);

			constexpr mem_addr rlua_newthread = CreateMemAddrForRbx(0x36CC828);

			constexpr mem_addr rlua_pushinstance = CreateMemAddrForRbx(0x18A2668);
			constexpr mem_addr pushinstance_registry = CreateMemAddrForRbx(0x18A20B4);
			static mem_addr pushinstance_registry_rebased = 0x0;
			
			constexpr mem_addr fireclickdetector = CreateMemAddrForRbx(0x23CF824);
			constexpr mem_addr fireproximityprompt = CreateMemAddrForRbx(0x23D3E78);
			constexpr mem_addr firetouchinterest = CreateMemAddrForRbx(0x2F44E84);
		}

		auto rbxLuaEnc(mem_addr sc) -> uintptr_t;
	}

	namespace functions {
		auto init() -> void;
		auto get_taskscheduler() -> mem_addr;
		auto set_identity(lua_State* lstate, int identity) -> void;

		inline int (*rbx_spawn)(lua_State* rL) = nullptr;
		inline lua_State* (*rlua_newthread)(lua_State* rL) = nullptr;
		inline mem_addr (*rbx_getthreadcontext)(lua_State* thread) = nullptr;
		inline int (*scriptcontext_resume)(int unk, std::uintptr_t sc, std::uintptr_t* ref, int nargs, int, int) = nullptr;
		inline mem_addr (*sandboxthreadandsetidentity)(lua_State* ls, std::uintptr_t identity, std::uintptr_t script) = nullptr;
		inline mem_addr (*rlua_pushinstance)(lua_State* ls, std::uintptr_t inst) = nullptr;
		inline mem_addr (*rlua_pushinstanceSP)(lua_State* ls, std::weak_ptr<uintptr_t> inst) = nullptr;
		inline mem_addr (*fireclickdetector)(std::uintptr_t detector, float distance, std::uintptr_t player) = nullptr;
		inline mem_addr (*fireproximityprompt)(std::uintptr_t prompt) = nullptr;
		inline mem_addr (*firetouchinterest)(std::uintptr_t world, std::uintptr_t to_touch, std::uintptr_t transmitter, int untouch, int) = nullptr;
	}

	namespace structs {
		struct live_thread_ref {
			int unk_0;
			lua_State* th;
			int thread_id;
		};

		struct weak_thread_ref_t {
			uint8_t pad_0[8];

			weak_thread_ref_t* previous; // 8
			weak_thread_ref_t* next; // 12 

			live_thread_ref* livethreadref; // 16
		};

		struct ExtraSpace_t {
			struct Shared {
                int threadCount; // 0
                std::uintptr_t script_context; // 4
                ExtraSpace_t* allThreads; // 8
            };
            
            ExtraSpace_t* previous; // 0
            size_t count; // 4
            ExtraSpace_t* next; // 8
			std::shared_ptr<Shared> shared; //12, 16
            
            weak_thread_ref_t* Node; // 20 // shouldnt this be weak_thread_ref_t::Node ?
            int context_level; // 24
            
            std::uint8_t pad_0[12];
            
            std::uintptr_t script_context; //40
            std::uintptr_t unk_0; //44
            std::uintptr_t context_perms; // 48
            std::uintptr_t unk_1; // 52
            std::weak_ptr<uintptr_t> script; //56
            // 60, 64, 68 contain weak refs to script, 72, 76 to a parent of the script 
		};
	}

	namespace luau { }
}
