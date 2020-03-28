#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib") 
using namespace std;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	char recvData[255] = "";
	char sendData[1024] = "";
	char number[3] = "";
	int size = 0;	//��������С
	int receive = 0;	//���յ�����
	int cnt = 0;	//��¼���Խ��յİ�����
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "start error" << endl;
		system("pause");
		return 0;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockSrv == INVALID_SOCKET)
	{
		cout << "create error !" << endl;
		system("pause");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(6000);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(sockSrv, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		cout << "bind error !" << endl;
		closesocket(sockSrv);
		system("pause");
		return 0;
	}

	sockaddr_in addrClient;

	int nAddrLen = sizeof(addrClient);
	int ret = recvfrom(sockSrv, number, 3, 0, (sockaddr *)&addrClient, &nAddrLen);
	if (ret > 0)
	{
		recvData[ret] = 0x00;
		printf("�ͻ�Ϊ��%s \r\n", inet_ntoa(addrClient.sin_addr));
	}
	cout << "�ͻ����ͱ��ĸ���Ϊ�� " << number << endl;

	//���ϵͳĬ�ϵĻ�������С
	int len = sizeof(size);

	//�ַ���ת��Ϊ����
	int n = atoi(number);
	cout << "���÷������˵Ľ��ջ������ֽ���" << endl;
	cin >> size;

	//�׽���ѡ�������ý��ջ�������С
	if (setsockopt(sockSrv, SOL_SOCKET, SO_RCVBUF, (const char *)&size, len) < 0)
	{
		cout << "�����׽�����Ϣʧ��" << endl;
		return -1;
	}
	if (getsockopt(sockSrv, SOL_SOCKET, SO_RCVBUF, (char *)&size, &len) < 0)
	{
		cout << "��ȡ�׽�����Ϣʧ��" << endl;
		return -1;
	}
	cout << "���ú��ϵͳ���ջ����С: " << size << endl;

	int time; int tlen = sizeof(time);
	cout << "���÷���˵ĳ�ʱʱ�䣨ms��" << endl;
	cin >> time;

	//�׽���ѡ�������ó�ʱʱ��
	if (setsockopt(sockSrv, SOL_SOCKET, SO_RCVTIMEO, (const char *)&time, tlen) < 0)
	{
		cout << "�����׽�����Ϣʧ��" << endl;
		return -1;
	}
	if (getsockopt(sockSrv, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, &tlen) < 0)
	{
		cout << "��ȡ�׽�����Ϣʧ��" << endl;
		return -1;
	}
	cout << "���ú��ϵͳ���ճ�ʱʱ��: " << time << endl;

	int sleepTime;
	cout << "��������պ��ӳ�ʱ�䣺";
	cin >> sleepTime;
	cout << "���ò�����ɣ��ȴ�����................" << endl;

	while (1)
	{
		int ret = recvfrom(sockSrv, recvData, size, 0, (sockaddr *)&addrClient, &nAddrLen);
		cnt++;
		//����0����ɹ�����
		if (ret>0){
			receive++;
			cout << "��" << receive << "�������ճɹ�" << endl;
		}
		//���а������Խ�����
		if (cnt == n)
			break;
		Sleep(sleepTime);
	}

	//����������
	cout << "һ����������" << n << endl;
	cout << "�յ�������Ϊ" << receive << endl;
	float t = 1 - ((float)receive / (float)n);
	cout << "������" << t << endl;
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}

