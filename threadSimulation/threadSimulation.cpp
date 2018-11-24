// threadSimulation.cpp : 定义 DLL 应用程序的导出函数。

#include "stdafx.h"
#include "threadSimulation.h"

const double inter = 1;
const int number_node = 150;
const double radius = 0.5;

vector<threeVector> rope;

void initSimulation(float input[150][3]) {
	//输入150个点的xyz坐标

	rope.clear();
	for (int i = 0; i < number_node; i++) {
		threeVector tmp;
		double x = static_cast<double>(input[i][0]);
		double y = static_cast<double>(input[i][1]);
		double z = static_cast<double>(input[i][2]);
		tmp.setXYZ(x, y, z);//这里应该是输入数据来初始化
		rope.push_back(tmp);
	}
}

void simulate1(float change1[3]) {
	//输入第一个点移动到的新位置。


	//TODO 根据input改变某个点的位置，如rope[i]=xxx这样，还要考虑步长限制

	double x1 = static_cast<double>(change1[0]);
	double y1 = static_cast<double>(change1[1]);
	double z1 = static_cast<double>(change1[2]);


	rope[0].setXYZ(x1, y1, z1);
	int numGrasp = 1;
	vector<int> nodeGrasp;
	nodeGrasp.push_back(0);

	//FTL
	FTL(rope, numGrasp, nodeGrasp);

	//构造AABB包围盒
	AABB leafNodes[number_node - 1];
	for (int i = 0; i<number_node - 1; i++) {
		AABB tmp(rope[i], rope[i + 1], i + 1);
		tmp.fitTheRope(radius);
		leafNodes[i] = tmp;
	}

	//BVH树的建立与遍历
	buildBVHTree(0, leafNodes, 0, number_node - 2);//number_node-2是指，算出了线段的数目再减一
	testTraverseTree();

	//碰撞管理
	collisionManagement(rope);

}


void simulate2(float change1[3], float change2[3]) {
	//输入第一个点和最后一个点（编号为0,就是绳子初始化中的第一个点）移动到的新位置。


	//TODO 根据input改变某个点的位置，如rope[i]=xxx这样，还要考虑步长限制

	double x1 = static_cast<double>(change1[0]);
	double y1 = static_cast<double>(change1[1]);
	double z1 = static_cast<double>(change1[2]);

	double x2 = static_cast<double>(change2[0]);
	double y2 = static_cast<double>(change2[1]);
	double z2 = static_cast<double>(change2[2]);

	rope[0].setXYZ(x1, y1, z1);
	rope[number_node - 1].setXYZ(x2, y2, z2);
    int numGrasp = 2;
	vector<int> nodeGrasp;
	nodeGrasp.push_back(0);
	nodeGrasp.push_back(number_node - 1);
	

	//FTL
	FTL(rope, numGrasp, nodeGrasp);

	//构造AABB包围盒
	AABB leafNodes[number_node - 1];
	for (int i = 0; i<number_node - 1; i++) {
		AABB tmp(rope[i], rope[i + 1], i + 1);
		tmp.fitTheRope(radius);
		leafNodes[i] = tmp;
	}

	//BVH树的建立与遍历
	buildBVHTree(0, leafNodes, 0, number_node - 2);//number_node-2是指，算出了线段的数目再减一
	testTraverseTree();

	//碰撞管理
	collisionManagement(rope);

}


void copySimulationResult(float output[])
{
	/*float *output = new float[450];*/
	for (int i = 0; i < number_node; i++) {
		float x = static_cast<float>(rope[i].gtx());
		float y = static_cast<float>(rope[i].gty());
		float z = static_cast<float>(rope[i].gtz());
		output[i * 3] = x;
		output[i * 3+1] = y;
		output[i * 3+2] = z;
	}
}
