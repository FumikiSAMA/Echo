// EchoClient.cpp: 定义控制台应用程序的入口点。
// 基于TCP协议实现Echo客户端

#include "stdafx.h"

// 获取上一个Socket错误
void GetLastSocketError(const char *);

int main()
{
	SOCKET hSock;
	WSADATA wsaData;

	// Winsock启动
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() Error" << endl;
		system("pause");
		exit(-1);
	}

	// 调用socket函数创建TCP套接字
	hSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSock == INVALID_SOCKET)
		GetLastSocketError("socket()");

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); => 这是老版本的绑定IP的函数 VS会报错 所以改用下面这个新函数 在头文件WS2tcpip.h下
	inet_pton(AF_INET, "127.0.0.1", &(servAddr.sin_addr.S_un.S_addr));
	servAddr.sin_port = htons(5000);
	// 调用connect函数使套接字和绑定了设定的IP地址等信息的主机进行连接 [此时确定此套接字为客户端套接字]
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
		
		// 调用send函数向服务端发送消息
		sendLen = send(hSock, sendMsg, sizeof(sendMsg), 0);
		recvLen = 0;
		while (recvLen < sendLen)
		{
			// 调用recv函数从客户端接收消息
			int recvCount = recv(hSock, &recvMsg[recvLen], sizeof(recvMsg), 0);
			cout << "[RecvCount: " << recvCount << "] [SendLen: " << sendLen << "]" << endl;
			recvLen += recvCount;
		}

		cout << "Message from server: " << recvMsg << endl;
	}

	// 调用closesocket关闭套接字
	closesocket(hSock);
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
