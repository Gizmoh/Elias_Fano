
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>
#include <iostream>
#include <fstream>
#include "include/BasicCDS.h"

using namespace std;
using namespace sdsl;
using namespace cds;

#define INC 16
#define sampling 8192
#define largoARR 131072
#define TESTING 1000

void testing(ulong *Gaps, int Y[], int Z[],int Bits){ //X arreglo de gaps; Gaps sampling, Z arreglo original
    int testing  = 0;
    int resultado = 0;
    int sampled = 0;
    testing  = rand()%largoARR;
    sampled = trunc(testing/sampling);
    resultado = Y[sampled];
    //cout << Y[sampled] << " "<< testing << " " << sampled << " " << sampled*sampling << endl;
    for(int i = sampled*sampling;i < testing; i++){
        resultado = resultado + getNum64(Gaps,(Bits*i),Bits);
    }
    //cout<< "Posicion buscada: " << testing << " Numero obtenido " << resultado << " Numero esperado " << Z[testing] << endl;
}


int main (int argc, char** argv){// Recibe como argumento el largoARR del arreglo
    int X [largoARR]={};
    int Y [largoARR] = {};
    int S [largoARR/sampling] = {};
    int max = 0;
    int r = 0;
    ulong *Gap;
    int nCellsS = largoARR / sampling;
    if (largoARR % sampling)
        nCellsS++;
    int nCellG = largoARR - nCellsS;
    for(int i = 1; i< largoARR; i++){//Genero el arreglo no decreciente
        r = rand()%INC;
        X[i]=X[i-1]+r;
    }

    Y[0] = X[0];
    for(int i=1;i<largoARR;i++){//Calculo el Gap maximo.
        Y[i]=X[i]-X[i-1];
        if(max < Y[i]) max = Y[i];
    }
    S[1]=Y[1];
    for(int i = 1; i < int(largoARR/sampling);i++){//Genero el sampling
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
    double Ttotal = 0.0;
    for(int i=0; i < TESTING; i++){
        t = getTime_ms();
        testing (Gap,S,X, BitMayor);
        Ttotal += (getTime_ms()-t);
    }
    Ttotal = (Ttotal / TESTING);
    double SArregloX = largoARR*sizeof(int);
    double SArregloG = aux*sizeof(ulong);
    double SArregloS = (largoARR/sampling)*sizeof(int);
    double Ratio = (SArregloG+SArregloS)/SArregloX;
    cout << "Gap Coding" << endl;
    cout << "Parametros: " << endl;
    cout << "Largo Arreglo: " << largoARR << " Incremento: " << INC << " Muestreo: " << sampling << endl;
    cout << "Tamaño arreglo original: " << SArregloX <<endl;
    cout << "Tamaño arreglo Gaps: " << SArregloG <<" Tamaño arreglo Samples: " << SArregloS <<endl;
    cout << "Ratio de compresion: " << Ratio << endl;
    cout << "Tiempo promedio de consulta: " << Ttotal<<"ms" << endl;
    ofstream Resultados;
    Resultados.open ("Resultados.csv");
    Resultados << "Esquema,Tiempo promedio (ms),Ratio de compresion, Bits arreglo original, Bits comprimido\n";
    Resultados <<"Gap Coding,"<< Ttotal<<","<<Ratio<<","<<SArregloX<<","<<(SArregloG+SArregloS)<<"\n";

}