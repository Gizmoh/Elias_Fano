
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>

using namespace std;
using namespace sdsl;

#define INC 128
#define sampling 64

void testing(int X[], int Y[], int Z[], int largo){ //X arreglo de gaps; Y sampling, Z arreglo original
    int testing  = 0;
    int resultado = 0;
    int sampled = 0;
    testing  = rand()%largo;
    sampled = trunc(testing/sampling);
    resultado = Y[sampled];
    //cout << Y[sampled] << " "<< testing << " " << sampled << " " << sampled*sampling << endl;
    for(int i = sampled*sampling;i < testing; i++){
        resultado = resultado + X[i+1];
    }
    cout << testing << " " << resultado << " " << Z[testing] << endl;
}


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

    for(int i=0; i < 25; i++){
        testing (Y,S,X, largo);
    }

}