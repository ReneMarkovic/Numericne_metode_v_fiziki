// Random_generator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <cmath>
#include<time.h>

using namespace std;


#define sample_size 10000
#define PI 4*atan(1)
#include "random_nenerator.h"



int main(){
    srand(time(NULL));
    printf("Izvajam vajo 1\n");
    vaje_01();
    printf("\n");

    printf("Izvajam vajo 2\n");
    vaje_02();
    printf("\n");

    printf("Izvajam vajo 3\n");
    igra_loto();
    printf("\n");
    printf("Program je uspešno zaključil");
    return 0;
}
