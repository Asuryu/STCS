#ifndef FUNC_H
#define FUNC_H

int funcaoPID(int TemperaturaRef, int h, int *Temperatura, int *V) {

    int i;
    //float Tempo[4] = {0};
    int num_dados = sizeof(Temperatura)/sizeof(Temperatura[0]);
    float E[num_dados];

    float Kp = 3.0;
    float Ki = 1.0;
    float Kd = 0.05;

    float Tref[num_dados];

    float Treal[num_dados];

    for (i = 0; i < 4; i++) {
        Tref[i] = TemperaturaRef;
    }

    for (i = 2; i < 10; i++) {
        E[i] = Tref[i] - Temperatura[i];
        V[i] = V[i-1] + E[i] * (Kp + Ki * h / 2 + 2 * Kd / h) +
               E[i-1] * (Ki * h - 4 * Kd / h) +
               E[i-2] * (-Kp + Ki * h / 2 + 2 * Kd / h);
    }

    return V[num_dados -1];
}
#endif