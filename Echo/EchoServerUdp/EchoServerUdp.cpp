// EchoServerUdp.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void GetLastSocketError(const char *);

int main()
{
	SOCKET hSocket;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup Error" << endl;
		system("pause");
		exit(-1);
	}

	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (hSocket == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(5000);
	if (bind(hSocket, (LPSOCKADDR)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		GetLastSocketError("bind()");

	SOCKADDR_IN clntAddr;
	int szClntAddr;
	char message[200];
	int strLen;
	while (true)
	{
		ZeroMemory(message, sizeof(message));
		szClntAddr = sizeof(clntAddr);
		ZeroMemory(&clntAddr, szClntAddr);
		strLen = recvfrom(hSocket, message, 200, 0, (LPSOCKADDR)&clntAddr, &szClntAddr);
		sendto(hSocket, message, strLen, 0, (LPSOCKADDR)&clntAddr, szClntAddr);
	}

	closesocket(hSocket);
	WSACleanup();

    return 0;
}

void GetLastSocketError(const char *errorFuncName)
{
	cout << errorFuncName << " Error by " << WSAGetLastError() << endl;
	system("pause");
	exit(-1);
}
