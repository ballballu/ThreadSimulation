
#include "stdafx.h"
#include"FTL_Method.h"

const int number_node = 150;                  //绳子上节点的个数

void FTL1(vector<threeVector> &rope, int i, int j) {
	int e = sign(j - i);
	int a = i;
	int b = a + e;

	threeVector clusterDelta[10];
	//初始化
	for (int j = 0; j < 10; j++) {
		threeVector tmp(0, 0, 0);
		clusterDelta[j] = tmp;
	}
	bool clusterDeltaFlag[10];
	for (int j = 0; j < 10; j++) 
		clusterDeltaFlag[j] = false;


	while (b != j + e) {

		//如果这个点有碰撞限制，就不能动他。直接跳过。
		//这里因为是变了a，来移动b，如果b有碰撞限制就跳过。如果a也有碰撞限制，在调用ftl之前就要禁止那个点的移动。

		/*
		if (rope[b].isConstraint()) {
			rope[b].removeConstraint();//已经决定不动他了，这里顺手就把他的限制解除
			a = b;
			b = a + e;
			continue;
		}
		*/



		//如果移动了的点a不是被打结的点，而后面一个点b是，那么照样计算b该移动的距离，并且记录到clusterDelta里，供后面的一起移动。
		//这里没有考虑 动的第一个点就是被打结的点的情况 太极端了。

		if (rope[b].isKnot()) {
			int c = rope[b].getCluster();
			if (clusterDeltaFlag[c]) {//说明这个结已经有设定好的移动向量了，要统一移动
				//cout <<"这是个结上的点："<< rope[b] << endl << endl;
				threeVector tmp = rope[b] + clusterDelta[c];
				rope[b] = tmp; 
				//这里有奇怪的数据溢出问题 直接rope[b] = rope[b]+...不行，我也不知道为啥
				//确实不行，因为rope[b]+tmp 返回的是一个没有特性的向量，会丢失节点b原有的特性。
				//再来修改一下赋值构造函数吧。
				a = b;
				b = a + e;
				continue;
			}
			else {
				//说明前面那个还是个自由点，他拉动了这个结，要计算一下。
				//cout << "这是结的第一个点：" << rope[b] << endl << endl;
				threeVector u = rope[b] - rope[a];
				u.unitlize();
				threeVector tmp = rope[a] + u;
				threeVector tmp2 = tmp - rope[b];//这个是b点应该移动的向量
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
	//这里开个大写O数组存i~j中间点的初始位置（不包括i，j），
	//因为处理的时候i、j位置已经定死在testFTL2程序中设定好了
	//新的位置。在FTL2算法中并没有移动i，j两个点。所以不用保
	//留这两个的原来的位置
	vector<threeVector>  O;
	for (int m = i + 1; m < j; m++)
		O.push_back(rope[m]);
	FTL1(rope, i, j - 1);

	vector<threeVector> C;
	for (int m = i + 1; m < j; m++)
		C.push_back(rope[m]);
	for (int m = i + 1, k = 0; m < j; m++, k++)
		rope[m] = O[k];                      //恢复

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
