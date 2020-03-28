#include<stdio.h>
#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>    
#include<time.h>
#include<string>
#include<string.h>
using namespace std;

int *inputList;
vector<int> fcfsList;
vector<int> sstfList;
vector<int> scanList;
int trackNum;
int nowLocation;

void init(string choice)
{
	cout << "****************** ���ɴŵ��� **********************��" << endl;

	if (choice == "manual")
	{
		cout << "������һ���Ĵŵ�������" << endl;
		cin >> trackNum;
		inputList = new int[trackNum];
		cout << "��������ŵ��ţ�1-200����" << endl;
		for (int i = 0; i < trackNum; i++)
		{
			cin >> inputList[i];
		}
		cout << "�ֶ�����Ĵŵ�����Ϊ��" << endl;
		for (int i = 0; i < trackNum; i++)
		{
			cout << inputList[i] << "\t";
		}
		cout << endl;
		cout << "���뵱ǰ��ͷλ�ã�1-200����" << endl;
		cin >> nowLocation;
	}
	else if (choice == "auto")
	{
		cout << "������һ���Ĵŵ�������" << endl;
		cin >> trackNum;
		inputList = new int[trackNum];
		srand(time(NULL));
		for (int i = 0; i < trackNum; i++)
		{
			inputList[i] = rand() % 200 + 1;
		}
		nowLocation = rand() % 200 + 1;
		cout << "��������Ĵŵ�����Ϊ��" << endl;
		for (int i = 0; i < trackNum; i++)
		{
			cout << inputList[i] << "\t";
		}
		cout << endl;
		cout << "��ǰ��ͷΪ�� " << nowLocation << endl;
	}
}

float fcfs() {
	cout << endl;
	cout << "****************** fcfs �㷨 **********************" << endl;
	float fcfsMoveNum = 0;
	float avgNum = 0;
	for (int i = 0; i < trackNum; i++)
	{
		fcfsList.push_back(inputList[i]);
	}
	cout << "���㷨��Ѱ������Ϊ��" << endl;
	for (int i = 0; i < fcfsList.size(); i++)
	{
		cout << fcfsList[i] << "\t";
	}
	cout << endl;
	for (int i = 0; i < fcfsList.size(); i++)
	{
		fcfsMoveNum += abs(fcfsList[i] - nowLocation);
		nowLocation = fcfsList[i];
	}
	cout << "���㷨�����Ʊ۾���Ϊ��" << int(fcfsMoveNum) << endl;
	cout << "���㷨��ƽ���Ʊ۾���Ϊ��" << float(fcfsMoveNum / trackNum) << endl;
	fcfsList.swap(vector<int>());
	return float(fcfsMoveNum / trackNum);

}


float sstf() {
	cout << endl;
	cout << "****************** sstf �㷨 **********************" << endl;
	
	float sstfMoveNum = 0;
	int nextLocationNum = nowLocation;
	int tmpMin = 200;
	int delta;
	int nextLocation = 0;
	int *flag;
	flag = new int[trackNum];
	while (sstfList.size() != trackNum)
	{
		for(int i = 0; i < trackNum; i++)
		{
			if (flag[i] != 1)
			{
				delta = abs(nextLocationNum - inputList[i]);
				if (delta < tmpMin)
				{
					tmpMin = delta;
					nextLocation = i;
				}
			}
		}
		sstfList.push_back(inputList[nextLocation]);
		flag[nextLocation] = 1;
		nextLocationNum = inputList[nextLocation];
		tmpMin = 200;
	}
	cout << "���㷨��Ѱ������Ϊ��" << endl;
	for (int i = 0; i < sstfList.size(); i++)
	{
		cout << sstfList[i] << "\t";
	}
	cout << endl;
	for (int i = 0; i < sstfList.size(); i++)
	{
		sstfMoveNum += abs(sstfList[i] - nowLocation);
		nowLocation = sstfList[i];
	}
	cout << "���㷨�����Ʊ۾���Ϊ��" << int(sstfMoveNum) << endl;
	cout << "���㷨��ƽ���Ʊ۾���Ϊ��" << float(sstfMoveNum / trackNum) << endl;
	sstfList.swap(vector<int>());
	return float(sstfMoveNum / trackNum);

}


