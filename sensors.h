#ifndef SENSORS_H
#define SENSORS_H

unsigned long long SensorsEncodeDateTime(sensor* info);

void SensorsSwapByIndex(sensor* info, int i, int j);

void SensorsOrderByT(sensor* info, int n);

void SensorsOrderByDate(sensor* info, int n);

void SensorsAddRecord(sensor** info, int number,
	short year, char month, char day, 
	char hour, char minute, signed char t);

void SensorPrint(sensor* item);

void SensorsPrint(sensor* info, int number);

#endif
