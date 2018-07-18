#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <math.h> 
#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include <bitset>
using namespace std;
using namespace sdsl;


#define INCRE 64
#define SAMPLEO 3
#define LARGO 10
#define const 
void buscar (int busca, int gc[] , int s[]){}

/*=====================================================================================*/

int main(int argc, char** argv){
    
    int x[LARGO] = {0}; 
    int Gc[LARGO] = {0};
    int S[LARGO/SAMPLEO] = {0};
    
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

    /*testing BUSCAR LA POSICÓN EN LA CUAL ESTA EL ELEMENTO 
    int busqueda = x[rand()%LARGO];
    buscar(busqueda,Gc,S);*/

/*=====================================================================================*/
//Elias-Fano
    
    cout << endl;
    int cantBits = log2(x[LARGO-1]) + 1; //bits a ocupar

    cout << "Cantidad de bits para arreglo 'x' en binario "<< cantBits << endl;
    cout << endl;

    string *xbinary = NULL;
    xbinary = new string[LARGO];
    xbinary[0] = bitset<8>(x[0]).to_string();
     cout << xbinary[0];
    for(int i = 1; i<LARGO; i++){
        xbinary[i] = bitset<8>(x[i]).to_string(); //Paso la cadena de bits a una cadena
        cout << " , " <<xbinary[i];
    }
    cout << endl; cout << endl;

    int hBit = log2(LARGO-1) + 1;

    cout << "Cantidad de bits para arreglo H : "<< hBit <<" Cantidad de bits para arreglo R: "<< cantBits-hBit << endl;
    cout << endl;
    
    string *H = NULL;
    H = new string[LARGO];
    string *R = NULL;
    R = new string[LARGO];

    for(int i = 0; i<LARGO; i++){  //separo el arreglo en binario en otros 2 arreglos
        string aux = xbinary[i];
        H[i] = aux.substr(0,hBit);
        cout << " { "<< "H" <<"["<<i<<"]: " <<H[i]<< " ";
        R[i] = aux.substr(hBit,aux.length());
        cout <<" R"<<"["<<i<<"]: " <<R[i] << " } ";
    }
    cout << endl; cout << endl; 

    
    /* Transformo el arreglo h en binario a uno h en enteros  */
    cout << "Arreglo H en Entero"<<endl;
    int *hEntero = NULL;
    hEntero = new int[LARGO];
    for(int i = 0; i<LARGO; i++){//Traspaso los bits de H a enteros

       int entero = std::stoi(H[i], nullptr, 2);
       hEntero[i] = entero;
       cout << hEntero[i] << " " ;
    }
    cout <<endl; cout << endl;

    /*  Gaps de H en enteros */
    int *Gch = NULL;
    Gch = new int[LARGO];

    Gch[0] = hEntero[0];
    cout << "Gaps de H en entero : "<< endl;
    cout << Gch[0];
    for(int i = 1; i<LARGO; i++){
        Gch[i] =hEntero[i]-hEntero[i-1];
        cout << " , "<< Gch[i] ;
    }
    cout << endl; cout << endl;
    
    /* Convierto el gaps del Hentero en Unario (Gch) */
    cout << "Gaps de H entero en Unario"<<endl;
   
    int largoUnario = 0;  //calcular largo del unario (cada elemento)
    for(int i = 0; i<LARGO; i++){ 
        if(Gch[i] == 0){ 
            largoUnario++;
        }else{
            largoUnario = largoUnario + Gch[i] + 1;
        }
    }

    bit_vector unario(largoUnario,0); //bit vector para almacenar el unario de cada elemento del gaps de H entero
    int count = 0;

    for(int i = 0; i<LARGO; i++){
        string aux = "";
        int ind = Gch[i];
        
        for(int j = 0; j< ind; j++){
            aux = aux + "0";
            unario[count] = 0;
            count++;
        }
        int aux2 = Gch[i];
        aux = aux + "1";
        cout << aux << " ";
        unario[count] = 1;
        count++;
    }
    cout << endl;
    cout << unario <<endl;
    cout << endl;

    
    int find = 8 ;
    bit_vector::select_1_type unario_sel(&unario); 

    cout <<"Select_support1(Arreglo unario, posición buscada en el arreglo original): " << unario_sel(find) + 1 << endl;  //entrega posición donde esta el quinto 1
    int selectResult = unario_sel(find) + 1;
    cout << endl;

    rank_support_v<0> unario_r1(&unario); 

    cout << "Rank_support0(unario, posición obtenida del select anterior): " << unario_r1(selectResult) <<endl; //hasta la posicón obtenida del select, entrega cuantos 0 encuentra
    int rankResult = unario_r1(selectResult);
    //cout <<"R5: "<< R[find-1] <<endl;
    //cout << "test binary : " << bitset<3>(rankResult)<<endl;

    string result = bitset<4>(rankResult).to_string() +  R[find-1];
    int posResult = 0;
    for(int i = 0; i<LARGO; i++){
        if (result == xbinary[i]){
            posResult = i;
        }
    }
    
    cout << endl;
    cout << "En la posición  8 buscada esta el elemento: "<< x[posResult] << endl;
    cout << endl;
 


/*=====================================================================================
    //rank_support_v<>rankeando(&unario);
    
    bit_vector b = {0,1,0,1,1,1,0,0,0,1,1};
    size_t zeros = rank_support_v<0>(&b)(b.size());
    bit_vector::select_0_type b_sel(&b);

    for (size_t i=1; i <= zeros; ++i) {
        cout << b_sel(i) << " - "; //Imprime en cada posición donde hay un 0
    }
    cout << endl;

    rank_support_v<1> b_r1(&b); 

    for (size_t i=0; i<=b.size(); ++i){
     cout << i << ": "<< b_r1(i)<< endl; //imprimo la cantidad de 1 antes de la posción i
    }
    cout << endl;
*/


/*
GAPS 20 5 7 11 4 55 3 16 8
Bitmax para marcar expcepciones
B = 1 0 0 0 0 1 0 1 0
[a,b] = [0,1.15]
EXep = 20 55 16
Pa = 5 7 11 4 3 8 
ALmacenar los dos en arreglos de bits
setnum64(arreglo,posicion inicial, cant de bits , valor a setear) //Setea en un arreglo, no retorna //la psoci inicial va aumentando segun bits necesarios

m = 1 +log2(max)
k = 3*m(cant de bit para el mas grande) /w64 = 0

if((3*6)%w64){k++}
Ex = new ulong[k] // con este ex llamo a setnum64

para leer getnum64(arrreglo, valor inicial, canti de bits) //devuelve el entero

El algoritmo si quiero extraaer posicion 'i'
busco en excepcion
Bex[i] es 0 o 1
if(ßexi[i]){
    i' = rank1(Bex, i)
    ini = i`*m //necesario para el getnum64
}


A = 3 5 9 16 16 20 25 27 29 30
gaps = 3 2 4 7 0 4 5 2 2 1
Bex = 0 0 0 1 0 0 1 0 0 0
S = 3 16 29
gaps = retriveGAP(i)
A[i] = ?
pos = i/s 
som = s[pos]
*/
/*=====================================================================================*/

/*  PiforDelta */

 


/*=====================================================================================*/
    return 0;
}
