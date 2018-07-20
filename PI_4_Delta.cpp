#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "include/BasicCDS.h"

using namespace std;
using namespace sdsl;
using namespace cds;

#define INC 64
#define sampling 8192
#define largoARR 65536
#define TESTING 1000

/*========================================================================================================*/

int genInterval(int X[], int mayor)
{ //Calculo el techo del intervalo de forma que 10% quede adentro
    int contadorI;
    int y = mayor;
    double percent = 1;
    while (percent > 0.9)
    {
        y--;
        contadorI = 0;
        percent = 1;
        for (int i = 0; i < largoARR; i++)
        {
            if (X[i] < y)
            {
                contadorI++;
            }
        }
        percent = double(contadorI) / double(largoARR);
    }
    return y;
}
/*========================================================================================================*/
double leerElemento(int pos, bit_vector Ex, ulong *Samples, ulong *Gaps, ulong *Excep, int mayor, int Smayor, int BitGapMayor)
{
    int SamplePos = pos / sampling;
    int Start = (SamplePos * sampling);
    int solucion = getNum64(Samples, (Smayor * (SamplePos)), Smayor);
    int r;
    double t = 0.0;
    rank_support_v<1> rankEx_1(&Ex);
    rank_support_v<0> rankEx_0(&Ex);
    bit_vector::select_1_type Ex_Sel1(&Ex);
    bit_vector::select_0_type Ex_Sel0(&Ex);
    t = getTime_ms();
    for (int i = Start; i < pos; i++)
    {
        //cout<< "Valor de i: " << i << " ";
        if (Ex[i] == 0)
        { /*
            cout <<" Select: "<<Ex_Sel0(i) << " Rank: ";
            cout << rankEx_0(i) << " (0) corresponde a " << getNum64(Gaps,(mayor*(rankEx_0(i-SamplePos+1))),mayor) << ", SamplePos: " << SamplePos << endl;
            cout << "Siguiente: " << getNum64(Gaps,(mayor*(rankEx_0(i-SamplePos+1+1))),mayor) << endl;
            cout << "Anterior: " << getNum64(Gaps,(mayor*(rankEx_0(i-SamplePos+1-1))),mayor) << endl;*/
            r = (rankEx_0(i) - SamplePos);
            solucion += getNum64(Gaps, (BitGapMayor * r), BitGapMayor);
        }
        else
        { /*
            cout << rankEx_1(i)<< " (1) corresponde a " << getNum64(Excep,(mayor*(rankEx_1(i))),mayor) << endl;
            cout << "Siguiente: " << getNum64(Excep,(mayor*(rankEx_1(i+1))),mayor) << endl;
            cout << "Anterior: " << getNum64(Excep,(mayor*(rankEx_1(i-1))),mayor) << endl;*/
            solucion += getNum64(Excep, (mayor * (rankEx_1(i))), mayor);
        }
    }
    t = getTime_ms() - t;
    //cout << "Numero calculado: " << solucion;
    return t;
}
/*========================================================================================================*/
void storeSamples(ulong *Samples, int X[], uint BitA, int nCellsS)
{
    int i, j;
    for (i = j = 0; i < nCellsS; i++, j += BitA)
    {
        setNum64(Samples, j, BitA, X[i * sampling]); /*
        cout <<"Posicion: " << i*sampling << endl;
        cout << "Numero real: " << X[i*sampling] << endl;
        cout <<"Numero almacenado: " << getNum64(Samples,j, BitA) << endl;*/
    }
}
/*========================================================================================================*/
void Storage(int Y[], ulong *Gaps, ulong *Excep, bit_vector Ex, int Bits, int BitGapMayor)
{
    int counterA = 0;
    int counterB = 0;
    double t = 0.0;

    t = getTime_ms();
    for (int i = 1; i < largoARR; i++)
    { //Si el elemento es uno de los que se encuentran en el sampling, se cambia el numero en el bit_vector a 0 y se salta.
        if (i % (sampling) == 0)
        {
            Ex[i] = 0;
            //cout << "posicion: " << i << " numero: " << Y[i] << endl;
            continue;
        }
        if (Ex[i] == 0)
        {                                                //Si en el bit_vector el valor es 0, guarda el elemento correspondiente del arreglo X en el ulong de gaps.
            setNum64(Gaps, counterA, BitGapMayor, Y[i]); /*
            cout << "Dentro intervalo, Numero real: " << Y[i] << endl;
            cout <<"Numero almacenado: " << getNum64(Gaps,counterA, Bits) << endl;*/
            counterA += BitGapMayor;
        }
        else
        {                                          //Si en el bit_vector el valor es 1, guarda el elemento correspondiente del arreglo X en el ulong de excepciones.
            setNum64(Excep, counterB, Bits, Y[i]); /*
            cout << "Fuera intervalo, Numero real: " << Y[i] << endl;
            cout <<"Numero almacenado: " << getNum64(Excep,counterB, Bits) << endl;*/
            counterB += Bits;
        }
    }
    t = getTime_ms() - t;
    cout << "Tiempo de compresion: " << t <<" ms" << endl;
}

/*========================================================================================================*/

