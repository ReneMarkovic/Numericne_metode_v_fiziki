#pragma warning (disable : 4996)
void zacetna_postavitev(double xyz[N][3]) {
	int ix, iy,iz;
	int in = 0;
	double da = a / (double)(Nx - 1);
	FILE* pisi = fopen("00_zacetna_konfiguracija.dat", "w+");
	for (ix = 0; ix < Nx; ix++) {
		for (iy = 0; iy < Ny; iy++) {
			for (iz = 0; iz < Nz; iz++) {
				xyz[in][2] = iz * da;//y lega
				xyz[in][1] = iy * da;//y lega
				xyz[in][0] = ix * da;//x lega
				fprintf(pisi, "%f\t%f\t%f\n", xyz[in][0], xyz[in][1], xyz[in][2]);
				in++;
			}
		}
	}
	fclose(pisi);
}

double energija(double xyz[N][3], double* Wv) {
	double E = 0.0;
	double En;
	double r, dx, dy,dz;
	int n1, n2;
	for (n1 = 0; n1 < N; n1++) {
		Wv[n1] = 0.0;
		En=0.0;
		for (n2 = n1 + 1; n2 < N; n2++) {
			dx = lega[n1][0] - lega[n2][0];
			dy = lega[n1][1] - lega[n2][1];
			dz = lega[n1][2] - lega[n2][2];

			if (dx > a) dx -= a;
			if (dx < -a ) dx += a;
			if (dy > a ) dy -= a;
			if (dy < -a ) dy += a;
			if (dz > a) dz -= a;
			if (dz < -a) dz += a;

			r = dx * dx + dy * dy+ dz * dz;
			En = pow(r, -6.0) - pow(r, -3.0);
			E +=En;
			Wv[n1]+=En;
			Wv[n2]+=En;
			//printf("%f\n", E);
		}
	}
	E = 2.0 * E;
	return E;//<povprečno vezavno energijo na delec>
}

double randd() {
	return (double)(rand()) / ((double)(RAND_MAX)) - 0.5;
}

int randint() {
	return (int)(N * (double)(rand()) / ((double)(RAND_MAX)));
}

void MC_simulator(double xyz[N][3], double E_old, double Wv[N]) {
	zacetna_postavitev(xyz);

	//Izberemo nakljucen delec
	int izbrani;
	int st_simulacij = 0;
	double E_new, dE;
	int MC = 0, ani_id = 0;
	int ix, iy, iz, in;
	FILE* pisi = fopen("02_E_T0_1.dat", "w+");
	FILE* ani;
	char ss[40];
	while (st_simulacij < STOP) {
		izbrani = randint();// rand() % N;
		//printf("izbran delec %d [0,%d)\n", izbrani,N);
		double x_i, y_i,z_i;//Za izbran delec si shranimo x in y koordinato
		x_i = xyz[izbrani][0];
		y_i = xyz[izbrani][1];
		z_i = xyz[izbrani][2];

		double da = a / (double)(Nx - 1);//Delec premaknemo
		double dr = 0.5 * da / 2;
		xyz[izbrani][0] += dr * randd();
		xyz[izbrani][1] += dr * randd();
		xyz[izbrani][2] += dr * randd();

		if (xyz[izbrani][0] > a) xyz[izbrani][0] -= a;
		if (xyz[izbrani][0] < 0) xyz[izbrani][0] += a;
		if (xyz[izbrani][1] > a) xyz[izbrani][1] -= a;
		if (xyz[izbrani][1] < 0) xyz[izbrani][1] += a;
		if (xyz[izbrani][2] > a) xyz[izbrani][2] -= a;
		if (xyz[izbrani][2] < 0) xyz[izbrani][2] += a;

		E_new = energija(lega,Wv);
		dE = E_new - E_old;

		if (dE < 0) {
			E_old = E_new;
			MC += 1; //Sprejmem
		}
		else {
			if (exp(-dE / T) > (randd() + 0.5)) {
				E_old = E_new;
				MC += 1; //Sprejmem
			}
			else {//Zavržem
				xyz[izbrani][0] = x_i;
				xyz[izbrani][1] = y_i;
				xyz[izbrani][2] = z_i;
			}
		}
		if ((st_simulacij % 100 == 0)) {	
			printf("<E>/N = %.2f\tMC=%d\ti=%.3f\r", E_old / ((double)N - 1), MC, (double)st_simulacij/((double)STOP));
			fflush(stdout);
			sprintf(ss, "data\\ani_%05d.dat", ani_id);

			ani = fopen(ss, "w+");
			in = 0;
			for (in = 0; in < N-1; in++) {
					fprintf(ani, "%f\t%f\t%f\t%f\n", xyz[in][0], xyz[in][1], xyz[in][2],Wv[in]);
			}
			fclose(pisi);
			ani_id += 1;
		}
		st_simulacij += 1;
		if (st_simulacij > int(EIN * STOP)) {
			fprintf(pisi, "%d\t%d\t%f\n", st_simulacij, MC, E_old / ((double)N - 1));
		}
	}
	fclose(pisi);


	in = 0;
	pisi = fopen("01_koncna_konfiguracija.dat", "w+");
	for (ix = 0; ix < Nx; ix++) {
		for (iy = 0; iy < Ny; iy++) {
			for (iz = 0; iz < Nz; iz++) {
				fprintf(pisi, "%f\t%f\t%f\n", xyz[in][0], xyz[in][1], xyz[in][2]);
				in++;
			}
		}
	}
	fclose(pisi);
}