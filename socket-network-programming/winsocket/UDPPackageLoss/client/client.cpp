#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib") 
using namespace std;
int main()
{
	WSADATA wsaData;
	//���������
	char sendData[256] = "xxxxxxxxxxxxxxxx";
	char recvData[255] = "";
	char num[3] = "";
	int number = 0;
	int count = 1;

	int lens = sizeof(sendData);
	//printf("len=%d\n", lens);
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "start error��" << endl;
		system("pause");
		return 0;
	}
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockClient == SOCKET_ERROR) {
		cout << "create error��" << endl;
		WSACleanup();
		system("pause");
		return 0;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(6000);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int len = sizeof(sin);
	cout << "���ͱ��ĸ���" << endl;
	cin >> num;
	sendto(sockClient, num, strlen(num), 0, (sockaddr *)&sin, len);
	number = atoi(num);
	getchar();
	getchar();
	while (number--)
	{
		cout << "���͵�" << count << "�����ݰ�" << endl;
		sendto(sockClient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);
		count++;
	}
	cout << "��������" << endl;
	closesocket(sockClient);
	WSACleanup();
	system("pause");
	return 0;
}