#pragma once

double **read_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Napaka: Ne morem odpreti datoteke %s\n", filename);
        return NULL;
    }

    // Allocate memory for the 2D array
    double **data = (double **)malloc(65536 * sizeof(double *));
    for (int i = 0; i < 65536; i++) {
        data[i] = (double *)malloc(2 * sizeof(double));
    }

    // Read the file and populate the 2D array
    int row = 0;
    while (!feof(file)) {
        double x, y;
        if (fscanf(file, "%lf\t%lf", &x, &y) != 2) {
            break;
        }
        data[row][0] = x;
        data[row][1] = y;
        row++;
    }

    fclose(file);
    return data;
}


int get_info(bool show){
	if (show) {
		printf("####################################\n");
		printf("#    Frekvencna analiza signala    #\n");
		printf("#            avtor: R.M.           #\n");
		printf("####################################\n");

		printf("Star = %.3f s\tEnd = %.3f\n", T_START, T_END);
		printf("Sampling rate = %.2f samples/second\n", FS);
		printf("Number of samples = %d \n", N);
		printf("\n");
		return 0;
	}else {
		return 0;
	}
}

double f(double* casovna_vrsta, double omega1, double omega2, double x0) {
	double t = 0;
	for (int i = 0; i < N; i++) {
		casovna_vrsta[i] = x0 * (sin(omega1 * t) + 0.25 * sin(omega2 * t));
		t += DT;
	}
	return 0.0;
}

void zapisi_casovno_vrsto(double* casovno_vrsta){
	FILE* data = NULL;
	fopen_s(&data, "caosvna_vsta.txt", "w+");
	double t = 0;
	for (int i = 0; i < N; i++) {
		fprintf(data, "%f\t%f\n", t, casovno_vrsta[i]);
		t += DT;
	}
	fclose(data);
}

double** DFT(double* casovna_vrsta) {
	int stolpci = 2;
	double** matrix;

	FILE* data = NULL;
	FILE* data2 = NULL;

	fopen_s(&data,"DFT_power_spectrum.txt", "w+");
	//fopen_s(&data2, "DFT_matrix.txt", "w+");

	matrix = (double**)malloc(N * sizeof(double*));
	for (int i = 0; i < stolpci; i++) {
		matrix[i]= (double*)malloc(N * sizeof(double));
	}

	int k, j;
	double omega_n = 2.0 * PI / N;
	for (k = 0; k < N; k++) {
		double real = 0.0;
		double imag = 0.0;
		for (j = 0; j < N; j++) {
			real += cos(omega_n * j * k) * casovna_vrsta[j];
			imag -= sin(omega_n * j * k) * casovna_vrsta[j];
		}
		matrix[0][k] = real;
		matrix[1][k] = imag;
	}

	double p;
	for (int i = 0; i < N/2; i++) {
		p = sqrt(matrix[0][i] * matrix[0][i] + matrix[1][i] * matrix[1][i]);
		fprintf(data, "%d\t%f\t%f\t%f\t%f\n", i, i/(N*DT), matrix[0][i], matrix[1][i], p);
	}
	fclose(data);
	return matrix;
}

typedef struct {
	double real;
	double imag;
} Complex;

Complex complex_add(Complex a, Complex b) {
	/*
	s1 = a1 + ib1;
	s2 = a2 + ib2;
	s3 = s1 + s2;
	s3 = (a1+a2) + i(b1+b2)
	*/
	Complex r;
	r.real = a.real + b.real;
	r.imag = a.imag + b.imag;
	return r;
}

Complex complex_sub(Complex a, Complex b) {
	/*
	s1 = a1 + ib1;
	s2 = a2 + ib2;
	s3 = s1 - s2;
	s3 = (a1-a2) + i(b1-b2)
	*/
	Complex r;
	r.real = a.real - b.real;
	r.imag = a.imag - b.imag;
	return r;
}

double complex_magnitude(Complex a) {
	/*
	s1 = a + ib;
	|s1| = sqrt(a*a + b*b);
	*/
	double m = sqrt(a.real * a.real * +a.imag * a.imag);
	return m;
}

double complex_phase(Complex a) {
	/*
	s1 = a + ib;
	phase = atan(real/imag);
	*/
	double phase=atan(a.real/a.imag);
	return phase;
}

Complex complex_neg(Complex a) {
	/*
	s1 = a + i*b;
	s2=complex_neg(s1);
	s2=a - i*b
	*/
	Complex b;
	b.real = a.real;
	b.imag = -a.imag;
	return b;
}


/*
Drugi del vaj
*/

Complex complex_ustvari(double r, double i) {
	/*
	r- realni del kompleksnega �tevila
	i- imaginarni del kompleksnega �tevila
	*/
	Complex c;
	c.real = r;
	c.imag = i;
	return c;
}

Complex complex_množenje(Complex a, Complex b) {
	Complex c;
	c.real = a.real * b.real - a.imag * b.imag;
	c.imag = a.real * b.imag + a.imag * b.real;
	return c;
}

#define SWAP(a,b)tempr=(a);(a)=(b);(b)=tempr

void FFT(double* casovna_vrsta, Complex* izhod, int n) {
	//printf("Pricenjam FFT\n");
	int m = log2(n);
	int i, j, k, n1, n2;
	double c, s, tempr, tempi;

	// Bit reversal
	//printf("Bit reverzija\n");
	j = 0;
	n2 = n / 2;
	for (i = 1; i < n - 1; i++) {
		n1 = n2;
		while (j >= n1) {
			j = j - n1;
			n1 = n1 / 2;
		}
		j = j + n1;
		//printf(" i = %d  | n1  =  %d  |  j = % d\n", i, n1, j);
		if (i < j && j < N) {
			//printf("SWAP (%f) <-> (%f)\n\n", casovna_vrsta[i], casovna_vrsta[j]);
			SWAP(casovna_vrsta[i], casovna_vrsta[j]);
		}
	}

	// Izra�un kompleksnega FFT
	//printf("Kompleksni array a\n");
	for (i = 0; i < n; i++) {
		izhod[i].real = casovna_vrsta[i];
		izhod[i].imag = 0.0;
	}
	

	Complex t, u;
	int k1;

	//printf("For zanaka \n");
	for (i = 0; i < m; i++) {
		n1 = 1 << i;
		n2 = n1 << 1;
		for (j = 0; j < n1; j++) {
			c = cos(-2.0 * PI * j / n2);
			s = sin(-2.0 * PI * j / n2);
			for (k = j; k < n; k = k + n2) {
				k1 = k + n1;
				t = complex_množenje(izhod[k1], complex_ustvari(c, s));
				u = izhod[k];
				izhod[k] = complex_add(u, t);
				izhod[k1] = complex_sub(u, t);
			}
		}
	}
}





