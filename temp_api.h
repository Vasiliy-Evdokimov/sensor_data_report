#include <stdint.h>

typedef struct {
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;
	uint64_t encoded_datetime;
	int8_t t;
} sensor;

int ReadFile(char file_name[], int* size, sensor** data);

void PrintHelp(char app_name[]);

int ProcessArguments(int argc, char *argv[]);

unsigned int DateToInt(sensor* info);

uint64_t EncodeDateTime(uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute);

void SensorsSwapByIndex(sensor* info, int i, int j);

void SensorsOrderByT(sensor* info, int n);

void SensorsOrderByDate(sensor* info, int n);

void SensorsAddRecord(sensor** info, int number,
	uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute, int8_t t);

void SensorsPrint(sensor* info, int number);

uint64_t SensorsEncodeDateTime(sensor* info);

void GetHelpInfo();

void ReportGetHeader(uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute);
	
void ReportGetValues(sensor* info, uint64_t period_start, uint64_t period_final);
