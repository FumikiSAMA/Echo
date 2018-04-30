// EchoClient.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void GetLastSocketError(const char *);

int main()
{
	SOCKET hSock;
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() Error" << endl;
		system("pause");
		exit(-1);
	}

	hSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSock == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &(servAddr.sin_addr.S_un.S_addr));
	servAddr.sin_port = htons(5000);
	if (connect(hSock, (LPSOCKADDR)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		GetLastSocketError("connect()");

	char recvMsg[200];
	char sendMsg[200];
	int sendLen = 0, recvLen = 0;
	while (true)
	{
		ZeroMemory(recvMsg, sizeof(recvMsg));
		ZeroMemory(sendMsg, sizeof(sendMsg));

		cout << "Input message (Q to quit): ";
		cin.getline(sendMsg, sizeof(sendMsg));
		cout << "Your input message is: " << sendMsg << endl;

		if (!strcmp(sendMsg, "q\n") || !strcmp(sendMsg, "Q\n"))
			break;

		sendLen = send(hSock, sendMsg, sizeof(sendMsg), 0);
		recvLen = 0;
		while (recvLen < sendLen)
		{
			int recvCount = recv(hSock, &recvMsg[recvLen], sizeof(recvMsg), 0);
			cout << "[RecvCount: " << recvCount << "] [SendLen: " << sendLen << "]" << endl;
			recvLen += recvCount;
		}

		cout << "Message from server: " << recvMsg << endl;
	}

	closesocket(hSock);

	WSACleanup();

    return 0;
}

void GetLastSocketError(const char *errorFuncName)
{
	cout << errorFuncName << " Error by " << WSAGetLastError();
	system("pause");
	exit(-1);
}
