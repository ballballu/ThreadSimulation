#pragma once

#include"AABB.h"

using namespace std;

int leafDivide(int);
bool isFull(int);//用来判断这个数是不是2的n次方。如果是说明这是个满二叉树
void buildBVHTree (int root, AABB rope[], int start , int end );
void traverseTree (AABB, AABB);
void testTraverseTree();



