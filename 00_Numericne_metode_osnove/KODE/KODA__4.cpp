#include <iostream>
#include <fstream>


int main(int argc, char * argv[]){
    std::fstream myfile("D:\\data.txt", std::ios_base::in);
    float a, b, c, d, e, f;
    myfile >> a >> b >> c >> d >> e >> f;
    printf("%f\t%f\t%f\t%f\t%f\t%f\n", a, b, c, d, e, f);
    getchar();
    return 0;
}