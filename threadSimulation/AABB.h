#pragma once

#include"Nodes.h"

class AABB {

	friend bool isCollided(AABB a, AABB b);

private:
	threeVector max;
	threeVector min;
	bool isLeaf;//标记是否为叶子节点（即区分是不是包含线段的包围盒）
	int segNum;//记录该包围盒里边装的绳子的编号。
	int nodeNum;//记录该包围盒BVH树中的节点编号。**待修改**

public:
    //默认构造函数不太敢下手，有点忘了。您们看看需不需要改。
    AABB();


	//给定两个向量（此处为一小段刚性绳的两个顶点，没有考虑半径），构建一个AABB包围盒
	AABB(threeVector a, threeVector b, int inputSegNum);

	//给定两个AABB包围盒，以此构建一个新的包围盒
	//其实也可以每一层都用两个顶点的三维向量来构建包围盒，这里只是写出来供参考对比，看看哪种效果好。
	AABB(AABB a, AABB b);


	void fitTheRope(double radius);
	void show();
	bool isLeafNode() {return isLeaf; };
	int getSegNum() {return segNum; };
	void setNodeNum (int input) {nodeNum = input; };
	int getNodeNum () {return nodeNum; };
};
