#pragma once

#pragma pack(push, 1)
typedef struct {
	uint32_t m_bLoopback;
	uint8_t* m_aSendBuff;
	uint32_t m_uOffset;
	uint32_t m_bIsEncryptedByShanda;
} OutPacket;
#pragma pack(pop)

namespace COutPacket {

	void UpdateFilterOpcodeSet(std::wstring& opcodes);

	bool IsFilterOpcode(uint16_t opcode);

	std::vector<PacketAction>* GetActions(void* key);

	void DeleteActions(void* key);

	void SetActions(void* key, const std::vector<PacketAction>& actions);

	// GMS95 void __thiscall COutPacket::Encode1(COutPacket *this, unsigned __int8 n)
	extern void(__thiscall* Encode1)(void* ecx, uint8_t n);
	void __fastcall Encode1_Hook(void* ecx, void* edx, uint8_t n);

	// GMS95 void __thiscall COutPacket::Encode2(COutPacket *this, unsigned __int16 n)
	extern void(__thiscall* Encode2)(void* ecx, uint16_t n);
	void __fastcall Encode2_Hook(void* ecx, void* edx, uint16_t n);

	// GMS95 void __thiscall COutPacket::Encode4(COutPacket *this, unsigned int n)
	extern void(__thiscall* Encode4)(void* ecx, uint32_t n);
	void __fastcall Encode4_Hook(void* ecx, void* edx, uint32_t n);

	// KMS1029 void __thiscall COutPacket::Encode8(COutPacket *this, __int64 n)
	extern void(__thiscall* Encode8)(void* ecx, uint64_t n);
	void __fastcall Encode8_Hook(void* ecx, void* edx, uint64_t n);

	// GMS95 void __thiscall COutPacket::EncodeStr(COutPacket *this, ZXString<char> s)
	extern void(__thiscall* EncodeStr)(void* ecx, char* s);
	void __fastcall EncodeStr_Hook(void* ecx, void* edx, char* s);

	// GMS95 void __thiscall COutPacket::EncodeBuffer(COutPacket *this, unsigned __int8 *p, unsigned int uSize)
	extern void(__thiscall* EncodeBuffer)(void* ecx, uint8_t* p, uint32_t uSize);
	void __fastcall EncodeBuffer_Hook(void* ecx, void* edx, uint8_t* p, uint32_t uSize);

	// GMS95 void __thiscall COutPacket::MakeBufferList(COutPacket *this,ZList<ZRef<ZSocketBuffer> > *l, unsigned __int16 uSeqBase, unsigned int *puSeqKey, int bEnc,unsigned int dwKey)
	extern void(__thiscall* MakeBufferList)(void* ecx, void* l, uint16_t uSeqBase, uint32_t* puSeqKey, int bEnc, uint32_t dwKey);
	void __fastcall MakeBufferList_Hook(void* ecx, void* edx, void* l, uint16_t uSeqBase, uint32_t* puSeqKey, int bEnc, uint32_t dwKey);
}