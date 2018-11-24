#pragma once

#include"Nodes.h"

class AABB {

	friend bool isCollided(AABB a, AABB b);

private:
	threeVector max;
	threeVector min;
	bool isLeaf;//����Ƿ�ΪҶ�ӽڵ㣨�������ǲ��ǰ����߶εİ�Χ�У�
	int segNum;//��¼�ð�Χ�����װ�����ӵı�š�
	int nodeNum;//��¼�ð�Χ��BVH���еĽڵ��š�**���޸�**

public:
    //Ĭ�Ϲ��캯����̫�����֣��е����ˡ����ǿ����費��Ҫ�ġ�
    AABB();


	//���������������˴�ΪһС�θ��������������㣬û�п��ǰ뾶��������һ��AABB��Χ��
	AABB(threeVector a, threeVector b, int inputSegNum);

	//��������AABB��Χ�У��Դ˹���һ���µİ�Χ��
	//��ʵҲ����ÿһ�㶼�������������ά������������Χ�У�����ֻ��д�������ο��Աȣ���������Ч���á�
	AABB(AABB a, AABB b);


	void fitTheRope(double radius);
	void show();
	bool isLeafNode() {return isLeaf; };
	int getSegNum() {return segNum; };
	void setNodeNum (int input) {nodeNum = input; };
	int getNodeNum () {return nodeNum; };
};
