#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include"pid.h" 

void clear_file() { 
    FILE *f = fopen("temperatures.txt", "w"); 
    FILE *g = fopen("log.txt", "w"); 
    time_t t = time(NULL); 
    struct tm tm = *localtime(&t); 
    fprintf(f, "Time: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); 
    fprintf(g, "Time: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); 
    fclose(f); 
}

int write_to_file(float time, float temperature[]){ 
    int j = 4; 
    FILE *f; 
    f= fopen("temperatures.txt", "a"); 
    if(f == NULL) { 
        return 1; 
    }
    fprintf(f, "%0.2f \t", time); 
    for(j = 0; j < 4; j++) { 
        fprintf(f, "%0.2f \t", temperature[j]); 
    } 
    fprintf(f, "\n");
    fclose(f); 
    return 0; 
}

void write_to_log(float e, float p, float d, float i, float u) { 
    FILE *f = fopen("log.txt", "a"); 
    fprintf(f, "---Tentativa--- \nErro: %f \nP: %f \nD: %f \nI: %f \nCorrecao a aplicar: %f \n", e, p, d, i, u); 
    fclose(f); 
}

int main(int argc, char *argv[]) { 
    int j, chk; 
    float frequency, set_point; 
    float integral[] = {0, 0, 0, 0}; 
    float derivada[] = {0, 0, 0, 0}; 
    float t[] = {-3, 5, 7, -8}; 
    float t_before[] = {-3, 5, 7, -8}; 
    float t_0 = 0; //First time 
    frequency = 5; //Hertz 
    set_point = 10; 

    clear_file(); 
    chk = write_to_file(t_0, t);
    for(j = 0; j < 1000; j++) { 
        t_0 = t_0 + pow(frequency, -1); 
        pid(t, integral, t_before, derivada, 1/frequency, set_point); 
        chk = write_to_file(t_0, t);
        if(chk == 1) { 
            printf("ERROR when reading the file! \n"); 
            return 1; 
        } 
    } 
    set_point = -2; 
    for(j = 0; j < 1000; j++) { 
        t_0 = t_0 + pow(frequency, -1); 
        pid(t, integral, t_before, derivada, 1/frequency, set_point); 
        chk = write_to_file(t_0, t);
        if(chk == 1) { 
            printf("ERROR when reading the file! \n"); 
            return 1; 
        } 
    } 


    printf("Program Finished!\n"); 
    return 0; 
} 


