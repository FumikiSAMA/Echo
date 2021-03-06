// GetHost.cpp: 定义控制台应用程序的入口点。
// 练习gethostbyname和gethostbyarrd函数

#include "stdafx.h"

// 运行gethostbyname函数
void RunGetHostByName();
// 运行gethostbyaddr函数
void RunGetHostByAddr();
// 获取上一个Socket错误
void ErrorHandling(const char *);

int main()
{
	RunGetHostByName();
	RunGetHostByAddr();
    return 0;
}

void RunGetHostByName()
{
	WSADATA wsaData;

	// Winsock启动
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() error" << endl;
		exit(-1);
	}

	char webSite[50];
	ZeroMemory(webSite, sizeof(webSite));
	cout << "请输入一个网址：";
	cin.getline(webSite, sizeof(webSite));

	hostent *host = new hostent();
	// 调用gethostbyname函数利用域名获取IP地址等信息
	host = gethostbyname(webSite);

	cout << "[Official name] " << host->h_name << endl;
	for (int i = 0; host->h_aliases[i]; ++i)
		cout << "[Alias list #" << i + 1 << "] " << host->h_aliases[i] << endl;
	cout << "[Host address type] " << ((host->h_addrtype == 2) ? "AF_INET" : "AF_INET6") << endl;
	cout << "[Length of address] " << host->h_length << endl;
	for (int i = 0; host->h_addr_list[i]; ++i)
		cout << "[List of addresses #" << i + 1 << "] " << inet_ntoa(*(in_addr *)host->h_addr_list[i]) << endl;

	system("pause");
	// Winsock清空
	WSACleanup();
}

void RunGetHostByAddr()
{
	WSADATA wsaData;

	// Winsock启动
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cout << "WSAStartup() error" << endl;
		exit(-1);
	}

	char ipAddr[20];
	ZeroMemory(ipAddr, sizeof(ipAddr));
	cout << "请输入一段IPv4地址（点分十进制）：";
	cin.getline(ipAddr, sizeof(ipAddr));

	hostent *host = new hostent();
	// 调用gethostbyaddr函数利用IP地址获取域名等信息
	host = gethostbyaddr(ipAddr, 4, AF_INET);
	if (!host)
	{
		cout << "Get host error. Host is nullptr." << endl;
		system("pause");
		exit(-1);
	}

	cout << "[Official name] " << host->h_name << endl;
	for (int i = 0; host->h_aliases[i]; ++i)
		cout << "[Alias list #" << i + 1 << "] " << host->h_aliases[i] << endl;
	cout << "[Host address type] " << ((host->h_addrtype == 2) ? "AF_INET" : "AF_INET6") << endl;
	cout << "[Length of address] " << host->h_length << endl;
	for (int i = 0; host->h_addr_list[i]; ++i)
		cout << "[List of addresses #" << i + 1 << "] " << inet_ntoa(*(in_addr *)host->h_addr_list[i]) << endl;

	system("pause");
	// Winsock清空
	WSACleanup();
}

// 获取上一个Socket错误
void ErrorHandling(const char *errorFuncName)
{
	cout << errorFuncName << " Error by " << WSAGetLastError() << endl;
	system("pause");
	exit(-1);
}