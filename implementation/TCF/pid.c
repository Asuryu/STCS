#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 

void pid(float t_in[4], float i[4], float t_before[4], float d_before[4], float h, float set_point) { //AINDA FALTA DEFINIR O RESTO!!!
    
    //Definir variáveis que iremos alterar para testar o controlador
    float K, T_i, T_d, N, b;    
    int j; 
    //Definir a função e(t) = SET_POINT - VALOR_ATUAL
    //float e[4], p[4], d[4], u[4]; 

    float *e, *p, *d, *u; 
    e = (float *)calloc(4, sizeof(float)); 
    p = (float *)calloc(4, sizeof(float)); 
    d = (float *)calloc(4, sizeof(float));
    u = (float *)calloc(4, sizeof(float));

    if(!e || !p || !d || !u) { 
        printf("ALLOCATION ERROR! \n"); 
    }

    //Definição de Constantes 
    N = 8; //Valores típicos variam entre o 8 e o 20 
    K = 0.25;
    T_i = 10; 
    T_d = 5; 
    b = 0.5; 

    //Definir o erro
    for(j = 0; j < 4; j++){ 
        e[j] = set_point - t_in[j]; 
        p[j] = K * (b * set_point - t_in[j]); 
        d[j] = T_d * pow(T_d + N * h, -1) * d_before[j] - (K * T_d * N) * pow(T_d + N * h, -1) * (t_in[j] - t_before[j]);  
        u[j] = p[j] + i[j] + d[j]; 
        
        //NÃO ATIVAR!
        //write_to_log(e[j], p[j], d[j], i[j], u[j]); 
        
        i[j] = i[j] + (K * h) * pow(T_i, -1) * e[j]; 
        d_before[j] = d[j]; 
        t_before[j] = t_in[j]; 

        //TEST CODE
        if(u[j] > 0) { 
            t_in[j] = t_in[j] + 1; 
        } else { 
            t_in[j] = t_in[j] - 0.2;
        }

        //Se nos der para mudar a resistência do termómetro, alterar esta função    
        //t_in[j] = t_in[j] + u[j]; 
    }  
    free(e); 
    free(p); 
    free(d); 
    free(u); 
}
