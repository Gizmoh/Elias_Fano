
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/util.hpp>
#include <math.h>

using namespace std;
using namespace sdsl;

#define INC 128
#define sampling 3


int main (int argc, char** argv){

    double r;
    int X [12]={0};
    int Y [12]={0};
    int S [12/sampling];


    for(int i = 0; i< 12; i++){
        r = rand()%INC;
        X[i]=X[i-1]+r;
        cout<< r << " " << X[i] << endl;
    }
    for (int i=0; i<12; i++){
        cout << X[i] << endl;
    }
    Y[0] = X[0];
    for(int i=1;i<12;i++){
        Y[i]=X[i]-X[i-1];
    }
    for(int i = 0; i < 12/sampling;i++){
        S[i] = X[i*sampling];
        cout << S[i] << endl;
    }
}