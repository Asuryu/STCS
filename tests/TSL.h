#ifndef THERMAL_SIMULATION_LIBRARY_H
#define THERMAL_SIMULATION_LIBRARY_H

#include <stdint.h>

typedef enum {NORMAL, ECLIPSE, SUN_EXPOSURE} period_t;

void initialize_simulation_environment(void);
void update_thermistor_values(void);
int get_thermistor_temperature(int id);
void set_heater_status(int id, int status);
int get_heater_status(int id);
period_t get_current_period(void);
void log_simulation_data(void);

#endif // THERMAL_SIMULATION_LIBRARY_H
 