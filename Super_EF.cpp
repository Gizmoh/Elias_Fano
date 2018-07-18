#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <math.h> 
#include <iostream>
#include <bitset>
#include "include/BasicCDS.h"

using namespace std;
using namespace sdsl;
using namespace cds;



#define INCRE 16
#define SAMPLEO 32
#define LARGO 64
#define const 
void buscar (int busca, int gc[] , int s[]){}

/*=====================================================================================*/

int main(int argc, char** argv){
    
    int x[LARGO] = {0}; 
    int Gc[LARGO] = {0};
    int S[LARGO/SAMPLEO] = {0};
    int logN,logM,largoH,largoR = 0;
    int mayor = 0;

    
    cout << "Iniciando Gaps" << endl;
    /*   GENERO ARREGLO NO DECRECIENTE  */
  
    cout << "Arreglo "<<endl;
    cout << x[0] ;
    for( int i = 1; i < LARGO; i++ ) {
        x[i] = x[i-1] + rand()%INCRE;
        cout << " , " <<x[i];
    }
    cout << endl;
  
    /*  GC  */
    Gc[0] = x[0];
    cout << "Gaps"<< endl;
    cout << Gc[0];
    for(int i = 1; i<LARGO; i++){
        Gc[i] = x[i]-x[i-1];
        cout << " , "<< Gc[i] ;
        if (mayor < Gc[i])  mayor = Gc[i];
    }
    cout << endl;
    
    /*  Calcular el sampling  */
    cout << "Sampling"<< endl;
        S[0] = x[0];
        cout << S[0];
    for(int i =SAMPLEO; i < LARGO; i = i + SAMPLEO ){
         S[i] = x[i];
         cout << " , " <<S[i];
    }
    cout << endl;

    logN = (1 + int(log2(LARGO)));
    logM = (1 + int(log2(x[LARGO-1])));
    cout << "Bits significativos: " << logN << " Bits por celda: " << logM << endl;
    largoH = logN*LARGO;
    largoR = (logM-logN)*LARGO;
    cout "Largo arreglos: " << largoH << " " << largoR << endl;



    /*testing BUSCAR LA POSICÓN EN LA CUAL ESTA EL ELEMENTO 
    int busqueda = x[rand()%LARGO];
    buscar(busqueda,Gc,S);*/

/*=====================================================================================*/
//Elias-Fano
}