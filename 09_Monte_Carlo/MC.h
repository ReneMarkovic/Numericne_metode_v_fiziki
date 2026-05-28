#pragma once

void T_to_str(char* buf){
	int k;
	sprintf(buf, "%.3f", T);
	for(k=0; buf[k]; k++) if(buf[k]=='.') buf[k]='_';
}

void inicializacija_leg(double lege[N][D]){
	int ix, iy;
	FILE*pisi;
	char fname[64]; char tstr[32]; T_to_str(tstr);
	sprintf(fname, "pos_init_%s.dat", tstr);
	pisi = fopen(fname,"w+");

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

			double r3i = 1.0/(r*r*r);
			En = r3i*r3i - r3i;
			
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
	if(bias<0.45)	delta *= 0.993;
	if(bias>0.55)	delta *= 1.003;
	return delta;
}


void shrani_lege(double lege[N][D]){
	FILE*pisi;
	char fname[64]; char tstr[32]; T_to_str(tstr);
	sprintf(fname, "pos_final_%s.dat", tstr);
	pisi = fopen(fname,"w+");

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

double energija_delca(double xyz[N][D], int k){
	double E = 0.0;
	double dx, dy, r, r3i;

	for(int j = 0; j < N; j++){
		if(j == k) continue;

		dx = xyz[j][0] - xyz[k][0];
		if(dx >  L/2.0) dx -= L;
		if(dx < -L/2.0) dx += L;

		dy = xyz[j][1] - xyz[k][1];
		if(dy >  L/2.0) dy -= L;
		if(dy < -L/2.0) dy += L;

		r = dx*dx + dy*dy;
		r3i = 1.0/(r*r*r);
		E += r3i*r3i - r3i;
	}
	return E;
}

void gr(double lege[N][D], double* histogram, int n_configs, bool write){
	int i, j, ir;
	double r;
	const double rmax = 7.0;
	const double dr = 0.2;
	const double rho = (double)(N) / (L*L);

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			if(j == i) continue;
			r = razdalja_ij(lege, i, j);
			ir = (int)(r / dr);
			if(ir < n_bins) histogram[ir] += 1.0;
		}
	}

	if(write){
		FILE* pisi;
		char fname[64]; char tstr[32]; T_to_str(tstr);
		sprintf(fname, "gr_%s.txt", tstr);
		pisi = fopen(fname, "w+");
		for(i = 0; i < n_bins; i++){
			double r1 = i * dr;
			double shell_area = 3.14159265358979 * ((r1+dr)*(r1+dr) - r1*r1);
			double g = histogram[i] / ((double)(n_configs) * (double)(N) * rho * shell_area);
			fprintf(pisi, "%.3f\t%.6f\n", r1 + 0.5*dr, g);
			printf("r = %.3f\tg(r) = %.3f\n",r1,g);
		}
		printf("Shranjeno v datoteko %s\n",fname);
		fclose(pisi);
	}
}


// * MC_bias
double MC_simulator(double lega[N][D], double* E_old, double* Wv, double* MC_bias){ 

	FILE*pisi;
	char fname[64]; char tstr[32]; T_to_str(tstr);
	sprintf(fname, "energije_%s.dat", tstr);
	pisi = fopen(fname,"w+");
	double histogram[n_bins] = {0.0};
	int n_configs = 0;
	int izbran_delec;
	int iteracija = 0;
	double dE, E_old_k, E_new_k;
	int MC = 0;

	double xi, yi;
	double bias;

	double delta = a;

	int rec = 0;
	while(iteracija < STOP){
		izbran_delec = rand_int();

		xi = lega[izbran_delec][0];
		yi = lega[izbran_delec][1];

		E_old_k = energija_delca(lega, izbran_delec);

		lega[izbran_delec][0] += rand_double(delta);
		lega[izbran_delec][1] += rand_double(delta);

		if (lega[izbran_delec][0]>L) lega[izbran_delec][0] -= L;
		if (lega[izbran_delec][0]<0) lega[izbran_delec][0] += L;
		if (lega[izbran_delec][1]>L) lega[izbran_delec][1] -= L;
		if (lega[izbran_delec][1]<0) lega[izbran_delec][1] += L;

		E_new_k = energija_delca(lega, izbran_delec);
		dE = 2.0 * (E_new_k - E_old_k);

		if(dE<0){
			*E_old += dE;
			MC += 1;
		}else{
			if(exp(-dE/T) > randd()){
				*E_old += dE;
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
			if(iteracija>EIN){
				gr(lega, histogram, ++n_configs, false);
			}
			//printf("MC bias = %f\n",bias);
		}

		
		fprintf(pisi,"%d\t",iteracija);
		fprintf(pisi,"%f\t",*E_old);
		fprintf(pisi,"%f\t",bias);
		fprintf(pisi,"%f\t",delta);
		fprintf(pisi,"%d\n",rec);

		if(iteracija > EIN){
			rec = 1;
		}
		
		*MC_bias = bias; // * MC_bias
	}

	fclose(pisi);
	shrani_lege(lega);
	gr(lega, histogram, ++n_configs, true);
	return *E_old; // * E_old 
}