int main(int argc, char **argv)
{ // Recibe como argumento el largoARR del arreglo
    double r = 0;
    int mayor = 0;
    int X[largoARR] = {};
    int Y[largoARR] = {};
    double test = double(largoARR / sampling);
    int nCellsS = largoARR / sampling;
    if (largoARR % sampling)
        nCellsS++;
    int nCellG = largoARR - nCellsS;
    int S[nCellsS] = {};
    ulong *Gaps, *Excep, *Samples;
    bit_vector Ex(largoARR, 0);

    for (int i = 0; i < largoARR; i++)
    { //Genero el arreglo no decreciente
        r = rand() % INC;
        X[i] = X[i - 1] + r;
    }
    Y[0] = X[0];
    mayor = X[0];
    for (int i = 1; i < largoARR; i++)
    { //Genero el arreglo de gaps
        Y[i] = X[i] - X[i - 1];
        if (mayor < Y[i])
        {
            mayor = Y[i];
        }
    }
    S[1] = Y[1];
    for (int i = 1; i < round(largoARR / sampling); i++)
    { //Genero el sampling
        S[i] = X[i * sampling];
    }

    uint BitMayor = (1 + int(log2(mayor)));
    uint BitSamplingMayor = (1 + int(log2(S[largoARR / sampling - 1])));
    uint GapMayor = 0;

    int top = 0;
    int contadorI = 0;
    int contadorO = 0;
    double porcentajeI = 0;
    double porcentajeO = 0;
    top = genInterval(Y, mayor);
    for (int i = 0; i < largoARR; i++)
    {
        if (top > Y[i])
        {
            if (GapMayor < Y[i])
                GapMayor = Y[i];
            contadorI++;
        }
        else
        {
            contadorO++;
            Ex[i] = 1;
        }
    }

    uint BitGapMayor = (1 + log2(GapMayor));

    int aux = nCellsS * BitSamplingMayor / W64;
    if ((nCellsS * BitSamplingMayor) % W64)
        aux++;
    int aux2 = nCellG * BitGapMayor / W64;
    if ((nCellG * BitGapMayor) % W64)
        aux2++;
    int aux3 = contadorO * BitMayor / W64;
    if ((contadorO * BitMayor) % W64)
    {
        aux3++;
    }
    /*
    cout << "Excepcion mayor: " << mayor << endl;
    cout << "Bits necesarios: " << BitMayor << endl;
    cout << "Sampling mayor: " << (S[largoARR / sampling - 1]) << endl;
    cout << "Bits necesarios: " << BitSamplingMayor << endl;
    cout << "Calculo de distribuciones:" << endl;
    cout << "Se necesitan " << (BitSamplingMayor * int(largoARR / sampling)) << " bits para almacenar el sampling" << endl;
    cout << "Bits arreglo original: " << largoARR*sizeof(int) << endl;

    cout << "sizeof Samples = " << aux * sizeof(ulong) << " bits" << endl;
    cout << "sizeof Gaps = " << aux3 * sizeof(ulong) << " bits" << endl;
    cout << "sizeof Exceptions = " << aux2 * sizeof(ulong) << " bits" << endl;*/
    Samples = new ulong[aux];
    Gaps = new ulong[aux2];
    Excep = new ulong[aux3];

    storeSamples(Samples, X, BitSamplingMayor, nCellsS);
    Storage(Y, Gaps, Excep, Ex, BitMayor, BitGapMayor);

    porcentajeI = double(contadorI) / double(largoARR);
    porcentajeO = double(contadorO) / double(largoARR);
    //cout << " Porcentaje en el intervalo: " << porcentajeI;
    //cout << ", porcentaje fuera del intervalo: " << porcentajeO << endl;
    double T_ejec = 0.0;
    int rand_X = 0;
    double sTotal = (aux * sizeof(ulong)+aux3 * sizeof(ulong)+aux2 * sizeof(ulong));
    double SArregloX = largoARR*sizeof(int);
    double SArregloG = aux3 * sizeof(ulong);
    double SArregloS = aux2 * sizeof(ulong);
    double Ratio = sTotal/double(largoARR*sizeof(int));
    for (int i = 0; i < TESTING; i++)
    {
        rand_X = rand() % largoARR;
        T_ejec += leerElemento(rand_X, Ex, Samples, Gaps, Excep, BitMayor, BitSamplingMayor, BitGapMayor);
        //cout << " Numero pedido: " << X[rand_X] << " Valor de x: "<< rand_X << endl;
    }
    double T_Total = T_ejec / double(TESTING);
    cout << "PforDelta" << endl;
    cout << "Tiempo promedio de consulta: " << T_Total << "ms" << endl;
    cout << "Bits arreglo original: " << SArregloX << " Bits comprimidos: " << (SArregloG,SArregloS) << endl;
    cout << "Ratio de compresion: " << Ratio << endl;
    cout << "Tiempo promedio de consulta: " << T_Total;

    ofstream Resultados;
    Resultados.open ("Resultados3.csv");
    Resultados << "Esquema,Tiempo promedio (ms),Ratio de compresion, Bits arreglo original, Bits comprimido,/n";
    Resultados <<"P4Delta,"<< T_Total<<","<<Ratio<<","<<SArregloX<<","<<(SArregloG+SArregloS)<<"\n";
    Resultados.close();
    exit;
}