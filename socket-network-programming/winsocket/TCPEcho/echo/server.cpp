/*
* ��ʽ�׽��ֻ����������server.cpp
* windows10-vs2017
* by sjy
*/

#include "stdafx.h"
#include<iostream>
#include<string>
#include<Winsock2.h>

#pragma comment(lib,"WS2_32")
using namespace std;


int main()
{
	//��ʼ��
	WORD w_version;
	WSADATA wsaData;
	int iResult;  //���ڽ�����Ϣ
	w_version = MAKEWORD(1, 1);
	iResult = WSAStartup(w_version, &wsaData);
	if (iResult != 0)
	{
		cout << "Start Error��" << endl;
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		cout << "Version Error��" << endl;
		WSACleanup();
		return 0;
	}

	//�����׽���,���õ�ַ
	SOCKET sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(1234);

	//�󶨵�ַ
	bind(sock_listen, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	//�����ͻ��ˣ��������г�������Ϊ5
	listen(sock_listen, 5);

	//�ӿͻ��˽��ܵĵ�ַ��Ϣ
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	cout << "waiting for client to connect:" << endl;
	SOCKET AcceptSocket;
	while (1)
	{
		//accept
		SOCKET sock_conn = accept(sock_listen, (SOCKADDR*)&addrClient, &len);
		if (sock_conn == INVALID_SOCKET)
		{
			cout << WSAGetLastError() << "Accept Error!" << endl;
			closesocket(sock_conn);
			WSACleanup();
			return -1;
		}
		cout << "connect to the client successfully" << endl;

		//���ͺͽ�������
		char sendBuf[100];
		char recvBuf[100];
		while (1)
		{
			iResult = recv(sock_conn, recvBuf, 100, 0);
			if (iResult == SOCKET_ERROR)
			{
				cout << WSAGetLastError() << "Receive Error!" << endl;
				closesocket(sock_conn);
				WSACleanup();
				return -1;
			}
			cout << "string received from client : " << recvBuf << endl;
			sprintf(sendBuf, "echo:%s", recvBuf);	//��recvBuf�����ݸ�ʽ�������sendBuf��
			iResult = send(sock_conn, sendBuf, strlen(sendBuf) + 1, 0);
			if (iResult == SOCKET_ERROR)
			{
				cout << WSAGetLastError() << "Send Error!" << endl;
				closesocket(sock_conn);
				WSACleanup();
				return -1;
			}
			cout << "data send to client : " << sendBuf << endl;
		}
		cout << "connect is closed" << endl;
		closesocket(sock_conn);

	}
	closesocket(sock_listen);
	WSACleanup();
	return 0;
}
