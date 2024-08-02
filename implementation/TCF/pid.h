#ifndef PID_H
#define PID_H


//t_in -> received_temp
//i -> inetegral_PID
//t_before -> initial_temp
//d_before -> initial_derivate_PID
//h -> step
//set_point é a temperatura de objetivo
void pid(float received_temp[4], float integral_PID[4], float initial_temp[4], float initial_derivate_PID[4], float step, float set_point, int response[4]); 

#endif