/*
* ʱ��ͬ����������ƣ�client.cpp
* author: sjy
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")  //���붯̬���ӿ�

#define RecvBufLen 4096          //���ջ���������
#define ServerPort 13131      //ʱ��ͬ���������˿ں�
#define ServerIP "127.0.0.1" // ������ip��ַ

int main()
{
	int iResult; //���ڽ�����Ϣ
	char recvBuf[RecvBufLen];

	WORD w_version = MAKEWORD(2, 2);
	WSADATA wsa_data;
	iResult = WSAStartup(w_version, &wsa_data);
	if (iResult != 0)
	{
		cout << WSAGetLastError() << "WSAStartup Error!" << endl;
		return -1;
	}
	if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2)
	{
		cout << WSAGetLastError() << "Version Error!" << endl;
		WSACleanup();
		return -1;
	}

	//�����׽���,���õ�ַ
	SOCKET sock_conn = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_conn == INVALID_SOCKET)
	{
		cout << WSAGetLastError() << "Socket Error!" << endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in addrSrv;
	memset(&addrSrv, 0, sizeof(addrSrv));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(ServerPort);	//Host to Network Short
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ServerIP); 

	//��������
	iResult = connect(sock_conn, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	if (iResult == SOCKET_ERROR)
	{
		cout << WSAGetLastError() << "Connect Error!" << endl;
		closesocket(sock_conn);
		WSACleanup();
		return -1;
	}

	//��ս��ջ�������׼����������
	memset(recvBuf, 0, sizeof(recvBuf));
	cout << "the data received is :" << endl;

	//��������
	do {
		iResult = recv(sock_conn, recvBuf, RecvBufLen, 0);
		if (iResult > 0)
		{
			cout << recvBuf << endl;
		}
		else if (iResult == 0)
		{
			cout << "server is closed" << endl;
		}
		else if (iResult < 0)
		{
			cout << "Recv Error" << endl;
		}
	} while (iResult > 0);

	//�Ͽ����ӣ���������
	closesocket(sock_conn);
	WSACleanup();
	return 0;
}





