
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>
#include "include/BasicCDS.h"

using namespace std;
using namespace sdsl;
using namespace cds;

#define INC 32
#define sampling 256
#define LARGO 65536
#define TESTING 1000

void testing(ulong *Gaps, int Y[], int Z[],int Bits){ //X arreglo de gaps; Gaps sampling, Z arreglo original
    int testing  = 0;
    int resultado = 0;
    int sampled = 0;
    testing  = rand()%LARGO;
    sampled = trunc(testing/sampling);
    resultado = Y[sampled];
    //cout << Y[sampled] << " "<< testing << " " << sampled << " " << sampled*sampling << endl;
    for(int i = sampled*sampling;i < testing; i++){
        resultado = resultado + getNum64(Gaps,(Bits*i),Bits);
    }
    //cout<< "Posicion buscada: " << testing << " Numero obtenido " << resultado << " Numero esperado " << Z[testing] << endl;
}


int main (int argc, char** argv){// Recibe como argumento el LARGO del arreglo
    int X [LARGO]={};
    int Y [LARGO] = {};
    int S [LARGO/sampling] = {};
    int max = 0;
    int r = 0;
    ulong *Gap;
    int nCellsS = LARGO / sampling;
    if (LARGO % sampling)
        nCellsS++;
    int nCellG = LARGO - nCellsS;
    for(int i = 1; i< LARGO; i++){//Genero el arreglo no decreciente
        r = rand()%INC;
        X[i]=X[i-1]+r;
    }

    Y[0] = X[0];
    for(int i=1;i<LARGO;i++){//Calculo el Gap maximo.
        Y[i]=X[i]-X[i-1];
        if(max < Y[i]) max = Y[i];
    }
    S[1]=Y[1];
    for(int i = 1; i < int(LARGO/sampling);i++){//Genero el sampling
        S[i] = X[i*sampling];
    }
    uint BitMayor = (1 + int(log2(max)));
    int aux = (nCellG*BitMayor)/W64;
    if((nCellG*BitMayor)%W64)
        aux++;
    Gap = new ulong[aux];
    int i = 0;
    int j = 0;
    for(i,j = 0; i < nCellG; i++,j+=BitMayor){//Guardo los gaps codificados en un ulong
        setNum64(Gap,j,BitMayor,Y[i]);
    }
    double t = 0.0;
    for(int i=0; i < TESTING; i++){
        t = getTime_ms();
        testing (Gap,S,X, BitMayor);
        t += getTime_ms()-t;
    }
    t = (t / TESTING);


    cout << "Tamaño arreglo inicial: " << LARGO*sizeof(int)<<endl;
    cout << "Tamaño arreglo Gaps: " << aux*sizeof(ulong) <<" Tamaño arreglo Samples: " << (LARGO/sampling)*sizeof(int) <<endl;
    cout << "Ratio de compresion: " << double(double(LARGO*sizeof(int))/double(aux*sizeof(ulong)+(LARGO/sampling)*sizeof(int)))<< endl;
    cout << "Tiempo promedio de calculo: " << t << endl;
}