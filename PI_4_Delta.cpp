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
#define TESTING 1000

int genInterval(int X[], int mayor){ //Calculo el techo del intervalo de forma que 10% quede adentro
    int contadorI;
    int y = mayor;
    double percent = 1;
    while (percent >0.9){
        y--;
        contadorI = 0;
        percent = 1;
        for(int i = 0;i < largoARR; i++){
            if (X[i]<y){
                contadorI++;
            }
        }
        percent = double(contadorI)/double(largoARR);
    }
    return y;
}

void leerElemento(int pos, bit_vector Ex,ulong *Samples, ulong *Gaps, ulong *Excep,int  mayor,int  Smayor){
    int SamplePos = pos/sampling;
    int Start = (SamplePos*sampling);
    int solucion = getNum64(Samples,(Smayor*(SamplePos)),Smayor);
    rank_support_v<1> rankEx_1 (&Ex);
    rank_support_v<0> rankEx_0 (&Ex);
    bit_vector::select_1_type Ex_Sel1(&Ex);
    bit_vector::select_0_type Ex_Sel0(&Ex);
    for (int i = Start; i < pos; i++){
        cout << i << " ";
        if(Ex[i]==0){
            cout <<" Select: "<<Ex_Sel0(i) << " Rank: ";
            cout << rankEx_0(i) << " (0) corresponde a " << getNum64(Gaps,(mayor*(rankEx_0(i))),mayor) << endl;
            cout << "Siguiente: " << getNum64(Gaps,(mayor*(rankEx_0(i+1))),mayor) << endl;
            cout << "Anterior: " << getNum64(Gaps,(mayor*(rankEx_0(i-1))),mayor) << endl;
            solucion += getNum64(Gaps,(mayor*(rankEx_0(i))),mayor);
        }
        else{
            cout << rankEx_1(i)<< " (1) corresponde a " << getNum64(Excep,(mayor*(rankEx_1(i))),mayor) << endl;
            solucion += getNum64(Excep,(mayor*(rankEx_1(i))),mayor);
        }
    }
    cout << "Numero calculado: " << solucion;
}

void storeSamples(ulong *Samples, int X[],uint BitA){
    int i,j;
    for (i=j=0; i< round(largoARR/sampling); i++,j+=BitA){
        setNum64(Samples,j,BitA,X[i*sampling]);/*
        cout <<"Posicion: " << i*sampling << endl;
        cout << "Numero real: " << X[i*sampling] << endl;
        cout <<"Numero almacenado: " << getNum64(Samples,j, BitA) << endl;*/
    }
}
void Storage(int Y[],ulong *Gaps, ulong *Excep, bit_vector Ex,int Bits){
    int counterA = 0;
    int counterB = 0;
    double t = 0.0;

    t = getTime_ms();
    for (int i = 1;i < largoARR; i++){//Si el elemento es uno de los que se encuentran en el sampling, se cambia el numero en el bit_vector a 0 y se salta.
        if(i%(sampling+1)==0){
            Ex[i] = 0;
            continue;
        }
        if(Ex[i]==0){//Si en el bit_vector el valor es 0, guarda el elemento correspondiente del arreglo X en el ulong de gaps.
            setNum64(Gaps,counterA,Bits,Y[i]);/*
            cout << "Dentro intervalo, Numero real: " << Y[i] << endl;
            cout <<"Numero almacenado: " << getNum64(Gaps,counterA, Bits) << endl;*/
            counterA +=Bits;
        }
        else{//Si en el bit_vector el valor es 1, guarda el elemento correspondiente del arreglo X en el ulong de excepciones.
            setNum64(Excep,counterB,Bits,Y[i]);/*
            cout << "Fuera intervalo, Numero real: " << Y[i] << endl;
            cout <<"Numero almacenado: " << getNum64(Excep,counterB, Bits) << endl;*/
            counterB +=Bits;
        }
    }
    t = getTime_ms() - t;
    cout << "Tiempo de compresion: " << t << endl;
}

void testing(int X[], int Y[], int Z[]){ //X arreglo de gaps; Y sampling, Z arreglo original
    int testing  = 0;
    int resultado = 0;
    int sampled = 0;
    int size = 0;
    srand (time(NULL));
    testing  = rand()%largoARR;
    sampled = trunc(testing/sampling);
    cout << sampled << endl;
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
    double test = double(largoARR/sampling);
    int nCellsS = largoARR/sampling;
    int nCellG = largoARR - nCellsS;
    int S [nCellsS] = {};
    ulong *Gaps, *Excep, *Samples;
    bit_vector Ex (largoARR,0);

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

    uint BitMayor = (1+int(log2(mayor)));
    uint BitSamplingMayor = (1+int(log2(S[largoARR/sampling-1])));

    cout << "Gap mayor: " << mayor << endl;
    cout << "Bits necesarios: " << BitMayor << endl;
    cout << "Sampling mayor: " << (S[largoARR/sampling-1]) << endl;
    cout << "Bits necesarios: " << BitSamplingMayor << endl;
    cout << "Calculo de distribuciones:" << endl;
    cout << "Se necesitan " << (BitSamplingMayor * round(largoARR/sampling)) << " bits para almacenar el sampling" << endl;


    int top = 0;
    int contadorI = 0; 
    int contadorO = 0;
    double porcentajeI = 0;
    double porcentajeO = 0;
    top = genInterval(Y,mayor);
    for(int i = 0; i < largoARR; i++){
        if(top > Y[i]){
            contadorI++;
        }
        else{
            contadorO++;
            Ex[i] = 1;
        }
    }

    int aux = nCellsS*BitSamplingMayor/W64;
    if (nCellsS*BitSamplingMayor % W64)
        aux++;
    int aux2 = nCellG*BitMayor/W64;
    if (nCellG*BitMayor % W64)
        aux2++;
    int aux3 = contadorO*BitMayor/W64;
    if(contadorO*BitMayor % W64){
        aux3++;
    }
    cout << "sizeof Samples = " << aux*sizeof(ulong) << " bits" << endl;
    cout << "sizeof Gaps = " << aux2*sizeof(ulong) << " bits" << endl;
    Samples = new ulong[aux];
    Gaps = new ulong[aux2];
    Excep = new ulong[aux3];

    storeSamples(Samples,X, BitSamplingMayor);
    Storage(Y,Gaps,Excep,Ex,BitMayor);

    porcentajeI = double(contadorI)/double(largoARR);
    porcentajeO = double(contadorO)/double(largoARR);
    cout << "Porcentaje en el intervalo: " << porcentajeI;
    cout << ", porcentaje fuera del intervalo: " << porcentajeO<< endl;
    leerElemento(129,Ex,Samples,Gaps,Excep,BitMayor,BitSamplingMayor);
    cout << " Numero pedido: " << X[129] << endl;
}