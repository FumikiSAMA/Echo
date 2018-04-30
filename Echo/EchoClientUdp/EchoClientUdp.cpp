// EchoClientUdp.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void GetLastSocketError(const char *);

int main()
{
	SOCKET hSocket;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() Error" << endl;
		system("pause");
		exit(-1);
	}

	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (hSocket == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(servAddr.sin_addr.S_un.S_addr));
	servAddr.sin_port = htons(5000);

	int szServAddr;
	char message[200];
	int strLen;
	while (true)
	{
		ZeroMemory(message, sizeof(message));
		szServAddr = sizeof(servAddr);
		cout << "Insert message: (q to quit): ";
		cin.getline(message, sizeof(message));
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(hSocket, message, strlen(message), 0, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		strLen = recvfrom(hSocket, message, 200, 0, (LPSOCKADDR)&servAddr, &szServAddr);

		cout << "Message from server: " << message << endl;
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
