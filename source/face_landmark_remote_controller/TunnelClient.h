
#ifndef TUNNEL_CLIENT_H
#define TUNNEL_CLIENT_H


#include <winsock2.h>

#include "TunnelData.h"

class TunnelClient{
public:
	TunnelClient(); //Kontruktor se podesava i pravi se server, poveznica
	~TunnelClient();
	void sendTunnelData(const TunnelData& tunnelData); //Salje podatke
private:
	
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
};

#endif