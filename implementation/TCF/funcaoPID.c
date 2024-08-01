#include "func.h"

int funcaoPID(int Temperaturainsert, int h) {
    int i;
    float Temperatura[4] = {0,0,0,0};  
    //float Tempo[4] = {0};  

    float E[4] = {0,0,0,0}; 
    float V[4] = {0,0,0,0}; 

    float Kp = 3.0;
    float Ki = 1.0;
    float Kd = 0.05;

    float Tref[4]={0,0,0,0};

    for (i = 0; i < 4; i++) {
        Tref[i] = Temperaturainsert;
    }

    for (i = 1; i < 4; i++) {
        E[i] = Tref[i] - Temperatura[i];
        V[i] = V[i-1] + E[i] * (Kp + Ki * h / 2 + 2 * Kd / h) +
               E[i-1] * (Ki * h - 4 * Kd / h) +
               E[i-2] * (-Kp + Ki * h / 2 + 2 * Kd / h);
    }

    return V[3];
}
