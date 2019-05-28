
#include "TunnelClient.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define IP_ADDR "127.0.0.1"
#define IP_PORT 27000

#include <iostream>
using namespace std;

TunnelClient::TunnelClient() {


	int iResult;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	// Create a SOCKET for connecting to server
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
		cout << "Waiting listener..." << endl;
	}


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
	
	cout << "SVE PROSLO" << endl;
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

