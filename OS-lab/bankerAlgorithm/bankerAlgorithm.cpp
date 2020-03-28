#include<stdio.h>
#include<iostream>
#include<string.h>
#include<string>
#include<iomanip>
using namespace std;

int *Available;	//��������Դʸ��
int **Max;	//����������
int **Allocation;	//�������
int **Need;	//�������
int *securitySequence;
int *securityPid;
int *pid;
int *rid;

int resourceTypeNum;
int processNum;


void show();
bool securityJudge();

void init() {
	cout << "�����������" << endl;
	cin >> processNum;
	securitySequence = new int[processNum];
	securityPid = new int[processNum];
	for (int i = 0; i < processNum; i++)
	{
		securitySequence[i] = 0;
		securityPid[i] = -1;
	}
	cout << "������Դ��������" << endl;
	cin >> resourceTypeNum;
	Available = new int[resourceTypeNum];
	cout << "��������ÿ����Դ�Ŀ��ø���Available��" << endl;
	for (int i = 0; i < resourceTypeNum; i++)
	{
		cin >> Available[i];
	}
	//Ϊÿ�����̳�ʼ����max��allocation����
	Max = new int*[processNum];
	Allocation = new int*[processNum];
	Need = new int*[processNum];
	for (int i = 0; i < processNum; i++)
	{
		Max[i] = new int[resourceTypeNum];
		Allocation[i] = new int[resourceTypeNum];
		Need[i] = new int[resourceTypeNum];
	}
	//��ʼ��Allocation
	cout << "�����ѷ������Allocation��" << endl;
	for (int i = 0; i < processNum; i++)
		for (int j = 0; j < resourceTypeNum; j++)
			cin >> Allocation[i][j];

	cout << "��������������Max��" << endl;
	for (int i = 0; i < processNum; i++)
	{
		for (int j = 0; j < resourceTypeNum; j++)
		{
			cin >> Max[i][j];
		}
	}
	//��ʼ��Need����
	for (int i = 0; i < processNum; i++)
	{
		for (int j = 0; j < resourceTypeNum; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}
	//initShow();
	show();
}


void show() {
	cout << "����������Max : " << endl;
	for (int i = 0; i < processNum; i++)
	{
		cout << "  |";
		for (int j = 0; j < resourceTypeNum; j++)
		{
			cout << "   " << Max[i][j];
		}
		cout << "  |";
		cout << endl;
	}
	cout << endl;
	cout << "�������Allocation : " << endl;
	for (int i = 0; i < processNum; i++)
	{
		cout << "  |";
		for (int j = 0; j < resourceTypeNum; j++)
		{
			cout << "   " << Allocation[i][j];
		}
		cout << "  |";
		cout << endl;
	}
	cout << endl;
	cout << "�������Need : " << endl;
	for (int i = 0; i < processNum; i++)
	{
		cout << "  |";
		for (int j = 0; j < resourceTypeNum; j++)
		{
			cout << "   " << Need[i][j];
		}
		cout << "  |";
		cout << endl;
	}
	cout << endl;

	cout << "��������Դʸ����" << endl;
	for (int i = 0; i < resourceTypeNum; i++)
	{
		cout << "  " << Available[i];
	}
	cout << endl; 
}


bool securityJudge()
{
	for (int i = 0; i < processNum; i++)
	{
		securitySequence[i] = 0;
		securityPid[i] = -1;
	}
	int *tmpAvail = new int[resourceTypeNum];
	for (int i = 0; i < resourceTypeNum; i++)
		tmpAvail[i] = Available[i];
	int flag = 0;
	//���ѭ�����������жϣ�ʹ��ȫ������
	for (int i = 0; i < processNum; i++)
	{
		//�ڲ�ѭ���������ж�ÿ�����̣�������
		for (int j = 0; j < processNum; j++)
		{
			if (securitySequence[j] == 0)
			{
				//���ѭ���ж�ĳһNeed���Ƿ�С��Available����
				for (int k = 0; k < resourceTypeNum; k++)
				{
					if (Need[j][k] <= tmpAvail[k])
					{
						flag++;
					}
				}
				//��������������ȫ����
				if (flag == resourceTypeNum)
				{
					securitySequence[j] = 1;
					securityPid[i] = j;
					//�����к��ͷ���Դ
					for (int k = 0; k < resourceTypeNum; k++)
						tmpAvail[k] += Allocation[j][k];
					flag = 0;
					break;
				}
				flag = 0;
			}
		}
	}

	for (int i = 0; i< processNum; i++)
	{

		if (securitySequence[i] == 0)
		{
			return false;
		}
	}
	cout << "��ȫ����Ϊ��{ ";
	for (int i = 0; i < processNum - 1; i++)
	{
		cout << securityPid[i] << ", ";
	}
	cout << securityPid[processNum - 1] << " } " << endl;
	return true;

}

//Pi��ʾ����i��Rj��ʾ������Դj��k��ʾ��������
void request(int Pi, int Rj, int k ){
	if (Need[Pi][Rj] < k)
	{
		cout << "����ʧ�ܣ���Ϊ����Ҫ��Դ�������������������ֵ" << endl;
	}
	else
	{
		if (k > Available[Rj])
		{
			cout << "����ʧ�ܣ���Ϊ�����㹻��Դ" << endl;
		}
		else
		{
			Available[Rj] -= k;
			Allocation[Pi][Rj] += k;
			Need[Pi][Rj] -= k;
			if (securityJudge())
			{
				cout << "������Դ�ɹ�" << endl;
			}
			else
			{
				Available[Rj] += k;
				Allocation[Pi][Rj] -= k;
				Need[Pi][Rj] += k;
				cout << "������Դʧ�ܣ���Ϊ��������㰲ȫ����" << endl;
			}
		}
	}
}


void help() {
	cout << "******************** help *********************** " << endl;
	cout << "request Pi, Rj, k   Pi��������Rj��Դk��" << endl;
	cout << "help                չʾ��������       " << endl;
	cout << "security            ֱ�ӵ��ð�ȫ�㷨���鿴��ȫ����" << endl;
	cout << "show                չʾ��ǰ���о������ " << endl;
	cout << "quit                �˳�                 " << endl;
	cout << "******************** help *********************** " << endl;
}

void shell() {
	while (1) 
	{
		cout << "shell>";
		string cmd1;
		int cmd2;
		int cmd3;
		int cmd4;
		cin >> cmd1;
		if (cmd1 == "request")
		{
			cin >> cmd2;
			cin >> cmd3;
			cin >> cmd4;
			request(cmd2, cmd3, cmd4);
			show();
		}
		else if (cmd1 == "show")
		{
			show();
		}
		else if (cmd1 == "help")
		{
			help();
		}
		else if (cmd1 == "quit")
		{
			break;
		}
		else if(cmd1 == "security")
		{
			securityJudge();
		}
		else
		{
			cout << "��������!" << endl;
			help();
		}
	}
}


int main() {
	init();
	shell();
	show();
	return 0;

}