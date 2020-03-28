#include<Winsock2.h>
#include<iostream>
#include <windows.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

int port = 10000;

DWORD WINAPI IOThread(LPVOID lpParam) {
	//����udp�����ӣ�ÿһ�ε����̶߳�Ҫ���°�
	cout << endl;
	char recvbuf[576];		//���ջ�������С
	char sendbuf[576];		//���ͻ�������С
	SOCKADDR_IN sinaddr;
	SOCKET  clientSocket;	//�ͻ����׽���
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == SOCKET_ERROR) {
		cout << "create socket error" << endl;
		closesocket(clientSocket);
		return 0;
	}

	//���õ�ַ�����ж˿����������в��ϵ������õĶ˿ں�
	sinaddr.sin_family = AF_INET;
	sinaddr.sin_addr.s_addr = INADDR_ANY;
	sinaddr.sin_port = htons(port);
	cout << "����˿ں�Ϊ�� " << port << endl;

	int len = sizeof(SOCKADDR_IN);

	//���׽���
	int iResult = bind(clientSocket, (SOCKADDR*)&sinaddr, sizeof(SOCKADDR_IN));
	if (iResult == SOCKET_ERROR) {
		cout << "bind error" << endl;
		closesocket(clientSocket);
		system("pause");
		return 0;
	}

	while (1) {
		//���̣߳�Ҳ���Ƚ����ٷ���
		iResult = recvfrom(clientSocket, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&sinaddr, &len);
		if (iResult == SOCKET_ERROR) {
			cout << "recvfrom error" << endl;
			break;
		}
		cout << "data received:" <<recvbuf << endl;

		sprintf_s(sendbuf, "echo : %s", recvbuf);
		iResult = sendto(clientSocket, sendbuf, strlen(sendbuf) + 1, 0, (SOCKADDR*)&sinaddr, len);
		if (iResult == SOCKET_ERROR) {
			cout << "sendtoerror" << endl;
			break;
		}
		cout << "data sent:" << sendbuf << endl;
	}
	closesocket(clientSocket);
	system("pause");
	return 0;
}

int main() {
	WSADATA wsd;
	SOCKET sockSrv;
	//SOCKET datasocket;
	SOCKADDR_IN addrSrv;
	char recvbuf[576];
	char sendbuf[576];
	HANDLE  hThread;	//���߳�
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (result != 0) {
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
	sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockSrv == SOCKET_ERROR) {
		cout << "�����׽���ʧ�ܣ�" << endl;
		WSACleanup();
		system("pause");
		return 0;
	}

	//ָ����ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = INADDR_ANY;
	addrSrv.sin_port = htons(10000);

	//���׽���
	result = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR_IN));
	if (result == SOCKET_ERROR) {
		cout << "bind error��" << endl;
		closesocket(sockSrv);
		WSACleanup();
		system("pause");
		return 0;
	}

	SOCKADDR_IN addrClt;
	int len = sizeof(SOCKADDR_IN);
	char temp[1000];

	while (true) {
		cout << "waiting for the data" << endl;
		//��������recvһ�Σ��õ�������Ϣ��������˿�
		int recvs = recvfrom(sockSrv, recvbuf, sizeof(recvbuf), 0, (SOCKADDR*)&addrClt, &len);
		if (recvs == SOCKET_ERROR) {
			cout << "recvfrom error" << endl;
			closesocket(sockSrv);
		}

		//����˿�
		port++;
		sprintf_s(sendbuf, "%d", port);
		int sed = sendto(sockSrv, sendbuf, strlen(sendbuf) + 1, 0, (SOCKADDR*)&addrClt, len);
		if (sed == SOCKET_ERROR) {
			cout << "send error" << endl;
			closesocket(sockSrv);
		}
		//���̵߳Ĵ���
		hThread = CreateThread(NULL, 0, IOThread, (LPVOID)sockSrv, 0, NULL);
		if (hThread == NULL)
		{
			cout << "new thread error��" << endl;
		}
		else
		{
			cout << "new thread success��" << endl;
		}
		Sleep(1000);
	}
	closesocket(sockSrv);
	WSACleanup();
	system("pause");
	return 0;
}
