/*************************************************************************
    > File Name: Ordercompute
    > Author: 崔德旭(cover:htliang)
    > Created : 2021年05月13日 星期四 于 研究生东馆C204
    > Introduction : 这是一个能够批量计算序参量的程序，对lammps输出的格式有一定要求	                
					（除此以外，通过此程序可以输出任何需求的参数（自定义），并由OVITIO进行可视化）！！！！
					（修改运算的路径，运行步数，步长,倒格矢）
 ************************************************************************/

#include "batch_processing_orderparameter.h"
#include<iostream>
#include<ctime>
#include<fstream>
using namespace std;
#include<math.h>

#define MAXNATOMS 1000000

#define M_PI 3.1415926535

#define running_steps 100 

double info_atom[MAXNATOMS][3],atom_id[MAXNATOMS],atom_type[MAXNATOMS],Property[MAXNATOMS][2];

int EndTime=150000;


int main()
{

	clock_t starttime,endtime;

	char tmp[100];
	
	int time,natoms,id,type;
	
	double x,y,z,xu,yu,zu,xlo,xhi,ylo,yhi,zlo,zhi,c_1,c_2,v_Temp;
	
	double cutoff = 3.0;
	
	double lattice = 3.64;
	
	double q[][3]={	2*M_PI/lattice,2*M_PI/lattice,-2*M_PI/lattice,
											-2*M_PI/lattice,2*M_PI/lattice,2*M_PI/lattice,
										2*M_PI/lattice,-2*M_PI/lattice,2*M_PI/lattice};
	
	
	OrderMorris Order;
	Order.setCutoff(lattice);
	Order.setRLattice(q[0][0],q[0][1],q[0][2],q[1][0],q[1][1],q[1][2],q[2][0],q[2][1],q[2][2]);
	

	ofstream fout;

	ofstream ffout;

ffout.open("Ni75Cu25_300K_order_1.txt");

fout.open("Ni75Cu25_300K_1.txt");
	
	ifstream fin;

fin.open("Ni75Cu25_300K_1.lammps"); 
	
    time = 0;

	int kk = 0;

	starttime = clock();

	for(int r=0;r<running_steps+1;r++) {
		
		fin.getline(tmp,100);

		fin >> time;		fin.getline(tmp,100);

		cout << time << endl;

		fin.getline(tmp,100);
		fin >> natoms;	fin.getline(tmp,100);
	
		fin.getline(tmp,100);
		fin >> xlo >> xhi; fin.getline(tmp,100);
		fin >> ylo >> yhi; fin.getline(tmp,100);
		fin >> zlo >> zhi; fin.getline(tmp,100);
		fin.getline(tmp,100);
		
		Order.setBoundary(xlo,xhi,ylo,yhi,zlo,zhi,PERIODICX);

		
		int count = 0;
		
		for (int i = 0; i < natoms; i++) {
			
			fin >> id >> type >> x >> y >> z>>v_Temp; fin.getline(tmp,100);

				atom_id[count] = id;
				atom_type[count] = type;
			    info_atom[count][0] = x;
				info_atom[count][1] = y;
				info_atom[count][2] = z;
				//Property[count][0] = c_1;
				//Property[count][1] = c_2;
				
				count++;

			}

	//if (r==10){ 
	//
		fout <<"ITEM: TIMESTEP"<< endl;
		fout << time << endl;
		fout <<"ITEM: NUMBER OF ATOMS"<< endl;
		fout << natoms << endl;
		fout <<"ITEM: BOX BOUNDS pp pp pp"<< endl;
		fout << xlo <<' '<< xhi << endl;
		fout << ylo <<' '<< yhi << endl;
		fout << zlo <<' '<< zhi << endl;
     	fout <<"ITEM: ATOMS id type x y z LOP"<< endl;

		Order.compute(info_atom,natoms); 

		 //for (int j = 0; j < natoms; j++){
			//
			///*fout<< atom_id[j]<<" "<<atom_type[j]<<" "<<info_atom[j][0]<<" "<<info_atom[j][1]<<" "<<info_atom[j][2] <<" "
			//	
			//	
			//	<<Property[j][0]<<" "<< Property[j][1] <<" "<<Order.out(j,1) <<endl;*/

			//fout<< atom_id[j]<<" "<<atom_type[j]<<" "<<info_atom[j][0]<<" "<<info_atom[j][1]<<" "<<info_atom[j][2] <<endl;
			//	
			//	
		 //}

     	 

		

		 for (int i = 0; i < natoms; i++){
			
			ffout<< info_atom[i][2] <<" "<<Order.out(i,1)<<endl;
		
		}


		 for (int i = 0; i< natoms; i++){
		 
		 fout << atom_id[i] <<" "<<atom_type[i]<<" "<<info_atom[i][0]<<" "<<info_atom[i][1]<<" "<<info_atom[i][2]<<" "<< Order.out(i,1)<< endl;
		 
		 }

		 
	//}
	}

	//}
	fin.close();

	fout.close();
    
    ffout.close();
	
	cout << "complete"<<" "<<endl;

	endtime = clock();

	cout<<"tiem-consuming"<<": "<<double(endtime-starttime)/CLOCKS_PER_SEC/60<<" "<<"min";

	system("pause");

	return 0;
}