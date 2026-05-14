#pragma once

void inicializacija_leg(double lege[N][D]){
	int ix, iy;
	FILE*pisi;

	//fopen_s(&pisi,"init_lege.dat","w+");

	pisi = fopen("init_lege.dat","w+");

	int count = 0;

	for(ix = 0; ix < Nx; ix++){
		for(iy = 0; iy < Ny; iy++){
			lege[count][0] = ix*a;
			lege[count][1] = iy*a;
			fprintf(pisi,"%f\t%f\n",lege[count][0],lege[count][1]);
			count++;
		}
	}
	fclose(pisi);
}

double energija_sistema(double xyz[N][D], double* Wv){
	double E = 0;
	double En;
	int i,j;
	double r,dx,dy,dz;

	for(i = 0; i < N; i++){
		Wv[i] = 0;
	}

	double temp_x, temp_y;

	for(i = 0; i < N; i++){
		En = 0;
		for(j = i + 1; j < N; j++){
			dx = (xyz[j][0] - xyz[i][0]);

			if(dx > L/2){
				temp_x = xyz[j][0];
				temp_x -= L;
				dx = (temp_x - xyz[i][0]);
			}

			if(dx < -L/2){
				temp_x = xyz[j][0];
				temp_x += L;
				dx = (temp_x - xyz[i][0]);
			}
			dx = dx*dx;

			dy = (xyz[j][1] - xyz[i][1]);

			if(dy > L/2){
				temp_y = xyz[j][1];
				temp_y -= L;
				dy = (temp_y - xyz[i][1]);
			}

			if(dy < -L/2){
				temp_y = xyz[j][1];
				temp_y += L;
				dy = (temp_y - xyz[i][1]);
			}
			dy = dy*dy;
			r = dy + dx;

			En = pow(r,-6.0)-pow(r,-3.0);
			
			E += 2.0*En;
			Wv[i] += En;
			Wv[j] += En; 

		}
	}
	return E;
}

int rand_int(){
	return rand()%N;
}


// Posodobljena funkcija za premikanje delcev v okolivi delta
double rand_double(double delta){ 
	double r = (double)(rand());
	r = r / ((double)(RAND_MAX));
	r = r - 0.5;
	r = r * 2.0 * delta;
	return r;
}


double randd(){
	return (double)(rand()) / ((double)(RAND_MAX));
}


double update_delta(double delta, double bias){
	if(bias<0.45)	delta *= 0.99;
	if(bias>0.55)	delta *= 1.03;
	return delta;
}


void shrani_lege(double lege[N][D]){
	FILE*pisi;
	pisi = fopen("fin_lege.dat","w+");

	int count = 0, ix, iy;

	for(ix = 0; ix < Nx; ix++){
		for(iy = 0; iy < Ny; iy++){
			fprintf(pisi,"%f\t%f\n",lege[count][0],lege[count][1]);
			count++;
		}
	}
	fclose(pisi);
}

double razdalja_ij(double lege[N][D], int i, int j){
    double dx = lege[i][0] - lege[j][0];
    double dy = lege[i][1] - lege[j][1];

    if(dx >  L/2.0) dx -= L;
    if(dx < -L/2.0) dx += L;
    if(dy >  L/2.0) dy -= L;
    if(dy < -L/2.0) dy += L;

    return sqrt(dx*dx + dy*dy);
}

void gr(double lege[N][D]){
	int i,j;
	double r1 = 0.0;
	double rmax = 7.0;
	double r;
	double nr;
	double dr = rmax/((double)(n_bins));
	int ir;

	double histogram[n_bins] = {0.0};

	for(i = 0; i < N; i++){
		ir = 0;
		r1 = 0.0;
		while(r1 < rmax){
			nr = 0.0;
			for(j = 0; j < N; j++){
				if(j!=i){
					r = razdalja_ij(lege, i, j);
					r = r - r1;
					
					//printf("i = %d, j = %d, r = %f",i,j,r);

					if(r > 0 && r < dr){
						nr+=1.0;
					}
				}
			}

			//nr = nr/(PI*((r1+dr)*(r1+dr)-r1*r1));
			
			//printf("r1 = %f\t nr %f\n",r1,nr);
			
			histogram[ir] += nr;
			r1 += dr;
			ir++;
		}
	}

	for(i = 0; i < n_bins; i++){
		histogram[ir] = histogram[ir]/((double)(N));
		printf("r (%.2f,%.2f), g(r) = %.2f\n",(double)(i)*dr,(double)(i+1)*dr,histogram[ir]);
	}
}


// * MC_bias
double MC_simulator(double lega[N][D], double* E_old, double* Wv, double* MC_bias){ 

	FILE*pisi;
	pisi = fopen("rezutati.dat","w+");

	int izbran_delec;
	int iteracija = 0;
	double E_new, dE;
	int MC = 0;
	int ani_id = 0;
	int ix,iy,iz;

	double xi,yi,zi;
	double bias;

	double delta = a;


	gr(lega);

	while(iteracija < STOP){
		izbran_delec = rand_int();

		xi = lega[izbran_delec][0];
		yi = lega[izbran_delec][1];

		lega[izbran_delec][0] += rand_double(delta);
		lega[izbran_delec][1] += rand_double(delta);

		if (lega[izbran_delec][0]>L) lega[izbran_delec][0] -= L;
		if (lega[izbran_delec][0]<0) lega[izbran_delec][0] += L;
		if (lega[izbran_delec][1]>L) lega[izbran_delec][1] -= L;
		if (lega[izbran_delec][1]<0) lega[izbran_delec][1] += L;

		E_new = energija_sistema(lega, Wv);

		dE = E_new - *E_old; // * E_old

		if(dE<0){
			*E_old = E_new; // * E_old 
			MC += 1;
		}else{
			if(exp(-dE/T) > randd()){
				*E_old = E_new; // * E_old 
				MC += 1;
			}else{
				lega[izbran_delec][0] = xi;
				lega[izbran_delec][1] = yi;
			}
		}
		iteracija += 1;
		bias = (double)(MC) / (double)(iteracija);

		delta = update_delta(delta,bias);

		if(iteracija%N == 0){
			printf("i  = %d\t <E>/N = %.2f\t bias = %.2f %%\t delta = %.3f\n",iteracija,*E_old,bias*100,delta);
			//printf("MC bias = %f\n",bias);
		}

		if(iteracija > EIN){
			fprintf(pisi,"%d\t",iteracija);
			fprintf(pisi,"%f\t",*E_old);
			fprintf(pisi,"%f\t",bias);
			fprintf(pisi,"%f\n",delta);
		}
		
		*MC_bias = bias; // * MC_bias
	}

	fclose(pisi);
	shrani_lege(lega);
	gr(lega);

	return *E_old; // * E_old 
}
