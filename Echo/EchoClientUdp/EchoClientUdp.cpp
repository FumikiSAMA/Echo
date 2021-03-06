// EchoClientUdp.cpp: 定义控制台应用程序的入口点。
// 基于UDP协议实现Echo客户端

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
		cout << "WSAStartup() Error" << endl;
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
	inet_pton(AF_INET, "127.0.0.1", &(servAddr.sin_addr.S_un.S_addr));
	servAddr.sin_port = htons(5000);  // UDP客户端中如果也可以调用connect函数对目标主机进行联机 如果调用了就称为连接套接字 如果没有调用就称为未连接套接字

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
		// 调用sendto函数从函数参数中传递的IP地址等信息所在的主机接收消息
		sendto(hSocket, message, strlen(message), 0, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		// 调用recvfrom函数向函数参数中传递的IP地址等信息所在的主机传递消息
		strLen = recvfrom(hSocket, message, 200, 0, (LPSOCKADDR)&servAddr, &szServAddr);

		cout << "Message from server: " << message << endl;
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
