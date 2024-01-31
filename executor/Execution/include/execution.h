#pragma once

#include <addresses.h>
#include <Luau/Bytecode.h>
#include <Luau/BytecodeUtils.h>
#include <Luau/BytecodeBuilder.h>
#include <cstdint>
#include <lstate.h>
#include <string>
#include <utility>

namespace roblox::executor::execution {
	namespace lua {
		bool executeLua(const std::string& script);

		class bytecode_encoder_t : public Luau::BytecodeEncoder {
			inline void encode(uint32_t* data, size_t count) override {
				for (uint32_t i = 0u; i < count;) {
					uint8_t& opcode = *reinterpret_cast<uint8_t*>(data + i);
					i += Luau::getOpLength(LuauOpcode(opcode));
					opcode *= 227;
				}
			}
		};
	}

	namespace thread {
		void schedule( const std::string& script);
		void scheduleThread(lua_State* luaState, int nargs);

		bool isThreadQueueEmpty();
		bool isYieldQueueEmpty();
		
		void clearAllQueue();

		std::string getThreadTop();
		std::pair<roblox::structs::live_thread_ref*, int> getYieldTop();
	}

	void hookScriptJob(uintptr_t job);
}
