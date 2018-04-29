// EchoServer.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void GetLastSocketError(const char *);

int main()
{
	SOCKET hServSock, hClntSock;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() Error" << endl;
		system("pause");
		exit(-1);
	}

	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hServSock == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(5000);
	if (bind(hServSock, (LPSOCKADDR)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		GetLastSocketError("bind()");

	int listenCount = 5;
	if (listen(hServSock, listenCount) == SOCKET_ERROR)
		GetLastSocketError("listen()");

	char recvMsg[200];
	char sendMsg[200];
	int recvLen = 0;
	for (int i = 1; i <= listenCount; ++i)
	{
		ZeroMemory(recvMsg, sizeof(recvMsg));
		ZeroMemory(sendMsg, sizeof(sendMsg));
		recvLen = 0;

		SOCKADDR_IN clntAddr;
		ZeroMemory(&clntAddr, sizeof(clntAddr));
		int szClntAddr = sizeof(clntAddr);
		hClntSock = accept(hServSock, (LPSOCKADDR)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET)
			GetLastSocketError("accept()");
		else
			cout << "Client connected...#" << i << endl;

		while ((recvLen = recv(hClntSock, recvMsg, sizeof(recvMsg), 0)) != 0)
		{
			cout << "Recv message: " << recvMsg << "[recvLen: " << recvLen << "]" << endl;
			memcpy(sendMsg, recvMsg, sizeof(recvMsg));
			send(hClntSock, sendMsg, sizeof(sendMsg), 0);
		}

		closesocket(hClntSock);
	}

	closesocket(hServSock);

	WSACleanup();

    return 0;
}

void GetLastSocketError(const char *errorFuncName)
{
	cout << errorFuncName << " Error by " << WSAGetLastError();
	system("pause");
	exit(-1);
}
