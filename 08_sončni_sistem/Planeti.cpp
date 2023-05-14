#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <cstdlib> 
#include <iostream>
#include <vector>
using namespace std;

#pragma warning (disable:4996) //fopen

//Konstante sistema//
#define MSonce 2.0e30 //Sonce [kg]
#define Mz 5.94e24 //Zemlja [kg]
#define Ml 7.35e22 //Luna [kg]
#define AU 150e9 //Astronomska enota [m]

#define G 6.67e-11
#define PI 4*atan(1.0)

//Domaca naloga
#define M_Venera 3.86e24 //Venera [kg]
#define M_Merkur 3.301e23//Merkur[kg]
#define M_Mars 6.4171e23
#define M_Jupiter 1.8986e27
#define M_Io 8.93e22
#define M_Evropa 4.8e22
#define M_Ganimed 1.48e23
#define M_Kalisto 1.08e23



double dt = 10*60; //[s] Integracijski korak v enotah sekunda
double leto = 2.0; //čas trajanje simulacije v enotah let (na Zemlji);



double N = leto * 365 * 24 * 60 * 60 / dt; // čtevilo integracijskih korakov
int dn = int(N / 2000.0); // časovno okno za posodovitev izpisa na zaslon.

//Uvozimo knjičico s funkcijami.
#include "solar_system.h"

double x,y,vx,vy;

int main() {
    //Ustvariom telo Sonce in določimo vse atribute
    struct telo Sonce;
    nastavi_atribute(&Sonce, 0.0, 0.0, 0.0, 0.0, MSonce); //glej solar_system.h
    //print_info(&Sonce);
    struct telo Zemlja;
    x = 1.0 * AU;
    y = 0.0;

    vx = 0.0;
    vy = 0.0;
    nastavi_atribute(&Zemlja, x, y, vx, vy, Mz);//glej solar_system.h
    Zemlja.vy=v0(&Zemlja, &Sonce);
    Zemlja.v = abs_v(&Zemlja);

    //print_info(&Zemlja);

    struct telo Luna;
    x = 1.002567 * AU;
    y = 0.0;

    vx = 0.0;
    vy = v0(&Luna, &Zemlja) + Zemlja.vy; //Satelit od Zemlje dobi hitrost Zemlje
    nastavi_atribute(&Luna, x, y, vx, vy, Ml);//glej solar_system.h

    Luna.vy=v0(&Luna, &Zemlja) + Zemlja.vy;
    Luna.v = abs_v(&Luna);

    //print_info(&Luna);
    
    /*Ustvarim seznam objektov v simulaciji*/
    struct telo* list[] = { &Sonce, &Zemlja, &Luna };

    //Avtomatizirano prečtejem čtevilo elementov v seznamu.
    int st_teles = sizeof(list) / sizeof(list[0]);

    //Ustvarim datoteko za zapisovanje podatkov.
    FILE* data;
    data = fopen("rezultati.txt", "w+");
    fclose(data);

    //Nekaj spremenljivk za stetje
    int time = 0, i;
    double done;

    for (time = 0; time < N; time++){

        /*Določim deleč opravljene simulacije*/
        done = (double)(time+1) / (double)(N)*100.0;
        
        //Ustvarim, si 2D vektor, ki bo začasno hranil podatke o pospečkih teles.
        vector<vector<double>> temp_a;

        /*Grem skozi vse objete v simulacijai*/
        for (i = 0; i < st_teles; i++) {
            struct telo* Telo = list[i];
            double forceX, forceY;

            /*Za trenutno izbrano telo izračunam vsoto sil*/
            doloci_sile(Telo, list, st_teles, &forceX, &forceY);

            //Za telo si zapomnim, kolikčen je pospek v x in y smeri (a = F/m)
            temp_a.push_back({G*forceX, G*forceY});

        }


        /*Lokacije, hitrosti in pospečke posodobimo potem, ko določimo vse sile.*/
        for (i = 0; i < st_teles; i++) {
            list[i]->ax = temp_a[i][0];
            list[i]->ay = temp_a[i][1];
            if(dt < 30*60){
                posodobi_vektorje_Euler(list[i], dt);
            }else{
                posodobi_vektorje_Verlet(list[i], dt);
            }
        }

        //Posodobim zapis na zaslonu in zapičem rezultate v datoteko.
        if ((time % dn)==0){
            printf("St. integracijskih korakov je %d | dt = %.1f s | delez = %3.2f %%\n", (int)(N),dt,(double)(done));
            data = fopen("rezultati.txt", "a");
            fprintf(data, "%f\t", time * dt / 3600.0);

            for (i = 0; i < st_teles; i++) {
                printf("i = %d | x = %lf | y = %lf\n", i, list[i]->x/ AU, list[i]->y/AU);
                fprintf(data, "%f\t%f", (list[i]->x) / AU, (list[i]->y) / AU);
                if(i!=st_teles-1){
                    fprintf(data, "\t");
                }else{
                    fprintf(data, "\n");
                }
            }
            fclose(data);
            system("cls");//Zbriči terminar
        }
    }
    return 0;
}