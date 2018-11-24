#include "stdafx.h"
#include "DistanceOFsegment.h"
bool IsEqual(double d1, double d2){
	if (abs(d1 - d2) < 1e-7)
		return true;
	return false;
}

threeVector distanceOFSegment(threeVector &node_1, threeVector &node_2, threeVector &node_3, threeVector &node_4) {

	double x1 = node_1.gtx(), x2 = node_2.gtx(), x3 = node_3.gtx(), x4 = node_4.gtx();
	double y1 = node_1.gty(), y2 = node_2.gty(), y3 = node_3.gty(), y4 = node_4.gty();
	double z1 = node_1.gtz(), z2 = node_2.gtz(), z3 = node_3.gtz(), z4 = node_4.gtz();

	double ux = x2 - x1;
	double uy = y2 - y1;
	double uz = z2 - z1;

	double vx = x4 - x3;
	double vy = y4 - y3;
	double vz = z4 - z3;

	double wx = x1 - x3;
	double wy = y1 - y3;
	double wz = z1 - z3;

	double a = (ux * ux + uy * uy + uz * uz); //u*u
	double b = (ux * vx + uy * vy + uz * vz); //u*v
	double c = (vx * vx + vy * vy + vz * vz); //v*v
	double d = (ux * wx + uy * wy + uz * wz); //u*w
	double e = (vx * wx + vy * wy + vz * wz); //v*w
	double dt = a * c - b * b;

	double sd = dt;
	double td = dt;

	double sn = 0.0;
	double tn = 0.0;

	if (IsEqual(dt, 0.0)) {
		//两直线平行
		sn = 0.0;
		sd = 1.00;
		tn = e;
		td = c;
	}
	else {
		sn = (b * e - c * d);
		tn = (a * e - b * d);
		if (sn < 0.0) {

			sn = 0.0;
			tn = e;
			td = c;
		}
		else if (sn > sd) {
			sn = sd;
			tn = e + b;
			td = c;
		}
	}
	if (tn < 0.0) {
		tn = 0.0;
		if (-d < 0.0)
			sn = 0.0;
		else if (-d > a)
			sn = sd;
		else {
			sn = -d;
			sd = a;
		}
	}
	else if (tn > td) {
		tn = td;
		if ((-d + b) < 0.0)
			sn = 0.0;
		else if ((-d + b) > a)
			sn = sd;
		else {
			sn = (-d + b);
			sd = a;
		}
	}

	double sc = 0.0;
	double tc = 0.0;

	if (IsEqual(sn, 0.0))
		sc = 0.0;
	else
		sc = sn / sd;

	if (IsEqual(tn, 0.0))
		tc = 0.0;
	else
		tc = tn / td;

	double dx = wx + (sc * ux) - (tc * vx);
	double dy = wy + (sc * uy) - (tc * vy);
	double dz = wz + (sc * uz) - (tc * vz);

    threeVector u_distance;
    u_distance.setXYZ(dx,dy,dz);

    return u_distance;
}
