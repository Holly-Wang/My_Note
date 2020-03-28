/*
* ��ʽ�׽��ֻ�����������ն������ݣ�server.cpp
* windows10-vs2017
* by sjy
*/
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <iostream>
#pragma comment(lib, "wsock32.lib")
#define  SERVERPROT 6000  
#define  MAXSIZE 100  

int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen);
int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen);
using namespace std;

int main()
{
	//��ʼ��WinSock  
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "start error��" << endl;
		return  -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		cout << "WSA�汾����" << endl;
		return  -1;
	}
	//���������׽���  
	SOCKET slisten;
	slisten = socket(AF_INET, SOCK_STREAM, 0);
	if (slisten == INVALID_SOCKET)
	{
		cout << "create error��" << endl;
		WSACleanup();
		return  -1;
	}
	//�������������ص�ַ��Ϣ  
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPROT);
	serveraddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//��  
	iResult = bind(slisten, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (iResult == SOCKET_ERROR)
	{
		cout << "bind error��" << endl;
		closesocket(slisten);
		WSACleanup();
		return  -1;
	}
	//������������  
	iResult = listen(slisten, 5);
	if (iResult == SOCKET_ERROR)
	{
		cout << "listen error��" << endl;
		closesocket(slisten);
		WSACleanup();
		return  -1;
	}
	while (1) {
		cout << "Waiting for client����" << endl;

		//���տͻ�������  
		struct sockaddr_in clientaddr;
		int clientaddrlen = sizeof(clientaddr);
		SOCKET sServer;//�����׽���  
		char buff[MAXSIZE];//���ջ���  
		char sendbuf[MAXSIZE];//���ͻ���  
		unsigned int slen = 0; //�������ݳ���  
		unsigned int buflen = MAXSIZE;

		sServer = accept(slisten, (struct sockaddr*)&clientaddr, &clientaddrlen);
		if (sServer == INVALID_SOCKET)
		{
			cout << "connect error��" << endl;
			closesocket(slisten);
			WSACleanup();
			return  -1;
		}
		cout << "client connected��" << inet_ntoa(clientaddr.sin_addr) << endl;

		while (TRUE)  //ѭ����������  
		{
			memset(buff, 0, MAXSIZE);
			memset(sendbuf, 0, MAXSIZE);
			iResult = recvvl(sServer, buff, buflen);
			if (iResult == SOCKET_ERROR)
			{
				closesocket(sServer);
				break;
			}
			sprintf_s(sendbuf, "echo:%s", buff);
			cout << sendbuf << endl;
			slen = (unsigned int)strlen(sendbuf);
			slen = htonl(slen);
			iResult = send(sServer, (char*)&slen, sizeof(unsigned int), 0);
			if (iResult == SOCKET_ERROR)
			{
				closesocket(sServer);
				break;
			}
			iResult = send(sServer, sendbuf, strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR)
			{
				closesocket(sServer);
				break;
			}
		}
		closesocket(sServer);
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
}

//ָ�����Ƚ���  
int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen)
{
	int iResult;    //�洢����recv�����ķ���ֵ  
	int cnt;         //����ͳ������ڹ̶����ȣ�ʣ������ֽ���δ����  
	cnt = fixedlen;
	while (cnt > 0) {
		iResult = recv(s, recvbuf, cnt, 0);
		if (iResult < 0) {
			//���ݽ��ճ��ִ��󣬷���ʧ��  
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		if (iResult == 0) {
			//�Է��ر����ӣ������ѽ��յ���С��fixedlen���ֽ���  
			printf("���ӹر�\n");
			return fixedlen - cnt;
		}
		//���ջ���ָ������ƶ�  
		recvbuf += iResult;
		//����cntֵ  
		cnt -= iResult;
	}
	return fixedlen;
}

int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen)
{
	int iResult;//�洢����recvn�����ķ���ֵ  
	unsigned int reclen; //���ڴ洢����ͷ���洢�ĳ�����Ϣ  
						 //��ȡ���ձ��ĳ�����Ϣ  
	iResult = recvn(s, (char *)&reclen, sizeof(unsigned int));
	if (iResult != sizeof(unsigned int))
	{
		//��������ֶ��ڽ���ʱû�з���һ���������ݾͷ��أ����ӹرգ���-1����������  
		if (iResult == -1) {
			printf("���շ�������: %d\n", WSAGetLastError());
			return -1;
		}
		else {
			printf("���ӹر�\n");
			return 0;
		}
	}
	//ת�������ֽ�˳�������ֽ�˳��  
	reclen = ntohl(reclen);
	if (reclen > recvbuflen)
	{
		printf("reclen>recvbuflen: %d>%d\n", reclen, recvbuflen);
		//���recvbufû���㹻�Ŀռ�洢�䳤��Ϣ������ո���Ϣ�����������ش���  
		while (reclen > 0) {
			iResult = recvn(s, recvbuf, recvbuflen);
			if (iResult != recvbuflen) {
				//����䳤��Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������  
				if (iResult == -1) {
					printf("���շ�������: %d\n", WSAGetLastError());
					return -1;
				}
				else {
					printf("���ӹر�\n");
					return 0;
				}
			}
			reclen -= recvbuflen;
			//�������һ�����ݳ���  
			if (reclen < recvbuflen)
				recvbuflen = reclen;
		}
		printf("�ɱ䳤�ȵ���Ϣ����Ԥ����Ľ��ջ���\r\n");
		return -1;
	}
	//���տɱ䳤��Ϣ  
	iResult = recvn(s, recvbuf, reclen);
	if (iResult != reclen)
	{
		//�����Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������  
		if (iResult == -1) {
			printf("���շ�������: %d\n", WSAGetLastError());
			return  -1;
		}
		else {
			printf("���ӹر�\n");
			return 0;
		}
	}
	return iResult;
}
