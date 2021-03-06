
#ifndef TUNNEL_CLIENT_H
#define TUNNEL_CLIENT_H


#include <winsock2.h>
#include <vector>


#include "TunnelData.h"

class TunnelClient{
public:
	TunnelClient(); //Kontruktor se podesava i pravi se server, poveznica
	~TunnelClient();
	void connectWithUnreal();
	void sendTunnelData(const TunnelData& tunnelData); //Salje podatke
	std::vector < float > calibrationOrient();
	std::vector < float > getSensorsData(); //Prima sensor data
private:
	
	WSADATA wsaData;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	SOCKET PhoneSocket;

	double aX, aY, aZ;

};

#endif