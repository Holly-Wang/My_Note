/*
* ��ʽ�׽��ֻ�����������ն������ݣ�server.cpp
* windows10-vs2017
* by sjy
*/
#include <iostream>
#include <Winsock2.h>
#include "stdio.h"
#include <time.h>
#pragma comment(lib, "WS2_32")
#define MAXSIZE 100
using namespace std;

BOOL recvn(SOCKET S, char* buf, unsigned int fixedlen)
{
	BOOL retval = true; //����ֵ
	int iResult; //�洢����recv�����ķ���ֵ
	int cnt; //ͳ��δ�����ֽ���
	cnt = fixedlen;
	ZeroMemory(buf, MAXSIZE);
	while (cnt>0)
	{
		iResult = recv(S, buf, cnt, 0);
		if (iResult == SOCKET_ERROR)
		{
			cout << "����ʧ�ܣ�" << endl;
			return -1;
			break;              // ����ѭ��  
		}
		if (0 == iResult)
		{
			cout << "�Է��ر����ӣ�" << endl;
			return fixedlen - cnt;
		}
		buf += iResult;
		cnt -= iResult;
	}
	return fixedlen;
}

int main() {
	char recvbuf[MAXSIZE];		//��������С
	char sendbuf[MAXSIZE];		//��������С
	WSADATA wsd;		//WSADATA����
	SOCKADDR_IN addr;	//��������ַ
	SOCKADDR_IN Caddr;	//�ͻ��˵�ַ
	SOCKET sockSrv;		//�������׽���

	//��ʼ���׽��ֶ�̬��
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (iResult != 0) {
		cout << "start error��" << endl;
		system("pause");
		return 0;
	}
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wVersion) != 2) {
		cout << "version error��" << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	//�����׽���
	sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	if (sockSrv == INVALID_SOCKET) {
		cout << "create error��" << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	//��������ַ
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(1234);

	//��
	iResult = bind(sockSrv, (SOCKADDR*)& addr, sizeof(SOCKADDR_IN));
	if (iResult == SOCKET_ERROR) {
		cout << "bind error��" << endl;
		closesocket(sockSrv);
		WSACleanup();
		system("pause");
		return 0;
	}

	//�����׽���
	iResult = listen(sockSrv, 3);
	if (iResult == SOCKET_ERROR) {
		cout << "listen error��" << endl;
		closesocket(sockSrv);
		WSACleanup();
		system("pause");
		return 0;
	}

	//���ܿͻ�������
	while (1) {
		cout << "Waiting for client����" << endl;
		SOCKET sockConn;		//�ͻ����׽���
		sockConn = SOCKET_ERROR;
		int len = sizeof(SOCKADDR_IN);
		sockConn = accept(sockSrv, (SOCKADDR*)&Caddr, &len);
		if (sockConn == INVALID_SOCKET) {
			cout << "connect error��" << endl;
			exit(0);
		}
		cout << "client connected��" << inet_ntoa(Caddr.sin_addr) << endl;

		while (true) {
			iResult = recvn(sockConn, recvbuf, 50);
			if (iResult < 0) {
				cout << "recv error��" << endl;
				closesocket(sockConn);
				break;
			}
			if (strlen(recvbuf) != 0 && strlen(sendbuf) != 0) {
				cout << "data received��" << recvbuf << endl;
				strcpy_s(sendbuf, "echo:");
				strcat_s(sendbuf, recvbuf);
				cout << "data sent��" << sendbuf << endl;
				iResult = send(sockConn, sendbuf, strlen(sendbuf), 0);
				if (iResult == SOCKET_ERROR) {
					cout << "send error��";
					closesocket(sockConn);
					break;
				}
			}
		}
		closesocket(sockConn);
	}
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}

