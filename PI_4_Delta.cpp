#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>
#include <time.h>
#include "include/BasicCDS.h"

using namespace std;
using namespace sdsl;
using namespace cds;

#define INC 128
#define sampling 64
#define largoARR 256
typedef bit_vector::size_type size_type;

void storeSamples(ulong *Samples, int X[],uint BitA){
    int i,j;
    for (i=j=0; i< round(largoARR/sampling); i++,j+=BitA){
        setNum64(Samples,j,BitA,X[i]);
    }
}
void Storage(int X[],ulong *Gaps, ulong *Excep, bit_vector Ex,int Mayor,int A,int contadorI, int contadorO){
    int i,j,k,BitsA,BitsB;
    BitsA = (1+int(log2(Mayor))); //bitsA almacena los bits necesarios para almacenar el Gap mas grande.
    BitsB = (1+int(log2(Mayor-A))); //bitsB almacena los bits necesarios para almacenar el Gap mas grande, menos A.

    Gaps = new ulong[contadorI*BitsA];//mmmmmmmmmmmm
    Excep = new ulong[contadorO*BitsB];//needs more cowbell

    for (i = 1,j,k=0;i < largoARR; i++){
        if(i%(sampling+1)==0){
            continue;
        }
        if(Ex[i]==0){
            setNum64(Gaps,j,BitsA,(X[i]-A));
            j+=BitsA;
            cout << "Numero real: " << X[i] << endl;
            cout <<"Numero almacenado: " << getNum64(Gaps,j, BitsA) << endl;
        }
        if(Ex[i]==1){
            setNum64(Excep,k,BitsB,(X[i]));
            k+=BitsB;
            cout << "Numero real: " << X[i] << endl;
            cout <<"Numero almacenado: " << getNum64(Excep,j, BitsB) << endl;
        }
    }
}

void testing(int X[], int Y[], int Z[]){ //X arreglo de gaps; Y sampling, Z arreglo original
    int testing  = 0;
    int resultado = 0;
    int sampled = 0;
    int size = 0;
    srand (time(NULL));
    testing  = rand()%largoARR;
    sampled = trunc(testing/sampling);
    resultado = Y[sampled];
    //cout << Y[sampled] << " "<< testing << " " << sampled << " " << sampled*sampling << endl;
    for(int i = sampled*sampling;i < testing; i++){
        resultado = resultado + X[i+1];
    }
    cout << testing << " " << resultado << " " << Z[testing] << endl;
}


int main (int argc, char** argv){// Recibe como argumento el largoARR del arreglo
    double r = 0;
    int mayor = 0;
    int X [largoARR]={};
    int Y [largoARR]={};
    int S [largoARR/sampling] = {};
    ulong *Gaps, *Excep, *Samples;
    bit_vector Ex (largoARR,0);
    rank_support_v<> rankEx (&Ex);
    bit_vector::select_1_type Ex_Sel1(&Ex);
    bit_vector::select_0_type Ex_Sel0(&Ex);

    for(int i = 0; i< largoARR; i++){//Genero el arreglo no decreciente
        r = rand()%INC;
        X[i]=X[i-1]+r;
    }
    Y[0] = X[0];
    mayor = X[0];
    for(int i=1;i<largoARR;i++){//Genero el arreglo de gaps
        Y[i]=X[i]-X[i-1];
        if (mayor  < Y[i]){
            mayor = Y[i];
        }
    }
    S[1]=Y[1];
    for(int i = 1; i < round(largoARR/sampling);i++){//Genero el sampling
        S[i] = X[i*sampling];
    }

    /*for(int i=0; i < 25; i++){
        testing (Y,S,X);
    }*/

    uint BitMayor = (1+int(log2(mayor)));
    uint BitSamplingMayor = (1+int(log2(S[largoARR/sampling-1])));

    cout << "Gap mayor: " << mayor << endl;
    cout << "Bits necesarios: " << BitMayor << endl;
    cout << "Sampling mayor: " << (S[largoARR/sampling-1]) << endl;
    cout << "Bits necesarios: " << BitSamplingMayor << endl;
    cout << "Calculo de distribuciones:" << endl;
    cout << "Se necesitan " << (BitSamplingMayor * round(largoARR/sampling)) << " bits para almacenar el sampling" << endl;


    int top = trunc(mayor *0.95);
    int bot = trunc(mayor *0.05);
    int contadorI = 0; 
    int contadorO = 0;
    double porcentajeI = 0;
    double porcentajeO = 0;
    double q;
    cout << bot << " " << top << endl;
    for(int i = 0; i < largoARR; i++){
        if(bot < Y[i] and top > Y[i]){
            contadorI++;
        }
        else{
            contadorO++;
            Ex[i] = 1;
        }
    }
    Samples = new ulong[S[largoARR/sampling-1]];
    storeSamples(Samples,X, BitSamplingMayor);
    Storage(X,Gaps,Excep,Ex,mayor,top,contadorI,contadorO);


    porcentajeI = double(contadorI)/double(largoARR);
    porcentajeO = double(contadorO)/double(largoARR);
    cout << "Porcentaje en el intervalo: " << porcentajeI << " " << contadorI;
    cout << ", porcentaje fuera del intervalo: " << porcentajeO<< " " << contadorO << endl;

    cout << Ex << endl;


}