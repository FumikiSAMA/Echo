// EchoServer.cpp: 定义控制台应用程序的入口点。
// 基于TCP协议实现的Echo服务端

#include "stdafx.h"

// 获取上一个Socket错误
void GetLastSocketError(const char *);

int main()
{
	SOCKET hServSock, hClntSock;
	WSADATA wsaData;

	// Winsock启动
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() Error" << endl;
		system("pause");
		exit(-1);
	}

	// 调用socket函数创建TCP套接字
	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hServSock == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(5000);
	// 调用bind函数将服务器套接字和设定的IP地址等信息进行绑定 [此时确定此套接字为服务端套接字]
	if (bind(hServSock, (LPSOCKADDR)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		GetLastSocketError("bind()");

	int listenCount = 5;
	// 调用listen函数开启服务器套接字的监听队列
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
		// 调用accept函数从监听队列中接受客户端与本服务端进行连接
		hClntSock = accept(hServSock, (LPSOCKADDR)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET)
			GetLastSocketError("accept()");
		else
			cout << "Client connected...#" << i << endl;

		// 调用recv函数接收从客户端发来的消息
		while ((recvLen = recv(hClntSock, recvMsg, sizeof(recvMsg), 0)) != 0)
		{
			cout << "Recv message: " << recvMsg << "[recvLen: " << recvLen << "]" << endl;
			memcpy(sendMsg, recvMsg, sizeof(recvMsg));
			// 调用send函数向客户端发送消息
			send(hClntSock, sendMsg, sizeof(sendMsg), 0);
		}
		// 调用closesocket函数关闭客户端套接字
		closesocket(hClntSock);
	}
	// 调用closesocket函数关闭服务端套接字
	closesocket(hServSock);
	// Winsock清空
	WSACleanup();

    return 0;
}

// 获取上一个Socket错误
void GetLastSocketError(const char *errorFuncName)
{
	cout << errorFuncName << " Error by " << WSAGetLastError();
	system("pause");
	exit(-1);
}
