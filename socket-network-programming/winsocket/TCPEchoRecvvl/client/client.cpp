/*
* ��ʽ�׽��ֻ�����������ն������ݣ�client.cpp
* windows10-vs2017
* by sjy
*/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>
#pragma  comment(lib, "wsock32.lib")
#define  MAXSIZE 100
using namespace std;

int recvn(SOCKET s, char * recvbuf, unsigned int fixedlen);
int recvvl(SOCKET s, char * recvbuf, unsigned int recvbuflen);

int main()
{
	//��ʼ���׽��ֶ�̬�� 
	WSADATA wsaData;
	int retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retVal != 0)
	{
		cout << "start error��" << endl;
		system("pause");
		return  -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		cout << "version error��" << endl;
		system("pause");
		return  -1;
	}
	//�����ͻ����׽��� 
	SOCKET sockclient;
	sockclient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockclient == INVALID_SOCKET)
	{
		WSACleanup();
		cout << "create error��" << endl;
		system("pause");
		return  -1;
	}
	//��������ַ��Ϣ  
	struct sockaddr_in saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_port = htons(6000);
	saServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//���ӷ�����  
	retVal = connect(sockclient, (sockaddr*)&saServer, sizeof(saServer));
	if (retVal == SOCKET_ERROR)
	{
		cout << "connect error��" << endl;
		closesocket(sockclient);
		WSACleanup();
		system("pause");
		return  -1;
	}
	else
		cout << "connect successfully��" << endl;

	//���������������
	//���ȷ��Ͷ�������Ϣ�������δ������Ϣ����  
	//�ٷ��ͱ䳤����Ϣ��
	unsigned int slen = 0;
	unsigned int buflen = MAXSIZE;
	char buff[MAXSIZE];  //���ͻ���  
	char recvbuff[MAXSIZE]; //���ջ���  
	while (TRUE)
	{
		memset(recvbuff, 0, MAXSIZE);
		cout << "Please input data��";
		cin >> buff;
		if (strcmp(buff, "q") == 0) { 
			cout << "finish input" << endl;
			break;
		}
			
		slen = (unsigned int)strlen(buff);
		// cout << slen << endl;
		slen = htonl(slen);

		//�ȷ�����
		if (send(sockclient, (char*)&slen, sizeof(unsigned int), 0) <= 0)
		{
			printf("����ʧ��!code:%d\n", WSAGetLastError());
			closesocket(sockclient);
			WSACleanup();
			system("pause");
			return  -1;
		}
		//�ٷ��䳤��Ϣ��
		if (send(sockclient, buff, strlen(buff), 0) <= 0)
		{
			printf("����ʧ�� !code:%d\n", WSAGetLastError());
			closesocket(sockclient);
			WSACleanup();
			return  -1;
		}

		retVal = recvvl(sockclient, recvbuff, buflen);
		if (retVal == SOCKET_ERROR)
		{
			closesocket(sockclient);
			WSACleanup();
			system("pause");
			return  -1;
		}

		//��ӡ�յ��Ļ����ַ���  
		cout << recvbuff << endl;
	}
	closesocket(sockclient);
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
			system("pause");
			return  -1;
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
	//printf("reclen:%d\n",reclen);  
	if (iResult != sizeof(unsigned int)) {
		//��������ֶ��ڽ���ʱû�з���һ���������ݾͷ��أ����ӹرգ���-1����������  
		if (iResult == -1) {
			printf("���շ�������: %d\n", WSAGetLastError());
			system("pause");
			return  -1;
		}
		else {
			printf("���ӹر�\n");
			system("pause");
			return 0;
		}
	}
	//ת�������ֽ�˳�������ֽ�˳��  
	reclen = ntohl(reclen);
	if (reclen > recvbuflen) {
		//���recvbufû���㹻�Ŀռ�洢�䳤��Ϣ������ո���Ϣ�����������ش���  
		while (reclen > 0) {
			iResult = recvn(s, recvbuf, recvbuflen);
			if (iResult != recvbuflen) {
				//����䳤��Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������  
				if (iResult == -1) {
					printf("���շ�������: %d\n", WSAGetLastError());
					system("pause");
					return  -1;
				}
				else {
					printf("���ӹر�\n");
					system("pause");
					return 0;
				}
			}
			reclen -= recvbuflen;
			//�������һ�����ݳ���  
			if (reclen < recvbuflen)
				recvbuflen = reclen;
		}
		printf("�ɱ䳤�ȵ���Ϣ����Ԥ����Ľ��ջ���\r\n");
		system("pause");
		return  -1;
	}
	//���տɱ䳤��Ϣ  
	iResult = recvn(s, recvbuf, reclen);
	if (iResult != reclen) {
		//�����Ϣ�ڽ���ʱû�з����㹻�����ݾͷ��أ����ӹرգ���-1����������  
		if (iResult == -1) {
			printf("���շ�������: %d\n", WSAGetLastError());
			system("pause");
			return  -1;
		}
		else {
			printf("���ӹر�\n");
			system("pause");
			return 0;
		}
	}
	return iResult;
}

