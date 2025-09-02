#include "pch.h"
#include "Hook.h"
#include "CClientSocket.h"
#include "Router.h"

#include "Share/Funcs.h"

namespace {

	inline bool checkAddr(uintptr_t addr, const std::wstring& funcName) {
		if (addr == 0) {
			DEBUGW(funcName + L" addr is empty");
			return false;
		}
		return true;
	}
}

namespace Hook {
	void Install(void* lpParameter) {
		HMODULE hModule = reinterpret_cast<HMODULE>(lpParameter);
		std::wstring dllDir = GetExecutableDir(hModule);
		Setting s{};
		bool ok = LoadSetting(dllDir, s);
		if (!ok) {
			DEBUGW(L"Failed to load setting");
			return;
		}
		const std::vector<std::pair<ULONG_PTR, std::wstring>> checks{
			{s.CInPacketDecode1Addr,L"CInPacket::Decode1"},
			{s.CInPacketDecode2Addr,L"CInPacket::Decode2"},
			{s.CInPacketDecode4Addr,L"CInPacket::Decode4"},
			{s.CInPacketDecodeStrAddr,L"CInPacket::DecodeStr"},
			{s.CInPacketDecodeBufferAddr,L"CInPacket::DecodeBuffer"},
			{s.COutPacketEncode1Addr,L"COutPacket::Encode1"},
			{s.COutPacketEncode2Addr,L"COutPacket::Encode2"},
			{s.COutPacketEncode4Addr,L"COutPacket::Encode4"},
			{s.COutPacketEncodeStrAddr,L"COutPacket::EncodeStr"},
			{s.COutPacketEncodeBufferAddr,L"COutPacket::EncodeBuffer"},
			{s.COutPacketMakeBufferListAddr,L"COutPacket::MakeBufferList"},
		};
		for (const auto& check : checks) {
			if (!checkAddr(check.first, check.second)) {
				return;
			}
		}
		// Filter Opcodes
		CInPacket::UpdateFilterOpcodeSet(s.CInPacketFilterOpcodes);
		COutPacket::UpdateFilterOpcodeSet(s.COutPacketFilterOpcodes);
		// CInPacket::Decode1
		SADDR(&CInPacket::Decode1, s.CInPacketDecode1Addr);
		SHOOK(true, &CInPacket::Decode1, CInPacket::Decode1_Hook);
		// CInPacket::Decode2
		SADDR(&CInPacket::Decode2, s.CInPacketDecode2Addr);
		SHOOK(true, &CInPacket::Decode2, CInPacket::Decode2_Hook);
		// CInPacket::Decode4
		SADDR(&CInPacket::Decode4, s.CInPacketDecode4Addr);
		SHOOK(true, &CInPacket::Decode4, CInPacket::Decode4_Hook);
		// CInPacket::Decode8
		if (s.CInPacketDecode8Addr > 0) {
			SADDR(&CInPacket::Decode8, s.CInPacketDecode8Addr);
			SHOOK(true, &CInPacket::Decode8, CInPacket::Decode8_Hook);
		}
		// CInPacket::DecodeStr
		SADDR(&CInPacket::DecodeStr, s.CInPacketDecodeStrAddr);
		SHOOK(true, &CInPacket::DecodeStr, CInPacket::DecodeStr_Hook);
		// CInPacket::DecodeBuffer
		SADDR(&CInPacket::DecodeBuffer, s.CInPacketDecodeBufferAddr);
		SHOOK(true, &CInPacket::DecodeBuffer, CInPacket::DecodeBuffer_Hook);
		// COutPacket::Encode1
		SADDR(&COutPacket::Encode1, s.COutPacketEncode1Addr);
		SHOOK(true, &COutPacket::Encode1, COutPacket::Encode1_Hook);
		// COutPacket::Encode2
		SADDR(&COutPacket::Encode2, s.COutPacketEncode2Addr);
		SHOOK(true, &COutPacket::Encode2, COutPacket::Encode2_Hook);
		// COutPacket::Encode4
		SADDR(&COutPacket::Encode4, s.COutPacketEncode4Addr);
		SHOOK(true, &COutPacket::Encode4, COutPacket::Encode4_Hook);
		// COutPacket::Encode8
		if (s.COutPacketEncode8Addr > 0) {
			SADDR(&COutPacket::Encode8, s.COutPacketEncode8Addr);
			SHOOK(true, &COutPacket::Encode8, COutPacket::Encode8_Hook);
		}
		// COutPacket::EncodeStr
		SADDR(&COutPacket::EncodeStr, s.COutPacketEncodeStrAddr);
		SHOOK(true, &COutPacket::EncodeStr, COutPacket::EncodeStr_Hook);
		// COutPacket::EncodeBuffer
		SADDR(&COutPacket::EncodeBuffer, s.COutPacketEncodeBufferAddr);
		SHOOK(true, &COutPacket::EncodeBuffer, COutPacket::EncodeBuffer_Hook);
		// COutPacket::MakeBufferList
		// Since some versions after BB have hook detection in CClientSocket::SendPacket
		// Using COutPacket::MakeBufferList_Hook instead of CClientSocket::SendPacket_Hook
		SADDR(&COutPacket::MakeBufferList, s.COutPacketMakeBufferListAddr);
		SHOOK(true, &COutPacket::MakeBufferList, COutPacket::MakeBufferList_Hook);
		// CClientSocket::ProcessPacket
		SADDR(&CClientSocket::ProcessPacket, s.CClientSocketProcessPacketAddr);
		SHOOK(true, &CClientSocket::ProcessPacket, CClientSocket::ProcessPacket_Hook);
		// CClientSocket::SendPacket
		SADDR(&CClientSocket::SendPacket, s.CClientSocketSendPacketAddr);
		//SHOOK(true, &CClientSocket::SendPacket, CClientSocket::SendPacket_Hook);
		// Router Init
		Router::Init(s.GUIServerIP, static_cast<uint16_t>(s.GUIServerPort));
	}

	void Uninstall() {
		SHOOK(false, &CInPacket::Decode1, CInPacket::Decode1_Hook);
		SHOOK(false, &CInPacket::Decode2, CInPacket::Decode2_Hook);
		SHOOK(false, &CInPacket::Decode4, CInPacket::Decode4_Hook);
		SHOOK(false, &CInPacket::Decode8, CInPacket::Decode8_Hook);
		SHOOK(false, &CInPacket::DecodeStr, CInPacket::DecodeStr_Hook);
		SHOOK(false, &CInPacket::DecodeBuffer, CInPacket::DecodeBuffer_Hook);
		SHOOK(false, &COutPacket::Encode1, COutPacket::Encode1_Hook);
		SHOOK(false, &COutPacket::Encode2, COutPacket::Encode2_Hook);
		SHOOK(false, &COutPacket::Encode4, COutPacket::Encode4_Hook);
		SHOOK(false, &COutPacket::Encode8, COutPacket::Encode8_Hook);
		SHOOK(false, &COutPacket::EncodeStr, COutPacket::EncodeStr_Hook);
		SHOOK(false, &COutPacket::EncodeBuffer, COutPacket::EncodeBuffer_Hook);
		SHOOK(false, &COutPacket::MakeBufferList, COutPacket::MakeBufferList_Hook);
		SHOOK(false, &CClientSocket::ProcessPacket, CClientSocket::ProcessPacket_Hook);
		//SHOOK(false, &CClientSocket::SendPacket, CClientSocket::SendPacket_Hook);
		Router::Free();
	}
}