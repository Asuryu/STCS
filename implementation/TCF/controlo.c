#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "func.h"

int main(){

    int h=5;
    int num_interacoes = 10;
    int Tref=-5;
    int i;
    int temp [10] = {0,1,2,3,4,5,6,7,8,9};
    int estadoanterior [10] = {1,0,1,0,1,1,1,1,1,1};

    int v = funcaoPID(Tref,h,temp,estadoanterior);
    printf("Valor de controlo PID: %d\n", v);

    return 0;
}