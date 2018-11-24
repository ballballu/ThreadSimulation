#pragma once

#include<iostream>
#include<math.h>

using namespace std;

struct Node {
	double x, y, z;
};

class threeVector {

	friend ostream& operator<<(ostream& os, const threeVector& a);
	friend threeVector& operator+ (const threeVector &a1, const threeVector &a2);
	friend threeVector& operator- (const threeVector &a1, const threeVector &a2);

private:
	Node node;
	bool contactConstraint;
	int numberOfDelta;
	Node delta;
	int collisionClusterNum;
	bool knot;

public:
	threeVector(double a = 0, double b = 0, double c = 0) {
		node.x = a;
		node.y = b;
		node.z = c;
		contactConstraint = false ;
		numberOfDelta = 0 ;
		delta.x = delta.y = delta.z = 0;
		collisionClusterNum = 0;
		knot = false;
	};

	threeVector& operator = (const threeVector &a);
	void setXYZ(double newX, double newY, double newZ);
	void showXYZ();
	double gtx() { return node.x; };//获取x，y，z坐标的值；
	double gty() { return node.y; };
	double gtz() { return node.z; };
	double norm() { return sqrt(node.x*node.x+node.y*node.y+node.z*node.z);}

	//碰撞簇
	int getCluster() { return collisionClusterNum; };
	void setCluster(int i) { collisionClusterNum = i; };
	void clearCluster() { collisionClusterNum = 0; };
	//打结识别
	bool isKnot() { return knot; };
	void setKnot() { knot = true; };

	threeVector& operator / (double);
	threeVector& operator * (double);
	void copyTraits(const threeVector &a);
	void unitlize();
	void setConstraint () { contactConstraint = true ; };
	void removeConstraint () { contactConstraint = false ;};
	bool isConstraint () { return contactConstraint ; }

	void addToDelta_Plus(threeVector input){

	    delta.x += input.gtx();
		delta.y += input.gty();
		delta.z += input.gtz();

	    numberOfDelta += 1 ;
	}

	void addToDelta_Minus(threeVector input){

	    delta.x -= input.gtx();
		delta.y -= input.gty();
		delta.z -= input.gtz();

	    numberOfDelta += 1 ;
	}

	void enableDelta(){
	    if (numberOfDelta == 0 )
	        return;
        else{
            node.x = node.x + delta.x/2;
            node.y = node.y + delta.y/2;
            node.z = node.z + delta.z/2;
        }
	}

	void clearDelta() {
		numberOfDelta = 0;
		delta.x = delta.y = delta.z = 0;
	}
};
