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



#define INCRE 64
#define LARGO 131072

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

void buscar (int busca, bit_vector H , bit_vector R){
    int aux = 0;
    int auxB = 0;
    string tempX = "";
    rank_support_v<0> rankH_0(&H);
    bit_vector::select_1_type H_Sel1(&H);
    aux  = H_Sel1(busca+1);
    cout << "Select 1: " << aux << endl;
    auxB = rankH_0(aux);
    cout << "Rank 0: " << auxB << endl;
    tempX = bitset<14>(auxB).to_string();
    aux = R[(2*busca)];
    auxB= R[(2*busca)+1];
    tempX += to_string(aux);
    tempX += to_string(auxB);
    cout << "Resultado: " << convertBinaryToDecimal(stoll(tempX))<< " " << tempX << endl;
}

/*=====================================================================================*/

int main(int argc, char** argv){
    
    int* x; 
    int* xPrima;
    int* Gc;
    x = new int [LARGO];
    xPrima = new int [LARGO];
    Gc = new int [LARGO];
    int logN,logM,largoH,largoR = 0;
    int mayor = 0;
    string TlargoH, temp,tempA = "";

    
    cout << "Iniciando Gaps" << endl;
       //GENERO ARREGLO NO DECRECIENTE
  
    cout << "Arreglo "<<endl;
    cout << x[0] ;
    for( int i = 1; i < LARGO; i++ ) {
        x[i] = x[i-1] + rand()%INCRE;
        cout << " , " <<x[i];
    }
    cout << endl;

    logN = (1 + int(log2(LARGO)));
    logM = (1 + int(log2(x[LARGO-1])));
    /*largoH = logN*LARGO; No puede calcularse a priori */
    largoR = (logM-logN)*LARGO;
    cout << "diff de bits: " << (logM-logN) << endl;



    /*testing BUSCAR LA POSICÓN EN LA CUAL ESTA EL ELEMENTO 
    int busqueda = x[rand()%LARGO];
    buscar(busqueda,Gc,S);*/

/*=====================================================================================*/
    //Genero los 2 bit vectors.
    string tempH[LARGO] = {""};
    int counter = 0;
    int X = 0;
    int aux = 0;
    bit_vector R (largoR,0);

    for(int i=0; i < LARGO;i++){
        temp = bitset<18>(x[i]).to_string();
        tempA = temp.substr(0,14);
        tempH[i] = tempA;
        temp.erase(0,14);
        for(int t = 0; t<4; t++){
            if(temp[t]=='1') R[(2*i)+t] = 1;
        }
        X = convertBinaryToDecimal(stoll(tempA));
        xPrima[i] = X;
    }
    Gc[0] = xPrima[0];
    for(int i = 1;i < LARGO; i++){
        Gc[i]= (xPrima[i]-xPrima[i-1]);
    }

    for (int i = 0; i < LARGO; i++){//Calculo cuantos bits necesito para los unarios.
        counter += (Gc[i]+1);
        //cout << tempH[i] << " " << X << " " << endl;
    }

    cout<< "Bits arreglos: " << counter << " " << largoR << endl;
    cout << "Bits vector original: " << sizeof(int)*LARGO;
    bit_vector H (counter,0);
    counter = 0;
    H[0] = 1;
    for (int i = 1; i < LARGO; i++){
        counter += (Gc[i]+1);
        H[counter] = 1;
    }
    
    cout << endl;
    //cout <<"Bits H "<< H << endl;
    //cout <<"Bits R " << R <<  endl;
    buscar(3,H,R);

    if (Gc) delete [] Gc;
    if (x) delete [] x;

//Elias-Fano
}