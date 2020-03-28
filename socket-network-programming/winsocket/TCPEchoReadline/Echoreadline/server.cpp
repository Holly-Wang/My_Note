/*
* ��ʽ�׽��ֻ������������һ�����ݣ�server.cpp
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

BOOL recvline(SOCKET S, char* buf)
{
	BOOL retval = true; //����ֵ
	BOOL bLineEnd = false;//һ�ж�ȡ����
	int nReadLen = 0; //�����ֽ���
	int nDataLen = 0; //���ݳ���

	while (!bLineEnd)
	{
		nReadLen = recv(S, buf, MAXSIZE, 0);
		if (nReadLen == SOCKET_ERROR)
		{
			cout << "����ʧ�ܣ�" << endl;
			int nErrCode = WSAGetLastError();   // ��ȡ�������  
			retval = false;   // ��ȡ����ʧ��  
			break;              // ����ѭ��  
		}
		if (0 == nReadLen)
		{
			retval = false; //��ȡ����ʧ��
			break;
		}
		for (int i = 0; i<nReadLen; i++)
		{
			if ('\n' == *(buf + i)) {
				bLineEnd = true;
				retval = true;
				break;
			}

		}
	}
	return retval;

}


int main() {
	char recvbuf[MAXSIZE];		//��������С
	char sendbuf[MAXSIZE];		//��������С
	WSADATA wsd;		//WSADATA����
	SOCKADDR_IN addr;	//��������ַ
	SOCKADDR_IN Caddr;	//�ͻ��˵�ַ
	SOCKET serverSocket;		//�������׽���

	//��ʼ���׽��ֶ�̬��
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (iResult != 0) {
		cout << "start error��" << endl;
		return 0;
	}
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wVersion) != 2) {
		cout << "version error" << endl;
		WSACleanup();
		return 0;
	}

	//�����׽���
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		cout << "socket create error��" << endl;
		WSACleanup();
		return 0;
	}

	//ָ����������ַ
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(12345);

	//��
	iResult = bind(serverSocket, (SOCKADDR*)& addr, sizeof(SOCKADDR_IN));
	if (iResult == SOCKET_ERROR) {
		cout << "bind error��" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}

	//�����׽���
	iResult = listen(serverSocket, 3);
	if (iResult == SOCKET_ERROR) {
		cout << "listen error��" << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}

	//���ܿͻ�������
	while (1) {
		cout << "Waiting for the client����" << endl;
		SOCKET clientSocket;		//�ͻ����׽���
		clientSocket = SOCKET_ERROR;
		int len = sizeof(SOCKADDR_IN);
		clientSocket = accept(serverSocket, (SOCKADDR*)&Caddr, &len);
		if (clientSocket == INVALID_SOCKET) {
			cout << "connect error��" << endl;
			return 0;
		}

		//inet_ntoa��ʾ��ip��ʾΪ���ʮ������ʽ
		cout << "client connected is��" << inet_ntoa(Caddr.sin_addr) << endl;
		while (1) {
			if (!recvline(clientSocket, recvbuf)) {
				cout << "recv error��" << endl;
				closesocket(clientSocket);
				break;
			}
			cout << "data received��" << recvbuf << endl;
			strcpy_s(sendbuf, "echo:");
			strcat_s(sendbuf, recvbuf);
			cout << "data sent��" << sendbuf << endl;
			iResult = send(clientSocket, sendbuf, strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR) {
				cout << "send error��" << endl;
				closesocket(clientSocket);
				break;
			}
		}
		closesocket(clientSocket);
	}
	closesocket(serverSocket);
	WSACleanup();

	return 0;
}

