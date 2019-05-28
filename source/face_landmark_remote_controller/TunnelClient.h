
#ifndef TUNNEL_CLIENT_H
#define TUNNEL_CLIENT_H


#include <winsock2.h>

#include "TunnelData.h"

class TunnelClient{
public:
	TunnelClient();
	~TunnelClient();
	void sendTunnelData(const TunnelData& tunnelData);
private:
	
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
};

#endif