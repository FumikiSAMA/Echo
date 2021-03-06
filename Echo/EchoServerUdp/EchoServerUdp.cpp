// EchoServerUdp.cpp: 定义控制台应用程序的入口点。
// 基于UDP协议实现Echo服务端

#include "stdafx.h"

// 获取上一个Socket错误
void GetLastSocketError(const char *);

int main()
{
	SOCKET hSocket;
	WSADATA wsaData;

	// Winsock启动
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup Error" << endl;
		system("pause");
		exit(-1);
	}

	// 调用socket函数创建UDP套接字
	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (hSocket == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(5000);
	// 调用bind函数将服务器套接字和设定的IP地址等信息进行绑定
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
		// 调用recvfrom函数向函数参数中传递的IP地址等信息所在的主机传递消息
		strLen = recvfrom(hSocket, message, 200, 0, (LPSOCKADDR)&clntAddr, &szClntAddr);
		// 调用sendto函数从函数参数中传递的IP地址等信息所在的主机接收消息
		sendto(hSocket, message, strLen, 0, (LPSOCKADDR)&clntAddr, szClntAddr);
	}

	// 调用closesocket函数关闭套接字
	closesocket(hSocket);
	// Winsock清空
	WSACleanup();

    return 0;
}

// 获取上一个Socket错误
void GetLastSocketError(const char *errorFuncName)
{
	cout << errorFuncName << " Error by " << WSAGetLastError() << endl;
	system("pause");
	exit(-1);
}
