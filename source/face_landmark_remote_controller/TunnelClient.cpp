
#include "TunnelClient.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define IP_ADDR "127.0.0.1"
#define IP_PORT 27000

#define IP_PORT_PHONE 27001


#include <iostream>
using namespace std;

TunnelClient::TunnelClient() {


	int iResult;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;
	PhoneSocket = INVALID_SOCKET;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	// Create a SOCKET for connecting to server //////////////////////////////////// PRVI SOCKET /////////////////////
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		cout << "socket failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}


	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	addr.sin_port = htons(IP_PORT);


	iResult = ::bind(ListenSocket, (SOCKADDR *)& addr, sizeof(addr));
	if (iResult == SOCKET_ERROR) {
		cout << "Server: bind() failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	else {
		cout << "Server: bind() is OK." << endl;
	}

	// Create a SOCKET for connecting to server //////////////////////////////////// DRUGI SOCKET /////////////////////
	PhoneSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (PhoneSocket == INVALID_SOCKET) {
		cout << "socket 2 failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	char broadcast = '1';

	if (setsockopt(PhoneSocket, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast)) < 0) {
		cout << "setsockopt(SO_REUSEADDR) failed" << endl;
	}
	if (setsockopt(PhoneSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)

	{
		cout << "Error in setting Broadcast option" << endl;
	}


	sockaddr_in addr_phone;
	addr_phone.sin_family = AF_INET;
	addr_phone.sin_addr.s_addr = INADDR_ANY;								//INADDR_ANY;
	addr_phone.sin_port = htons(IP_PORT_PHONE);

	iResult = ::bind(PhoneSocket, (SOCKADDR *)& addr_phone, sizeof(addr_phone));
	if (iResult == SOCKET_ERROR) {
		cout << "Client: bind() failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	else {
		cout << "Client: bind() is OK." << endl;
	}

	cout << "All sockets are successfully created" << endl;
}


TunnelClient::~TunnelClient() {
	int iResult;
	// Gasenje socketa
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown failed with error: " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
}

void TunnelClient::connectWithUnreal()
{
	int iResult;

	cout << "Waiting listener..." << endl;

	// Waiting client ////////////////////////////////////////////////// The FIRST ONE ////////////////////////////////////
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		cout << "listen failed with error: " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "accept failed with error: " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	closesocket(ListenSocket);
	
}


void TunnelClient::sendTunnelData(const TunnelData& tunnelData) {

	int iSendResult;
	iSendResult = send(this->ClientSocket, (const char*)&tunnelData, sizeof(tunnelData), 0);
	if (iSendResult == SOCKET_ERROR) {
		cout << "send failed with error: " <<  WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}
	//TODO iSendResult must be equal to sizeof(tunnelData)
	//cout << "Bytes sent: " <<  iSendResult << endl;

}

void TunnelClient::getSensorsData( TunnelData& tunnelData) {

	unsigned long bytes_available = -1;
	int iResult;

	char data[256] = "proba";
	char temp[500000];


		ioctlsocket(PhoneSocket, FIONREAD, &bytes_available);

		if (bytes_available > 0) {

			while (bytes_available > 300) {

				iResult = recv(PhoneSocket, temp, bytes_available - 256, 0);
				if (iResult < 0) {
					cout << "Recv failed 1" << endl;
				}
				ioctlsocket(PhoneSocket, FIONREAD, &bytes_available);

			}


			iResult = recv(PhoneSocket, data, 8316, 0);
			if (iResult < 0) {
				cout<<"Recv failed 2"<<endl;
			}

			string dataS(data);

			tunnelData.orientX = ::atof(dataS.substr( 22, 27).c_str());
			tunnelData.orientY = ::atof(dataS.substr( 30, 35).c_str());
			tunnelData.orientY = ::atof(dataS.substr( 38, 43).c_str());

			cout << tunnelData.orientX << endl;

		}

}

