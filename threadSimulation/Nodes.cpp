#include "stdafx.h"
#include"Nodes.h"


ostream& operator <<(ostream& os, const threeVector &a) {
	os << "The vector is : (" << a.node.x << ',' << a.node.y << ',' << a.node.z << ')' << endl;
	os << "Constraint? : " << (a.contactConstraint ? "True" : "False")<< endl;
	os << "numberOfDelta?: " << a.numberOfDelta << endl;
	os << "Is knot?: " << (a.knot ? "True" : "False") << endl;
	return os;
}

threeVector& operator+(const threeVector &a1, const threeVector &a2) {
	threeVector tmp;
	tmp.node.x = a1.node.x + a2.node.x;
	tmp.node.y = a1.node.y + a2.node.y;
	tmp.node.z = a1.node.z + a2.node.z;
	return tmp;
}

threeVector& operator-(const threeVector &a1, const threeVector &a2) {
	threeVector tmp;
	tmp.node.x = a1.node.x - a2.node.x;
	tmp.node.y = a1.node.y - a2.node.y;
	tmp.node.z = a1.node.z - a2.node.z;
	return tmp;

}

threeVector& threeVector::operator * (double times) {
	node.x = node.x * times;
	node.y = node.y * times;
	node.z = node.z * times;
	return *this;

}

void threeVector::copyTraits(const threeVector &a)
{
	contactConstraint = a.contactConstraint;
	numberOfDelta = a.numberOfDelta;
	delta.x = a.delta.x;
	delta.y = a.delta.y;
	delta.z = a.delta.z;
	collisionClusterNum = a.collisionClusterNum;
	knot = a.knot;
}

threeVector& threeVector::operator/(double devisor) {
	node.x = node.x / devisor;
	node.y = node.y / devisor;
	node.z = node.z / devisor;
	return *this;

}

threeVector& threeVector::operator = (const threeVector&a) {
	if (this == &a) return *this;
	node.x = a.node.x;
	node.y = a.node.y;
	node.z = a.node.z;
	return *this;

}


void threeVector::setXYZ(double newX, double newY, double newZ) {
	node.x = newX;
	node.y = newY;
	node.z = newZ;
}

void threeVector::showXYZ() {
	cout << "The vector is : (" << node.x << ',' << node.y << ',' << node.z << ')' << endl;
}

void threeVector::unitlize(){
    double devisor = norm();
    node.x /= devisor;
    node.y /= devisor;
    node.z /= devisor;
}
