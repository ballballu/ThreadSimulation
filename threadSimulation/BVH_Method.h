#pragma once

#include"AABB.h"

using namespace std;

int leafDivide(int);
bool isFull(int);//�����ж�������ǲ���2��n�η��������˵�����Ǹ���������
void buildBVHTree (int root, AABB rope[], int start , int end );
void traverseTree (AABB, AABB);
void testTraverseTree();



