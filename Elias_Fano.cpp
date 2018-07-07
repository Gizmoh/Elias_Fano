
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>

using namespace std;
using namespace sdsl;

#define INC 128
#define sampling 64


int main (int argc, char** argv){// Recibe como argumento el largo del arreglo
    int largo = 0;
    double r = 0;
    largo = atoi(argv[1]);
    int X [largo]={};
    int Y [largo]={};
    int S [largo/sampling] = {};

    for(int i = 1; i< largo; i++){//Genero el arreglo no decreciente
        r = rand()%INC;
        X[i]=X[i-1]+r;
    }
    Y[0] = X[0];
    for(int i=1;i<largo;i++){//Genero el arreglo de gaps
        Y[i]=X[i]-X[i-1];
    }
    S[1]=Y[1];
    for(int i = 1; i < round(largo/sampling);i++){//Genero el sampling
        S[i] = X[i*sampling];
    }

    
}