
#include "stdafx.h"
#include <iostream>
#include "BVH_Method.h"
#include "AABB.h"

using namespace std;

const int number_node = 150;//������߶θ����ͽڵ������أ�-1���������Դ˼����Ҫ������Ĵ�С��

const int MAXNUM = 2*(number_node-1)-1; //����ȫ����������Ҫ������ռ��С��
//��һ��ȫ�ֱ�����һ��AABB�й��ɵ����顪��BVH����

const int maxResultNum = 1 + ((number_node-1)*(number_node-2) / 2 - (number_node-2))*2; //���ڴ�Ž�������顣��������󳤶ȡ�

int collisionResult[maxResultNum];  //���ڼ�¼��ײ�������������һ�飬�������ཻ�İ�Χ�����������ӵı�š�
int resultPosition = 0;             //���ڼ�¼��ǰ�Ľ����¼��������ĸ�λ���ˡ�ÿ�μ��ǰҪ���㡣

AABB BVHTree[MAXNUM];

int leafDivide(int offset){
    if (isFull(offset))
        return (offset/2);
    //���������������ֱ�ӷ���һ����Ϊ�������Ľڵ�����

    int mMax = 1;

    while (mMax * 2 < offset)
        mMax *= 2;           //����Ҷ�ڵ��������
    int mMin = mMax/2;       //����Ҷ�ڵ��������
    if ((offset-mMin)>mMax)
        return mMax;         //����������������
    else
        return (offset-mMin);//����������������
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

//����BVH�����ݹ麯��
void buildBVHTree (int root, AABB rope[], int start , int end ){
    if (start==end)//Ҷ�ӽڵ�
        BVHTree[root]=rope[start];

    else{
        int offset = end - start + 1;
        int div = leafDivide(offset);
        buildBVHTree(root*2+1, rope, start, start + div -1);//�ݹ鹹��������
        buildBVHTree(root*2+2, rope, start + div, end);     //�ݹ鹹��������
        //���ݵ�ǰ�����ӽڵ��Χ�й���ø��ڵ�İ�Χ��
        AABB tmp (BVHTree[root*2+1], BVHTree[root*2+2]);
        tmp.setNodeNum(root);//�Ѹýڵ���BVH���ı��Ҳ�Ž�ȥ��
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
            //cout<<numA<<"��"<<numB<<"���߶��ཻ�ˣ����Ҷ���Ҷ�ӽڵ㡣"<<endl;
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
    //��������ʼ����
    //��ʼ��Ϊÿ��λ�ö���0����Ϊ�˺���ʹ�ø�����ʱ�ܹ��ҵ��յ㡣Ϊ�˷�ֹ�±����������ĳ��ȱ�ʵ����Ҫ�Ķ�1�����λ��һ����0��
    for ( int i = 0 ;i < maxResultNum; i++)
        collisionResult[i] = 0 ;
    resultPosition = 0;


    //cout<<"��ʼ����"<<endl;
    //cout<<"����������󳤶�Ϊ"<<maxResultNum<<endl;
    traverseTree(BVHTree[0], BVHTree[0]);
}
