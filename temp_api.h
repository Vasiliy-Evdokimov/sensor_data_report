#include <stdint.h>

typedef struct {
	char file_name[255];
	int year_no;
	char month_no;
	int year_no2;
	char month_no2;
} arguments;

typedef struct {
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;
	uint64_t encoded_datetime;
	int8_t t;
} sensor;

typedef struct {
	char file_name[255];
	uint32_t lines_processed;
	uint32_t lines_approved;
	uint32_t lines_rejected;
	uint64_t min_datetime;
	uint64_t max_datetime;
} readFileResults;

void PrintCharString(int count, char fill_char);

void PrintReadFileResults(readFileResults* rfr);

int ReadFile(char file_name[], int* size, sensor** data, readFileResults* rfr);

void PrintHelp(char app_name[]);

int ProcessArguments(int argc, char *argv[], arguments* args);

void PrintArguments(arguments* args);

unsigned int DateToInt(sensor* info);

uint64_t EncodeDateTime(uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute);

void DecodeDateTime(uint64_t DateTime, uint16_t* year, uint8_t* month, 
	uint8_t* day, uint8_t* hour, uint8_t* minute);	

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
