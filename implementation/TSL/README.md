# Thermal Simulation Library (TSL)

## How to run TSL
Do the following steps inside TSL dir to run TSL:
```
cd ../bin
./TSL.o
```

## Interaction with TSL
TSL is a process running cyclically which is constantly writting data to a TEMP_INFO_PIPE pipe and reading data from the RESPONSE_PIPE pipe. 
The following message formats should be kept in mind while interacting with TSL:

### Writting to TEMP_INFO_PIPE (TSL -> TCF)
```
{THERM-01_TEMP}-{HTR-01_PWR};{THERM-02_TEMP}-{HTR-02_PWR};{THERM-03_TEMP}-{HTR-03_PWR};{THERM-04_TEMP}-{HTR-04_PWR}
```
- {THERM-01_TEMP} - Temperature of the first thermistor
- {HTR-01_PWR} - Power of the first heater
- {THERM-02_TEMP} - Temperature of the second thermistor 
- {HTR-02_PWR} - Power of the second heater
- {THERM-03_TEMP} - Temperature of the third thermistor 
- {HTR-03_PWR} - Power of the third heater
- {THERM-04_TEMP} - Temperature of the fourth thermistor 
- {HTR-04_PWR} - Power of the fourth heater

### Reading from RESPONSE_PIPE (TCF -> TSL)
```
{HTR-01_PWR};{HTR-02_PWR};{HTR-03_PWR};{HTR-04_PWR}
```
- {HTR-01_PWR} - Power of the first heater
- {HTR-02_PWR} - Power of the second heater
- {HTR-03_PWR} - Power of the third heater
- {HTR-04_PWR} - Power of the fourth heater