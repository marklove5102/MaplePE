#pragma once
#include <cstdint>

enum class Opcode : uint16_t {
	CP_Ping,
	CP_SendPacketInfo,
	LP_SendPacketInfo,
	LP_UpdateFilterOpcodeSet,
};