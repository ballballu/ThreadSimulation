
#include "stdafx.h"
#include <iostream>
#include "BVH_Method.h"
#include "AABB.h"

using namespace std;

const int number_node = 150;//这里的线段个数和节点个数相关（-1），并且以此计算好要开数组的大小。

const int MAXNUM = 2*(number_node-1)-1; //该完全二叉树所需要的数组空间大小。
//开一个全局变量，一个AABB盒构成的数组——BVH树。

const int maxResultNum = 1 + ((number_node-1)*(number_node-2) / 2 - (number_node-2))*2; //用于存放结果的数组。这是其最大长度。

int collisionResult[maxResultNum];  //用于记录碰撞结果。两个两个一组，代表发生相交的包围盒所包含绳子的编号。
int resultPosition = 0;             //用于记录当前的结果记录到数组的哪个位置了。每次检测前要清零。

AABB BVHTree[MAXNUM];

int leafDivide(int offset){
    if (isFull(offset))
        return (offset/2);
    //如果是满二叉树，直接返回一半作为左子树的节点数。

    int mMax = 1;

    while (mMax * 2 < offset)
        mMax *= 2;           //子树叶节点个数上限
    int mMin = mMax/2;       //子树叶节点个数下限
    if ((offset-mMin)>mMax)
        return mMax;         //左子树是满二叉树
    else
        return (offset-mMin);//右子树是满二叉树
}


bool isFull(int offset){
    int i = 1;
    while(i<=offset){
        if (i == offset)
        return true;
        i*=2;
    }
    return false;
}

//构建BVH树。递归函数
void buildBVHTree (int root, AABB rope[], int start , int end ){
    if (start==end)//叶子节点
        BVHTree[root]=rope[start];

    else{
        int offset = end - start + 1;
        int div = leafDivide(offset);
        buildBVHTree(root*2+1, rope, start, start + div -1);//递归构造左子树
        buildBVHTree(root*2+2, rope, start + div, end);     //递归构造右子树
        //根据当前左右子节点包围盒构造该根节点的包围盒
        AABB tmp (BVHTree[root*2+1], BVHTree[root*2+2]);
        tmp.setNodeNum(root);//把该节点在BVH树的编号也放进去。
        BVHTree[root]=tmp;
    }

}

void traverseTree (AABB boxA , AABB boxB){
    if (!isCollided(boxA, boxB)){
        return;
    }

    if (boxB.isLeafNode()){

        if (boxA.isLeafNode()){
            //Todo
            int numA = boxA.getSegNum();
            int numB = boxB.getSegNum();
            //cout<<numA<<"和"<<numB<<"号线段相交了，并且都是叶子节点。"<<endl;
            if (numA < (numB - 2) ){
                collisionResult[resultPosition] = numA;
                collisionResult[resultPosition + 1] = numB;
                resultPosition +=2;
            }
        }
        else{
            int leftNodeOfA = boxA.getNodeNum()*2 + 1;
            int rightNodeOfA = boxA.getNodeNum()*2 + 2;
            traverseTree(BVHTree[leftNodeOfA],boxB);
            traverseTree(BVHTree[rightNodeOfA],boxB);
        }
    }
    else{
        int leftNodeOfB = boxB.getNodeNum()*2 + 1;
        int rightNodeOfB = boxB.getNodeNum()*2 + 2;
        traverseTree(boxA, BVHTree[leftNodeOfB]);
        traverseTree(boxA, BVHTree[rightNodeOfB]);
    }

    return;
}

void testTraverseTree(){
    //结果数组初始化。
    //初始化为每个位置都是0，是为了后面使用该数组时能够找到终点。为了防止下标溢出，数组的长度比实际需要的多1，这个位置一定是0。
    for ( int i = 0 ;i < maxResultNum; i++)
        collisionResult[i] = 0 ;
    resultPosition = 0;


    //cout<<"开始测试"<<endl;
    //cout<<"结果数组的最大长度为"<<maxResultNum<<endl;
    traverseTree(BVHTree[0], BVHTree[0]);
}
