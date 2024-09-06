#include "PID.h"

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
void pid(float received_temp[4], float integral_PID[4], float initial_temp[4], float initial_derivate_PID[4], float step, float set_point[4], int response[4]) { //AINDA FALTA DEFINIR O RESTO!!!
    
    //Definition of the controller constants
    float K, T_i, T_d, N, b;
    
    
    //Define the error, proprotiona, derivative and control variables
    float *error, *proportional, *derivative, *control_signal; 
    error = (float *)calloc(4, sizeof(float)); 
    proportional = (float *)calloc(4, sizeof(float)); 
    derivative = (float *)calloc(4, sizeof(float));
    control_signal = (float *)calloc(4, sizeof(float));

    if(!error || !proportional || !derivative || !control_signal) { 
        printf("ALLOCATION ERROR! \n"); 
    }

    //Constant Definition
    N = 8; //Undefined Constant, typical values vary between 8 and 20
    K = KP; //Proportional Constant
    T_i = KI; //Integration Time Constant
    T_d = KD;  //Derivative Time Constant
    b = 1;  //Second Porporcional Constant, changed to 1 to avoid constraints with the definition of K_i, K_p and K_d    

    //Definir o erro
    for(int j = 0; j < 4; j++){ 
        //For now, let K be the Proportional Constant and k the actual iteration
        //Define the error function, e(t_k) = SET_POINT - VALOR_ATUAL
        error[j] = set_point[j] - received_temp[j]; 
        //Define the proportional function of the PID controller 
        //p(t) = K * (b * set_point - valor_atual)
        proportional[j] = K * (b * set_point[j] - received_temp[j]); 
        //Define the actual derivative value, d(t_k)
        //d(t_k) = T_d * (1/(T_d+N*step)) * d_(t_k-1) - (K * T_d * N) * (1/(T_d + N * step)) * (t_k-t_k-1)
        derivative[j] = T_d * pow(T_d + N * step, -1) * initial_derivate_PID[j] - (K * T_d * N) * pow(T_d + N * step, -1) * (received_temp[j] - initial_temp[j]);  
        //Define the action of the controller
        //u(t_k) = p(t_k) + i(t_k) + d(t_k)
        control_signal[j] = proportional[j] + integral_PID[j] + derivative[j]; 
        
        //NÃO ATIVAR, QUE DÁ ERRO!
        //write_to_log(e[j], p[j], d[j], i[j], u[j]); 
        
        //Define the integration value of the PID controller to be used 
        //i(t_futura) = i(t) + (K * passo) * (1/T_i)
        integral_PID[j] = integral_PID[j] + (K * step) * pow(T_i, -1) * error[j]; 
        
        //Prepare the values of the current derivative and the curretn temperature to be used o the next iteration
        initial_derivate_PID[j] = derivative[j]; 
        initial_temp[j] = received_temp[j]; 

        //CÓDIGO DE TESTE - DEPOIS ALTERAR PARA ZEROS E UNS! 
        //Based on te values, it is decided whether the heater values are turned ON or OFF
        if(control_signal[j] > 0) { 
            response[j] = 1; 
        } else { 
            response[j] = 0;
        }

        //Se nos der para mudar a resistência do termómetro, melhor ainda, basta alterar esta função com os valores da resistência a aplicar     
        //received_temp[j] = received_temp[j] + u[j]; 
    } 
    
    //Delete the error, proprotional, derivative and control signal variables 
    free(error); 
    free(proportional); 
    free(derivative); 
    free(control_signal); 
}
