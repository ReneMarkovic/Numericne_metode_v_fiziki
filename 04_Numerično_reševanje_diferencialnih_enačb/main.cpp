// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double x, y_e,y_rk4, h;

//Parametri prostega pada
#define rho_z 10.0
#define rho_atm 1.29
#define g 10.0
#define r 0.05
#define PI 4.0*atan(1)
#define S 4.0*PI*r*r
#define V 4.0/3.0*PI*r*r*r
#define m_z rho_z*V

#define c 1.0
#define k 0.5*rho_atm*c*S/m_z

#include "solve_ode.h"

int main(){
	
	//Parametri
	/*x = 0;
	y_e = 1;
	y_rk4 = 1;
	h = 0.1;*/

	FILE* pisi;
	double v_rk4 = 0.0;
	double v_e = 0.0;
	double dt = 0.1; //Veliko
	double vt = sqrt((2 * g * m_z) / (rho_atm * c * S));
	double t = 0.0;
	fopen_s(&pisi, "rezultati_vaje_04_pt2.txt", "w+");
	fprintf(pisi, "%f\t%f\t%f\t%f\n", t, v_rk4, v_e, vt);
	double Fu,Fg;
	while (t <= 120.0){
		v_e += newton_euler_dynamics(v_e, dt);//ode_RK4_solver(x, y_rk4, h);
		v_rk4 += newton_RK4_dynamics(v_rk4, dt);//ode_Euler_solver(x, y_e, h);

		Fu = 0.5 * S * c * rho_atm * v_e * v_e;
		Fg = m_z * g;
		printf("t = %f\tv = %f\ta = %f\tFu = %f\t Fg = %f\n",t,v_e,a(v_e),Fu,Fg);
		t += dt;

		fprintf(pisi, "%f\t%f\t%f\t%f\n", t, v_rk4, v_e, vt);
	}

	printf("%f\t%f\t%f\t%f\n", t, v_rk4, v_e, vt);

	fclose(pisi);
	return 0;
}