#ifndef ORDER_MORRIS_H
#define ORDER_MORRIS_H

#include<iostream>
#include<math.h>

using namespace std;

#define NATOMS_MAX 1000000
#define NEIGHBOR_MAX 100

#define PERIODICX 1 << 0
#define PERIODICY 1 << 1
#define PERIODICZ 1 << 2

class OrderMorris {
 public:
	 int setCutoff(double);
	 int setBoundary(double ,double ,double ,double ,double ,double ,int );
	 int setRLattice(double ,double ,double ,double ,double ,double ,double ,double ,double);
	 int compute(double (*)[3],int);
	 double out(int ,int );
 protected:
	 double order[NATOMS_MAX][2];
	 int NNeighbors[NATOMS_MAX];
	 int Natoms;
	 int neighbor_list[NATOMS_MAX][NEIGHBOR_MAX];
	 int setRLattice_flag;
	 double RLattice[3][3];
	 int setBoundary_flag,PBCondition;
	 double xlo,xhi,ylo,yhi,zlo,zhi,Lx,Ly,Lz;
	 int setCutoff_flag;
	 double cutoff,cutoffsq;
};

int OrderMorris::setBoundary(double xl,double xh,double yl,double yh,double zl,double zh,int condition) {
	if (xh < xl) {cout <<"xh < xl"<< endl; return 1;}
	if (yh < yl) {cout <<"yh < yl"<< endl; return 1;}
	if (zh < zl) {cout <<"zh < zl"<< endl; return 1;}
	xlo = xl; xhi = xh;
	ylo = yl; yhi = yh;
	zlo = zl; zhi = zh;
	Lx = xhi - xlo;
	Ly = yhi - ylo;
	Lz = zhi - zlo;
	PBCondition = condition;
	setBoundary_flag  = 1;
	return 0;
}

int OrderMorris::setCutoff(double cut) {
	cutoff = cut;
	cutoffsq = cut * cut;
	setCutoff_flag = 1;
	return 0;
}

int OrderMorris::setRLattice(double a00,double a01,double a02,double a10,double a11,double a12,double a20,double a21,double a22) {
	RLattice[0][0] = a00;
	RLattice[0][1] = a01;
	RLattice[0][2] = a02;
	RLattice[1][0] = a10;
	RLattice[1][1] = a11;
	RLattice[1][2] = a12;
	RLattice[2][0] = a20;
	RLattice[2][1] = a21;
	RLattice[2][2] = a22;
	setRLattice_flag = 1;
	return 0;
}
#define NINT(a) ((a) >= 0.0 ? (int)((a)+0.5) : (int)((a)-0.5))
int OrderMorris::compute(double (*atom)[3],int n) {
	Natoms = n;
	if (setBoundary_flag != 1) {cout <<"setBoundary is not set"<< endl; return 1;}
	if (setCutoff_flag != 1) {cout <<"setCutoff is not set"<< endl; return 1;}
	if (setRLattice_flag != 1) {cout <<"setRLattice is not set"<< endl; return 1;}
	if (Natoms > NATOMS_MAX) {cout <<"Natoms > NATOMS_MAX"<< endl; return 1;}

	double dx,dy,dz,dxsq,dysq,dzsq,rsq;

	// build neighbor list   
	for (int i = 0; i < Natoms; i++)
		NNeighbors[i] = 0;
	for (int i = 0; i < Natoms; i++) { 
		for (int j = i+1; j < Natoms; j++) {
			dx = atom[i][0] - atom[j][0];
			if (PBCondition & PERIODICX) dx -= NINT(dx/Lx)*Lx;
			dxsq = dx * dx;
			if (dxsq > cutoffsq) continue;
			dy = atom[i][1] - atom[j][1];
			if (PBCondition & PERIODICY) dy -= NINT(dy/Ly)*Ly;
			dysq = dy * dy;
			if (dysq > cutoffsq) continue;
			dz = atom[i][2] - atom[j][2];
			if (PBCondition & PERIODICZ) dz -= NINT(dz/Lz)*Lz;
			dzsq = dz * dz;
			if (dzsq > cutoffsq) continue;
			rsq = dxsq + dysq + dzsq;
			if (rsq > cutoffsq) continue;
			neighbor_list[i][NNeighbors[i]] = j;  //exchange
			NNeighbors[i]++;
			neighbor_list[j][NNeighbors[j]] = i;
			NNeighbors[j]++;
		}
	}
	// order compute
	for (int i = 0; i < Natoms; i++) {
		double cossum = 0, sinsum = 0;
		int j;
		for (j = 0; j < NNeighbors[i]; j++) {                  //原子总数=邻域的总数
			dx = atom[neighbor_list[i][j]][0] - atom[i][0];
			dy = atom[neighbor_list[i][j]][1] - atom[i][1];
			dz = atom[neighbor_list[i][j]][2] - atom[i][2];
			// periodic boundary conditions 
			if (PBCondition & PERIODICX) dx -= NINT(dx/Lx)*Lx;
			if (PBCondition & PERIODICY) dy -= NINT(dy/Ly)*Ly;
			if (PBCondition & PERIODICZ) dz -= NINT(dz/Lz)*Lz;
			cossum += cos(RLattice[0][0]*dx + RLattice[0][1]*dy + RLattice[0][2]*dz) + \
								cos(RLattice[1][0]*dx + RLattice[1][1]*dy + RLattice[1][2]*dz) + \
								cos(RLattice[2][0]*dx + RLattice[2][1]*dy + RLattice[2][2]*dz);
			sinsum += sin(RLattice[0][0]*dx + RLattice[0][1]*dy + RLattice[0][2]*dz) + \
								sin(RLattice[1][0]*dx + RLattice[1][1]*dy + RLattice[1][2]*dz) + \
								sin(RLattice[2][0]*dx + RLattice[2][1]*dy + RLattice[2][2]*dz);
		}
		if (j > 0) {cossum /= 3*j; sinsum /= 3*j;}
		order[i][0] = cossum*cossum + sinsum*sinsum;
	}
	// average order compute
	for (int i = 0; i < Natoms; i++) {
		order[i][1] = order[i][0];
		int j;
		for (j = 0; j < NNeighbors[i]; j++) 
			order[i][1] += order[neighbor_list[i][j]][0];
		order[i][1] /= j+1;
	}
	return 0;
}

double OrderMorris::out(int i,int j) {
	if (i < Natoms && j < 2)
		return order[i][j];
	else {
		cout <<"error for i or j "<< i <<" "<< j << endl;
		return 0;
	}
}

#endif