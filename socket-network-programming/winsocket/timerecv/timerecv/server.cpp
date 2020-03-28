/*
* ʱ��ͬ����������ƣ�server.cpp
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

#define ListenQ 1024		//��������
#define	RecvBufLen 4096		//���ջ�����
#define ServerPort 13131	//ʱ��ͬ���������˿�

int main()
{
	time_t t;	//ʱ�����ͣ���ʵ�ǳ�����
	char sendBuf[100];	//���ͻ�����
	char recvBuf[RecvBufLen];	//���ջ�����
	int iResult;	//������Ϣ��
	
	//��ʼ��windows Socket DLL �汾Э���
	WORD w_version = MAKEWORD(2, 2);
	WSADATA wsaData;
	iResult = WSAStartup(w_version, &wsaData);
	if (iResult != 0)
	{
		cout << WSAGetLastError() << "WSAStartup Error!" << endl;
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		cout << WSAGetLastError() << "Version Error!" << endl;
		WSACleanup();
		return -1;
	}

	//�����׽���,�����������׽���
	SOCKET sock_conn = socket(AF_INET, SOCK_STREAM, 0);
	SOCKET sock_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_listen == INVALID_SOCKET)
	{
		cout << WSAGetLastError() << "Socket Error";
		WSACleanup();
		return -1;
	}

	//���õ�ַ
	sockaddr_in addrSrv;
	memset(&addrSrv, 0, sizeof(addrSrv));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(ServerPort);	//Host to Network Short
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //Host to Network Long��INADDR_ANY����0.0.0.0 ͨ��

	//Ϊ�����׽��ְ󶨵�ַ
	iResult = bind(sock_listen, (struct sockaddr *)&addrSrv, sizeof(addrSrv));
	if (iResult == SOCKET_ERROR)
	{
		cout << WSAGetLastError() << "Bind Error" << endl;
		closesocket(sock_listen);
		WSACleanup();
		return -1;
	}
	
	//�����ͻ���
	iResult = listen(sock_listen, ListenQ);
	if (iResult == SOCKET_ERROR)
	{
		cout << WSAGetLastError() << "Listen Error!" << endl;
		closesocket(sock_listen);
		WSACleanup();
		return -1;
	}

	//ѭ�������������û���������
	while (true)
	{
		//����accept
		sock_conn = accept(sock_listen, NULL, NULL);
		if (sock_conn == INVALID_SOCKET)
		{
			cout << WSAGetLastError() << "Accept Error!" << endl;
			closesocket(sock_conn);
			WSACleanup();
			return -1;
		}

		//��ȡʱ��
		t = time(NULL);
		time(&t);
		memset(sendBuf, 0, sizeof(sendBuf)); //��շ��ͻ�������׼������
		strcpy(sendBuf, ctime(&t)); //��ʱ��ת��Ϊ�ַ�����ʽ
		cout << "time:" << sendBuf << endl;

		//����ʱ��
		iResult = send(sock_conn, sendBuf, (int)strlen(sendBuf), 0);
		if (iResult == SOCKET_ERROR)
		{
			cout << WSAGetLastError() << "Send Error!" << endl;
			closesocket(sock_conn);
			WSACleanup();
			return -1;
		}
		cout << "send time information to client successfully" << endl;

		//�رշ���ͨ�������ٷ�������
		iResult = shutdown(sock_conn, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			cout << WSAGetLastError() << "Shutdown Error!" << endl;
			closesocket(sock_conn);
			WSACleanup();
			return -1;
		}


		//�ͷ������׽���
		closesocket(sock_conn);
		cout << "Server close" << endl;


		//�ͷż����׽���
		closesocket(sock_listen);
		WSACleanup();
		return 0;
	}
}