# FFT

Ta koda je dopolnjena z algoritmom za računanje FFT. Tako je sedaj datoteka *Freq_ana.h* opremljena dodatno še s funkcijo za računanje frekvenčnega spektra z metodo FFT.

## DFT algoritem

Na predhodnih vajah smo podrobneje spoznali algoritem za **D**iskretno **F**urierjevo **T**ransformacijo (DFT). Uporabili smo ga, da smo numerično upodobljeno časovno vrsto določili njen frekvenčni spekter. Klučni parametri programa so bili podani kot:
```c
#define T_START 0.0 //sec
//#define T_END 101.0 //sec
#define pp 15.0
#define N (int)(pow(2,pp))
#define FS 10.0 //meritev na sekundo
//#define N (int)((T_END-T_START)*FS) 
#define T_END (double)N/FS + TS
#define DT 1/FS
#define T_END 101.0 //sec
```

Uporabili smo funkcijo:
```c
double x0 = 1.0;
double f1 = 1 / 10.0, f2 = 1 / 4.0;
double o1 = 2.0 * PI * f1;
double o2 = 2.0 * PI * f2;


double f(double* casovna_vrsta, double omega1, double omega2, double x0) {
	double t = 0;
	for (int i = 0; i < N; i++) {
		casovna_vrsta[i] = x0 * (sin(omega1 * t) + 0.25 * sin(omega2 * t));
		t += DT;
	}
	return 0.0;
}
```

kjer je casovna_vrsta polje, ki ga poselimo z izračunanimi vrednostmi. To časovno vrsto vodimo v funkcijo DFT, ki nam vrne matriko.Matrika, ki jo pridobimo z metodo DFT je kvadratna in dimenzij $N$ x $N$, kjer je $N$ dolžina signala ali število vzorcev. Vsak element matrike DFT predstavlja kompleksno amplitudo signala na določeni frekvenci. Komputacijska zahtevnost je podnaa v spodnji tabeli.

| pp | N     | DFT \[seconds\] |
|:--:|:-----:|:---------------:|
| 8  | 256   | 0.045           |
| 9  | 512   | 0.099           |
| 10 | 1024  | 0.161           |
| 11 | 2048  | 0.692           |
| 12 | 4096  | 2.130           |
| 13 | 8192  | 8.179           |
| 14 | 16384 | 29.14           |
| 15 |       |                 |

Opazimo lahko, da se prične komputacijski čas zelo hitro povečevati in tako postane ppri daljših signalih analiza zelo zamudna. Zaradi omenjenega si lahko pomagamo z metodo FFT.

## FFT algoritem

V datoteko *freq_ana.h* so dodane naslednje funkcije:
```c
#define SWAP(a,b)tempr=(a);(a)=(b);(b)=tempr 
void FFT(double* casovna_vrsta, Complex* izhod, int n) { 
    int m = log2(n); int i, j, k, n1, n2;
    double c, s, tempr, tempi;
    j = 0; n2 = n / 2;
    for (i = 1; i &lt; n - 1; i++){
        n1 = n2;
        while (j &gt;= n1){
            j = j - n1;
            n1 = n1 / 2;
        }
        j = j + n1;
        if (i &lt; j && j < N) {
            SWAP(casovna_vrsta[i], casovna_vrsta[j]);
        }
    }
    for (i = 0; i < n; i++){
        izhod[i].real = casovna_vrsta\[i]; izhod[i].imag = 0.0;
    }
    Complex t, u; int k1; 
    for (i = 0; i < m; i++){
        n1 = 1 << i;
        n2 = n1 << 1;
        for (j = 0; j < n1; j++){
            c = cos(-2.0 * PI * j / n2);
            s = sin(-2.0 * PI * j / n2);
            for (k = j; k < n; k = k + n2){
                k1 = k + n1;
                t = complex_množenje(izhod[k1], complex_ustvari(c, s));
                u = izhod[k]; izhod[k] = complex_add(u, t); izhod[k1] = complex_sub(u, t);
            }
        }
    }
}
```
Kot vidimo algoritem FFT ne vrne ničesar. Rezultati se zapišejo v polje Complex* izhod. Krovni program, main.c, je tako sedaj oblikovan kot:
```c
int main(){
	get_info(true);
    
    //Ustvarimo časovno vrsto
	double* y = (double*)malloc(N * sizeof(double));
	f(y,o1,o2,x0);
	zapisi_casovno_vrsto(y);
    
    //Zagon DFT
	start = clock();
	double** matrix;
	matrix = DFT(y);
	free(matrix);
	end = clock();
	cpu_time_dft = ((double)(end - start)) / CLOCKS_PER_SEC;
	//printf("DFT je za analizo %d podatkov potreboval %f s\n", N, cpu_time);

    
    //Zagon FFT
	start = clock();
	Complex* izhod = (Complex*)malloc(N * sizeof(Complex));
	FFT(y, izhod, N);
	end = clock();
	cpu_time_fft = ((double)(end - start)) / CLOCKS_PER_SEC;
	//printf("DFT je za analizo %d podatkov potreboval %f s\n", N, cpu_time);
    
    //Primerjamo čas računanja
	printf("%d\t%d\t%f\t%f\n", (int)(pp), (int)(N), cpu_time_dft, cpu_time_fft);
    
    //Zapišemo razultate
	FILE* file = NULL;
	fopen_s(&file, "FFT.txt", "w+");
	for (int i = 0; i < N; i++) {
		double freq = (double)i / (N * DT); // Calculate the frequency
		fprintf(file, "%f\t%f\t%f\ti%f\n", freq, complex_magnitude(izhod[i]), izhod[i].real, izhod[i].imag);
	}
    
    //Sprostimo delavni pomnilnik
	fclose(file);
	free(y);
	free(izhod);

	return 0;
}
```

### Primerjava komputacijske zahtevnosti

| pp |    N  |	DFT   |	FFT   | Ratio | rel_delta_DFT |	rel_delta_FFT |
|:--:|:-----:|:------:|:-----:|:-----:|:-------------:|:-------------:|
| 8  | 256   |	0.045 |	0     |    /  |      /        |       /       |
| 9  | 512   |	0.099 |	0.001 |	  99  |	    2.2       |       /       |
| 10 | 1024  |	0.161 |	0.001 |	 161  |	    1.63      |       1.00    |
| 11 | 2048  |	0.692 |	0.002 |	 346  |	    4.30      |       2.00    |
| 12 | 4096  |	2.130 |	0.003 |	 710  |	    3.08      |       1.50    |
| 13 | 8192  |	8.179 |	0.006 |	 1363 |	    3.84      |       2.00    |
| 14 | 16384 |	29.14 |	0.012 |	 2428 |	    3.56      |       2.00    |
| 15 | 32768 |	120.2 |	0.031 |	 3878 |	    4.13      |       2.58    |
