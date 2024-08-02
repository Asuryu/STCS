#include "pid.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 

//recieeved_temp is the temperature that gets in the function
//integral_PID is the integral variable to be used in the actual iteration of the control action -> u(t)
//initial_temp are the temperatures provided in the prior iteration, used to compute the actual derivative variable
//d_before are the prior derivative values provided to compute the actual varriable
//h is the step, i.e. 1/frequency 
//set_point it's the goal temperature
void pid(float received_temp[4], float integral_PID[4], float initial_temp[4], float initial_derivate_PID[4], float step, float set_point) { //AINDA FALTA DEFINIR O RESTO!!!
    
    //Definition of the controller constants
    float K, T_i, T_d, N, b;    
    int j; 
    
    float *e, *p, *d, *u; 
    e = (float *)calloc(4, sizeof(float)); 
    p = (float *)calloc(4, sizeof(float)); 
    d = (float *)calloc(4, sizeof(float));
    u = (float *)calloc(4, sizeof(float));

    if(!e || !p || !d || !u) { 
        printf("ALLOCATION ERROR! \n"); 
    }

    //Constant Definition
    N = 8; //Typical values vary between 8 and 20
    K = 0.25; //Proportional Constant
    T_i = 10; //Integration Time Constant
    T_d = 5;  //Derivative Time Constant
    b = 0.5;  //Second Porporcional Constant    

    //Definir o erro
    for(j = 0; j < 4; j++){ 
        //For now, let K be the Proportional Constant and k the actual iteration
        //Define the error function, e(t_k) = SET_POINT - VALOR_ATUAL
        e[j] = set_point - received_temp[j]; 
        //Define the proportional function of the PID controller 
        //p(t) = K * (b * set_point - valor_atual)
        p[j] = K * (b * set_point - received_temp[j]); 
        //Define the actual derivative value, d(t_k)
        //d(t_k) = T_d * (1/(T_d+N*step)) * d_(t_k-1) - (K * T_d * N) * (1/(T_d + N * step)) * (v-valor_antigo)
        d[j] = T_d * pow(T_d + N * step, -1) * initial_derivate_PID[j] - (K * T_d * N) * pow(T_d + N * step, -1) * (received_temp[j] - initial_temp[j]);  
        //Define the action of the controller
        //u(t) = p(t) + i(t) + d(t)
        u[j] = p[j] + integral_PID[j] + d[j]; 
        
        //NÃO ATIVAR, QUE DÁ ERRO!
        //write_to_log(e[j], p[j], d[j], i[j], u[j]); 
        
        //Define the integration value of the PID controller to be used 
        //i(t_futura) = i(t) + (K * passo) * (1/T_i)
        integral_PID[j] = integral_PID[j] + (K * step) * pow(T_i, -1) * e[j]; 
        
        //Preparar os valores da derivada atual e da temperatura atual para serem usados na próxima iteração
        initial_derivate_PID[j] = d[j]; 
        initial_temp[j] = received_temp[j]; 

        //CÓDIGO DE TESTE - DEPOIS ALTERAR PARA ZEROS E UNS! 
        //É aqui que se decide se o aquecedor liga-se ou não
        if(u[j] > 0) { 
            received_temp[j] = received_temp[j] + 1; 
        } else { 
            received_temp[j] = received_temp[j] - 0.2;
        }

        //Se nos der para mudar a resistência do termómetro, melhor ainda, basta alterar esta função com os valores da resistência a aplicar     
        //received_temp[j] = received_temp[j] + u[j]; 
    }  
    free(e); 
    free(p); 
    free(d); 
    free(u); 
}
