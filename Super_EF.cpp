#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <bitset>
#include "include/BasicCDS.h"

using namespace std;
using namespace sdsl;
using namespace cds;

#define INC 16
#define sampling 8192
#define largoARR 131072
#define TESTING 1000

/*=====================================================================================*/

int convertBinaryToDecimal(long long n) //Esta funcion convierte de binario a numero descaradamente.
{
    int decimalNumber = 0, i = 0, remainder;
    while (n != 0)
    {
        remainder = n % 10;
        n /= 10;
        decimalNumber += remainder * pow(2, i);
        ++i;
    }
    return decimalNumber;
}

/*=====================================================================================*/

void buscar(int busca, bit_vector H, bit_vector R)
{
    int aux = 0;
    int auxB = 0;
    string tempX = "";
    rank_support_v<0> rankH_0(&H);
    bit_vector::select_1_type H_Sel1(&H);
    aux = H_Sel1(busca + 1);
    //cout << "Select 1: " << aux << endl;
    auxB = rankH_0(aux);
    //cout << "Rank 0: " << auxB << endl;
    tempX = bitset<30>(auxB).to_string();
    aux = R[(2 * busca)];
    auxB = R[(2 * busca) + 1];
    tempX += to_string(aux);
    tempX += to_string(auxB);
    //cout << "Resultado: " << convertBinaryToDecimal(stoll(tempX))<< " " << tempX << endl;
}

/*=====================================================================================*/

int main(int argc, char **argv)
{

    int *x;
    int *xPrima;
    int *Gc;
    int diff = 0;
    x = new int[largoARR];
    xPrima = new int[largoARR];
    Gc = new int[largoARR];
    int logN, logM, largoH, largoR = 0;
    int mayor = 0;
    string TlargoH, temp, tempA = "";

    for (int i = 1; i < largoARR; i++)
    {
        x[i] = x[i - 1] + rand() % INC;
        //cout << " , " <<x[i];
    }
    //cout << endl;

    logN = (1 + int(log2(largoARR)));
    logM = (1 + int(log2(x[largoARR - 1])));
    /*largoH = logN*largoARR; No puede calcularse a priori */
    largoR = (logM - logN) * largoARR;
    diff = logM - logN;

    /*testing BUSCAR LA POSICÓN EN LA CUAL ESTA EL ELEMENTO 
    int busqueda = x[rand()%largoARR];
    buscar(busqueda,Gc,S);*/

    /*=====================================================================================*/
    //Genero los 2 bit vectors.
    string tempH[largoARR] = {""};
    int counter = 0;
    int X = 0;
    int aux = 0;
    bit_vector R(largoR, 0);

    for (int i = 0; i < largoARR; i++)
    {
        temp = bitset<30>(x[i]).to_string();
        tempA = temp.substr(0, logN);
        tempH[i] = tempA;
        temp.erase(0, logN);
        for (int t = 0; t < diff; t++)
        {
            if (temp[t] == '1')
                R[(2 * i) + t] = 1;
        }
        X = convertBinaryToDecimal(stoll(tempA));
        xPrima[i] = X;
    }
    Gc[0] = xPrima[0];
    for (int i = 1; i < largoARR; i++)
    {
        Gc[i] = (xPrima[i] - xPrima[i - 1]);
    }

    for (int i = 0; i < largoARR; i++)
    { //Calculo cuantos bits necesito para los unarios.
        counter += (Gc[i] + 1);
        //cout << tempH[i] << " " << X << " " << endl;
    }

    //cout << "Bits arreglos: " << counter << " " << largoR << endl;
    //cout << "Bits vector original: " << sizeof(int) * largoARR;
    bit_vector H(counter, 0);
    counter = 0;
    H[0] = 1;
    for (int i = 1; i < largoARR; i++)
    {
        counter += (Gc[i] + 1);
        H[counter] = 1;
    }

    //cout << endl;
    uint auxi = 0;
    double t = 0.0;
    double tiempoTotal = 0.0;
    //cout <<"Bits H "<< H << endl;
    //cout <<"Bits R " << R <<  endl;
    for (int i = 0; i < TESTING; i++)
    {
        t = getTime_ms();
        auxi = rand() % largoARR;
        //cout << auxi << endl;
        buscar(auxi, H, R);
        tiempoTotal += (getTime_ms()-t);
    }
    double TTotal = sizeof(int) * largoARR;
    double TComp = (counter + largoR)/8;
    double Ratio = TComp / TTotal;
    tiempoTotal = (tiempoTotal/TESTING);
    cout << "Elias Fano" << endl;
    cout << "Tamaño arreglo original: " << TTotal;
    cout << "Tamaño arreglos comprimidos: " << TComp;
    cout << "Ratio de compresion: " << TComp / TTotal << endl;
    cout << "Tiempo promedio de consulta: " << tiempoTotal <<"ms" << endl;

    ofstream Resultados;
    Resultados.open ("Resultados2.csv");
    Resultados << "Esquema,Tiempo promedio (ms),Ratio de compresion, Bits arreglo original, Bits comprimido,/n";
    Resultados <<"Elias Fano,"<< tiempoTotal<<","<<Ratio<<","<<TTotal<<","<<TComp<<"\n";

    if (Gc)
        delete[] Gc;
    if (x)
        delete[] x;

    //Elias-Fano
}