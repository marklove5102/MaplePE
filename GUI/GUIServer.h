#pragma once
#include "IMainController.h"

class GUIServer final : public UDPServer
{
public:
	GUIServer(IMainController* mainControllerImpl);
	~GUIServer();
	std::vector<uint8_t> Log2Buffer(const PacketLogModel& log);
	std::wstring SendPacketInfo(const PacketLogModel& log);
	std::wstring BroadcastPacketInfo(const PacketLogModel& log);
	std::wstring BroadcastFilterOpcodes(const std::wstring& inOpcodes, const std::wstring& outOpcodes);
	void HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort) override;
private:
	std::map<uint32_t, ClientEndpoint> m_clientEndpointMap;
	IMainController* m_mainControllerImpl = nullptr;
	std::wstring sendPacket(uint32_t pid, std::vector<uint8_t> buffer);
	std::wstring broadcastPacket(std::vector<uint8_t> buffer);
	void handlePacketInfo(PacketInfo& info);
};