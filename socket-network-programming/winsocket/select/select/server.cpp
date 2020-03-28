#include<winsock2.h>
#include<stdio.h>
#include<iostream>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

#define tcpPort 10000
#define udpPort 11000

int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int iResult;
	HANDLE hThread;
	char recvBuf[100];
	char sendBuf[100];
	//Ԥ�ȴ���tcp�����׽���
	SOCKET tcpConnSock = INVALID_SOCKET;

	//��ʼ����̬���ӿ�
	wVersionRequested = MAKEWORD(1, 1);
	iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0)
	{
		cout << "start error" << endl;
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		cout << "version error" << endl;
		WSACleanup();
		return -1;
	}

	//tcp���ڼ������׽���
	SOCKET tcpServerSock = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpServerSock == INVALID_SOCKET)
	{
		cout << "SOCKET tcp error" << endl;
		WSACleanup();
		return -1;
	}

	//���õ�ַ Ϊtcp
	SOCKADDR_IN addrSrvTCP;
	addrSrvTCP.sin_family = AF_INET;
	addrSrvTCP.sin_port = htonl(tcpPort);
	addrSrvTCP.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	int lenSrv = sizeof(SOCKADDR_IN);

	//�󶨵�ַ tcp
	iResult = bind(tcpServerSock, (struct sockaddr*)&addrSrvTCP, sizeof(addrSrvTCP));

	//���� tcp
	iResult = listen(tcpServerSock, 5);

	//����udp�׽���
	SOCKET udpServerSock = socket(AF_INET, SOCK_DGRAM, 0);

	//���õ�ַ Ϊudp
	SOCKADDR_IN addrSrvUDP;
	addrSrvUDP.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrvUDP.sin_family = AF_INET;
	addrSrvUDP.sin_port = htonl(udpPort);
	iResult = bind(udpServerSock, (struct sockaddr*)&addrSrvUDP, sizeof(addrSrvUDP));

	cout << "tcp�˿�Ϊ" << tcpPort << endl;
	cout << "udp�˿�Ϊ" << udpPort << endl;
	
	//�����׽�����
	fd_set fdRead, fdSocket;
	FD_ZERO(&fdSocket);
	FD_SET(tcpServerSock, &fdSocket);
	FD_SET(udpServerSock, &fdSocket);

	//��ʼ����
	while (true)
	{
		fdRead = fdSocket;
		//����ģʽ����ʱʱ��Ҳ��������ΪNULL
		iResult = select(0, &fdRead, NULL, NULL, NULL);
		//�����򷵻�һ������0������������Ƿ��������¼����׽����ܺ�
		if (iResult > 0)
		{
			//�������¼�������ȷ������Щ�׽�����δ����IO������һ������
			for (int i = 0; i < (int)fdSocket.fd_count; i++)
			{
				if (FD_ISSET(fdSocket.fd_array[i], &fdRead))
				{
					//tcp�����׽�������������
					if (fdSocket.fd_array[i] == tcpServerSock)
					{
						if (fdSocket.fd_count < FD_SETSIZE)
						{
							tcpConnSock = accept(tcpServerSock, (struct sockaddr*)&addrSrvTCP, &lenSrv);

							//��tcp�����׽����ٴμ����׽�����
							FD_SET(tcpConnSock, &fdSocket);
							cout << "�յ��µ���������" << inet_ntoa(addrSrvTCP.sin_addr);
						}
						else
						{
							cout << "������������" << endl;
							continue;
						}
					}
					//��tcp�����׽��ַ����¼�
					else if (fdSocket.fd_array[i] == tcpConnSock)
					{
						cout << "tcp���ӡ�������Ϊ" << inet_ntoa(addrSrvTCP.sin_addr) << endl;
						memset(recvBuf, 0, 100);
						iResult = recv(fdSocket.fd_array[i], recvBuf, 100, 0);
						//����ͬ����ֵ
						//����0�ɹ�����
						if (iResult > 0)
						{
							cout << recvBuf << endl;
							sprintf(sendBuf, "echo: %s", recvBuf);
							cout << sendBuf << endl;
							iResult = send(fdSocket.fd_array[i], sendBuf, 100, 0);
						}
						//����0���ӹر�
						else if (iResult == 0)
						{
							cout << "���ӹر���......" << endl;
							closesocket(fdSocket.fd_array[i]);
							FD_CLR(fdSocket.fd_array[i], &fdSocket);
						}
						//С��0����ʧ��
						else
						{
							cout << "recv error" << endl;
							closesocket(fdSocket.fd_array[i]);
							FD_CLR(fdSocket.fd_array[i], &fdSocket);
						}
					}
					//udp����
					else
					{
						memset(recvBuf, 0, 100);
						cout << "udp���ݡ�������Ϊ " << inet_ntoa(addrSrvTCP.sin_addr) << endl;
						recvfrom(udpServerSock, recvBuf, 100, 0, (struct sockaddr*)&addrSrvUDP, &lenSrv);
						sprintf(sendBuf, "echo:%s", recvBuf);
						sendto(udpServerSock, sendBuf, 100, 0, (struct sockaddr*)&addrSrvUDP, lenSrv);
					}
				}
				else
				{
					cout << "select error" << endl;
					break;
				}
			}
		}
		closesocket(tcpConnSock);
		closesocket(tcpServerSock);
		closesocket(udpServerSock);
		WSACleanup();
		return 0;
	}
}