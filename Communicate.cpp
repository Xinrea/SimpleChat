
#include "stdafx.h"
#include "communicate.h"

#pragma comment(lib,"Ws2_32.lib")


communicate::communicate(WCHAR* ip, const int port)
{
	result_ = WSAStartup(MAKEWORD(2, 2), &wsa_data_);
	if (result_ != NO_ERROR) {
		OutputDebugString(L"WSAStartup() Failed\n");
	}
	connect_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect_socket_ == INVALID_SOCKET) {
		OutputDebugString(L"socket() Failed\n");
		WSACleanup();
	}
	client_service_.sin_family = AF_INET;
	InetPton(AF_INET,ip, &client_service_.sin_addr);
	client_service_.sin_port = htons(port);
}

bool communicate::sconnect()
{
	result_ = connect(connect_socket_, reinterpret_cast<sockaddr*>(&client_service_), sizeof(client_service_));
	if (result_ == SOCKET_ERROR) {
		OutputDebugString(L"connect() Failed\n");
		result_ = closesocket(connect_socket_);
		if (result_ == SOCKET_ERROR) {
			OutputDebugString(L"closesocket() Failed\n");
			WSACleanup();
		}
		return false;
	}
	return true;
}

bool communicate::sendmsg(char* message)
{
	result_ = send(connect_socket_, message, sizeof(message), 0);
	if (result_ == SOCKET_ERROR)return false;
	return true;
}


communicate::~communicate()
{
	result_ = closesocket(connect_socket_);
	if (result_ == SOCKET_ERROR) {
		OutputDebugString(L"closesocket() Failed\n");
	}
	WSACleanup();
}
