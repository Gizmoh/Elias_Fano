#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>
#include <time.h>

using namespace std;
using namespace sdsl;

#define INC 128
#define sampling 64
#define largoARR 256
typedef bit_vector::size_type size_type;

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
    bit_vector Ex (largoARR,0);
    rank_support_v<> rankEx (&Ex);

    for(int i = 1; i< largoARR; i++){//Genero el arreglo no decreciente
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

    cout << "gap mayor: " << mayor << endl;
    cout << "calculo de distribuciones:" << endl;

    int top = trunc(mayor *0.875);
    int bot = trunc(mayor *0.125);
    int contadorI = 0; 
    int contadorO = 0;
    double porcentajeI = 0;
    double porcentajeO = 0;
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
    porcentajeI = double(contadorI)/double(largoARR);
    porcentajeO = double(contadorO)/double(largoARR);
    cout << "porcentaje en el intervalo: " << porcentajeI << " " << contadorI;
    cout << " porcentaje fuera del intervalo: " << porcentajeO<< " " << contadorO << endl;

    cout << Ex << endl;

}