//Ĭ�������ҵ�scan
float scan() {
	cout << endl;
	cout << "****************** scan �㷨 **********************" << endl;
	float scanMoveNum = 0;
	int storeLocation = nowLocation;
	int *tmpList;
	tmpList = new int[trackNum];
	for (int i = 0; i < trackNum; i++)
	{
		tmpList[i] = inputList[i];
	}
	sort(tmpList, tmpList + trackNum);
	int min = tmpList[0];
	int max = tmpList[trackNum - 1];
	int p = 0;
	if (nowLocation <= min)
	{
		for (int i = 0; i < trackNum; i++)
		{
			scanList.push_back(tmpList[i]);
		}
	}
	else if (nowLocation >= max)
	{
		for (int i = trackNum - 1; i >= 0; i--)
		{
			scanList.push_back(tmpList[i]);
		}
	}
	else if(nowLocation > min && nowLocation < max)
	{
		while (tmpList[p + 1] <= nowLocation )
		{
			p++;
		}
		for (int i = p + 1; i < trackNum; i++)
			scanList.push_back(tmpList[i]);
		for (int i = p; i >= 0; i--)
			scanList.push_back(tmpList[i]);
	}
	cout << "���㷨��Ѱ������Ϊ��" << endl;
	for (int i = 0; i < scanList.size(); i++)
	{
		cout << scanList[i] << "\t";
	}
	cout << endl;
	for (int i = 0; i < scanList.size(); i++)
	{
		scanMoveNum += abs(scanList[i] - nowLocation);
		nowLocation = scanList[i];
	}
	cout << "���㷨�����Ʊ۾���Ϊ��" << int(scanMoveNum) << endl;
	cout << "���㷨��ƽ���Ʊ۾���Ϊ��" << float(scanMoveNum / trackNum) << endl;
	scanList.swap(vector<int>());
	return float(scanMoveNum / trackNum);
}

void compare() {
	float avgFCFS;
	float avgSSTF;
	float avgSCAN;
	
	string cmd = "auto";
	for (int i = 0; i < 3; i++)
	{
		cout << "�����ʼ��" << endl;
		init(cmd);
		int storeLocation = nowLocation;
		cout << endl;
		nowLocation = storeLocation;
		avgFCFS = fcfs();
		nowLocation = storeLocation;
		avgSSTF = sstf();
		nowLocation = storeLocation;
		avgSCAN = scan();
		cout << endl;
	}
	cout << "�����㷨�����������Σ�ƽ���Ʊ۾���Ϊ��" << endl;
	cout << "FCFS�㷨��           " << avgFCFS << endl;
	cout << "SSTF�㷨��           " << avgSSTF << endl;
	cout << "SCAN�㷨��           " << avgSCAN << endl;
}
void help() {
	cout << endl;
	cout << "************************ help ********************************" << endl;
	cout << "1. init		           �ֶ�ģʽ���ֶ���ʼ�����ֶ�ѡ���㷨��" << endl;
	cout << "2. fcfs/sstf/scan	   �ֶ�ģʽ��ѡ�����ֵ����㷨		       " << endl;
	cout << "3. auto			   ȫ�Զ�ģʽ���Զ���ʼ���������������㷨��" << endl;
	cout << "4. help			   չʾָ���������                        " << endl;
	cout << "4. quit			   �˳�                                    " << endl;
	cout << "************************ help ********************************" << endl;
	cout << endl;
}

void shell() {
	while (true)
	{
		help();
		cout << "shell>";
		string cmd1;
		int cmd2;
		int cmd3;
		int cmd4;
		cin >> cmd1;
		if (cmd1 == "init")
		{
			init("manual");
		}
		else if (cmd1 == "fcfs")
		{
			fcfs();
		}
		else if (cmd1 == "sstf")
		{
			sstf();
		}
		else if (cmd1 == "scan")
		{
			scan();
		}
		else if (cmd1 == "auto")
		{
			compare();
		}
		else if (cmd1 == "help")
		{
			help();
		}
		else if (cmd1 == "quit")
		{
			break;
		}
		else
		{
			cout << "��������!" << endl;
			help();
		}
	}
}

int main() {
	shell();
}