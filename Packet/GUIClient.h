#pragma once
#include "CInPacket.h"
#include "COutPacket.h"

class GUIClient final : public UDPClient {
public:
	GUIClient(const std::wstring& serverIP, const uint16_t serverPort);
	~GUIClient();
	void Ping(uint32_t pid);
	void SendPacketInfo(PacketInfo& info);
	void HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort) override;
private:
	bool m_running = true;
	std::thread m_pingThread;
	static GUIClient* s_instance;
	void handlePacketInfo(PacketInfo& info);
	void handleFilterOpcodesSet(FilterOpcodeSet& set);
};