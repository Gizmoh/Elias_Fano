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
#define SAMPLEO 128
#define LARGO 1024
#define const 
void buscar (int busca, bit_vector H , bit_vector R){
    int aux = 0;
    int auxB = 0;
    rank_support_v<0> rankH_0(&H);
    bit_vector::select_1_type H_Sel1(&H);
    aux  = H_Sel1(busca);
    auxB = rankH_0(aux);
    
}

/*=====================================================================================*/

int convertBinaryToDecimal(long long n) //Esta funcion convierte de binario a numero descaradamente.
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

/*=====================================================================================*/

int main(int argc, char** argv){
    
    int* x; 
    int* Gc;
    int* S;
    x = new int [LARGO];
    Gc = new int [LARGO];
    S = new int [LARGO/SAMPLEO];
    int logN,logM,largoH,largoR = 0;
    int mayor = 0;
    string TlargoH, temp,tempA = "";

    
    //cout << "Iniciando Gaps" << endl;
    /*   GENERO ARREGLO NO DECRECIENTE  */
  
    //cout << "Arreglo "<<endl;
    //cout << x[0] ;
    for( int i = 1; i < LARGO; i++ ) {
        x[i] = x[i-1] + rand()%INCRE;
        //cout << " , " <<x[i];
    }
    //cout << endl;
  
    /*  GC  */
    Gc[0] = x[0];
    //cout << "Gaps"<< endl;
    //cout << Gc[0];
    for(int i = 1; i<LARGO; i++){
        Gc[i] = (x[i]-x[i-1]);
        //cout << " , "<< Gc[i] ;
        if (mayor < Gc[i])  mayor = Gc[i];
    }
    //cout << endl;
    
    /*  Calcular el sampling  */
    //cout << "Sampling"<< endl;
        S[0] = x[0];
        //cout << S[0];
    for(int i =SAMPLEO; i < LARGO; i = i + SAMPLEO ){
         S[i] = x[i];
         //cout << " , " <<S[i];
    }
    cout << endl;

    logN = (1 + int(log2(LARGO)));
    logM = (1 + int(log2(x[LARGO-1])));
    cout << "Bits significativos: " << logN << " Bits por celda: " << logM << endl;
    /*largoH = logN*LARGO; No puede calcularse a priori */
    largoR = (logM-logN)*LARGO;
    cout<< "Largo arreglos: " << largoH << " " << largoR << endl;



    /*testing BUSCAR LA POSICÓN EN LA CUAL ESTA EL ELEMENTO 
    int busqueda = x[rand()%LARGO];
    buscar(busqueda,Gc,S);*/

/*=====================================================================================*/
    //Genero los 2 bit vectors.
    string tempH[LARGO] = {""};
    int counter = 0;
    int X = 0;
    bit_vector R (largoR,0);

    for(int i=0; i < LARGO;i++){
        temp = bitset<13>(Gc[i]).to_string();
        tempA = temp.substr(0,11);
        tempH[i] = tempA;
        temp.erase(0,11);
        for(int t = 0; t<2; t++){
            if(temp[t]=='1') R[(2*i)+t] = 1;
        }
        X = convertBinaryToDecimal(stoll(tempA));
    }
    for (int i = 0; i < LARGO; i++){
        X = convertBinaryToDecimal(stoll(tempH[i]));
        counter += (X+1);
        //cout << tempH[i] << " " << X << " " << endl;
    }
    bit_vector H ((counter),0);
    counter = 0;
    H[0] = 1;
    for (int i = 1; i < LARGO; i++){
        X = convertBinaryToDecimal(stoll(tempH[i]));
        counter += (X+1);
        H[counter] = 1;
    }
    /*
    cout << endl;
    cout <<"Bits H "<< H << endl;
    cout <<"Bits R " << R <<  endl;*/

    if (Gc) delete [] Gc;
    if (x) delete [] x;
    if (S) delete [] S;

//Elias-Fano
}