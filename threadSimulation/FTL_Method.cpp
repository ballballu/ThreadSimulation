
#include "stdafx.h"
#include"FTL_Method.h"

const int number_node = 150;                  //�����Ͻڵ�ĸ���

void FTL1(vector<threeVector> &rope, int i, int j) {
	int e = sign(j - i);
	int a = i;
	int b = a + e;

	threeVector clusterDelta[10];
	//��ʼ��
	for (int j = 0; j < 10; j++) {
		threeVector tmp(0, 0, 0);
		clusterDelta[j] = tmp;
	}
	bool clusterDeltaFlag[10];
	for (int j = 0; j < 10; j++) 
		clusterDeltaFlag[j] = false;


	while (b != j + e) {

		//������������ײ���ƣ��Ͳ��ܶ�����ֱ��������
		//������Ϊ�Ǳ���a�����ƶ�b�����b����ײ���ƾ����������aҲ����ײ���ƣ��ڵ���ftl֮ǰ��Ҫ��ֹ�Ǹ�����ƶ���

		/*
		if (rope[b].isConstraint()) {
			rope[b].removeConstraint();//�Ѿ������������ˣ�����˳�־Ͱ��������ƽ��
			a = b;
			b = a + e;
			continue;
		}
		*/



		//����ƶ��˵ĵ�a���Ǳ����ĵ㣬������һ����b�ǣ���ô��������b���ƶ��ľ��룬���Ҽ�¼��clusterDelta��������һ���ƶ���
		//����û�п��� ���ĵ�һ������Ǳ����ĵ����� ̫�����ˡ�

		if (rope[b].isKnot()) {
			int c = rope[b].getCluster();
			if (clusterDeltaFlag[c]) {//˵��������Ѿ����趨�õ��ƶ������ˣ�Ҫͳһ�ƶ�
				//cout <<"���Ǹ����ϵĵ㣺"<< rope[b] << endl << endl;
				threeVector tmp = rope[b] + clusterDelta[c];
				rope[b] = tmp; 
				//��������ֵ������������ ֱ��rope[b] = rope[b]+...���У���Ҳ��֪��Ϊɶ
				//ȷʵ���У���Ϊrope[b]+tmp ���ص���һ��û�����Ե��������ᶪʧ�ڵ�bԭ�е����ԡ�
				//�����޸�һ�¸�ֵ���캯���ɡ�
				a = b;
				b = a + e;
				continue;
			}
			else {
				//˵��ǰ���Ǹ����Ǹ����ɵ㣬������������ᣬҪ����һ�¡�
				//cout << "���ǽ�ĵ�һ���㣺" << rope[b] << endl << endl;
				threeVector u = rope[b] - rope[a];
				u.unitlize();
				threeVector tmp = rope[a] + u;
				threeVector tmp2 = tmp - rope[b];//�����b��Ӧ���ƶ�������
				clusterDelta[c] = tmp2;
				clusterDeltaFlag[c] = true;
				rope[b] = tmp;
				a = b;
				b = a + e;
				continue;
			}
		}


		threeVector u = rope[b] - rope[a];
		u.unitlize();
		threeVector tmp = rope[a] + u;
		rope[b] = tmp;
		a = b;
		b = a + e;
	}
}

void FTL2(vector<threeVector>& rope, int i, int j) {
	//���￪����дO�����i~j�м��ĳ�ʼλ�ã�������i��j����
	//��Ϊ�����ʱ��i��jλ���Ѿ�������testFTL2�������趨����
	//�µ�λ�á���FTL2�㷨�в�û���ƶ�i��j�����㡣���Բ��ñ�
	//����������ԭ����λ��
	vector<threeVector>  O;
	for (int m = i + 1; m < j; m++)
		O.push_back(rope[m]);
	FTL1(rope, i, j - 1);

	vector<threeVector> C;
	for (int m = i + 1; m < j; m++)
		C.push_back(rope[m]);
	for (int m = i + 1, k = 0; m < j; m++, k++)
		rope[m] = O[k];                      //�ָ�

	FTL1(rope, j, i + 1);

	for (int a = i + 1, k = 0; a < j; a++, k++) {
		threeVector tmp1 = rope[a] + C[k];
		threeVector tmp2 = tmp1 / 2;
		rope[a] = tmp2;
	}
}

void FTL(vector<threeVector>& rope, int numGrasp, vector<int> nodeGrasp) {
	if (nodeGrasp[0] > 0)
		FTL1(rope, nodeGrasp[0], 0);
	if (nodeGrasp[numGrasp - 1] < number_node - 1)
		FTL1(rope, nodeGrasp[numGrasp - 1], number_node - 1);
	for (int i = 0; i < numGrasp - 1; i++)
		FTL2(rope, nodeGrasp[i], nodeGrasp[i + 1]);
}


int sign(int x) {
	const int just = 1;
	const int zero = 0;
	const int nega = -1;
	if (x > 0)
		return just;
	else if (x < 0)
		return nega;
	else
		return zero;
}
