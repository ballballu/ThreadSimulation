#include "stdafx.h"
#include"AABB.h"
bool isCollided (AABB a,AABB b){

    if(a.max.gtx() < b.min.gtx() || a.min.gtx() > b.max.gtx()) return false ;
    if(a.max.gty() < b.min.gty() || a.min.gty() > b.max.gty()) return false ;
    if(a.max.gtz() < b.min.gtz() || a.min.gtz() > b.max.gtz()) return false ;
    return true ;
}

AABB::AABB(){
    isLeaf=false;
    segNum=0;
    nodeNum=0;
}

AABB::AABB(threeVector a, threeVector b, int inputSegNum){

	double maxX, maxY, maxZ, minX, minY, minZ;

	if (a.gtx() >= b.gtx()) {
		maxX = a.gtx();
		minX = b.gtx();
	}
	else {
		maxX = b.gtx();
		minX = a.gtx();
	}

	if (a.gty() >= b.gty()) {
		maxY = a.gty();
		minY = b.gty();
	}
	else {
		maxY = b.gty();
		minY = a.gty();
	}

	if (a.gtz() >= b.gtz()) {
		maxZ = a.gtz();
		minZ = b.gtz();
	}
	else {
		maxZ = b.gtz();
		minZ = a.gtz();
	}

	max.setXYZ(maxX, maxY, maxZ);
	min.setXYZ(minX, minY, minZ);
	isLeaf=true;
	segNum=inputSegNum;
	nodeNum=0;

}

AABB::AABB(AABB a, AABB b) {

	double maxX, maxY, maxZ, minX, minY, minZ;

	if (a.max.gtx() >= b.max.gtx())
		maxX = a.max.gtx();
	else
		maxX = b.max.gtx();
	if (a.min.gtx() <= b.min.gtx())
		minX = a.min.gtx();
	else
		minX = b.min.gtx();


	if (a.max.gty() >= b.max.gty())
		maxY = a.max.gty();
	else
		maxY = b.max.gty();
	if (a.min.gty() <= b.min.gty())
		minY = a.min.gty();
	else
		minY = b.min.gty();


	if (a.max.gtz() >= b.max.gtz())
		maxZ = a.max.gtz();
	else
		maxZ = b.max.gtz();
	if (a.min.gtz() <= b.min.gtz())
		minZ = a.min.gtz();
	else
		minZ = b.min.gtz();

	max.setXYZ(maxX, maxY, maxZ);
	min.setXYZ(minX, minY, minZ);
	isLeaf=false;
	segNum=0;
	nodeNum=0;
}

//这个东西就是所谓的，“将6个面向外推r长度，也就是最大，最小的点的三维坐标相应变化r”
void AABB::fitTheRope(double radius){
    max.setXYZ(max.gtx()+radius,max.gty()+radius,max.gtz()+radius);
    min.setXYZ(min.gtx()-radius,min.gty()-radius,min.gtz()-radius);
}


//用于展示目前这个盒子的最大，最小点，还有是不是叶子节点，包含了第几条线段的信息。
void AABB::show(){
    cout<<"MinVector:"<<endl;
    min.showXYZ();
    cout<<"MaxVector:"<<endl;
    max.showXYZ();
    cout<<"Is Leaf?: "<<(isLeaf?"Yes":"No");
    cout<<endl;
    cout<<"Which Segment? "<<segNum<<endl;
}
