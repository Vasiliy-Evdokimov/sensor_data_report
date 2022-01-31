#include <stdint.h>

#define DEBUG

#ifdef DEBUG
	#define DBG if(1)
#else
	#define DBG if(0)
#endif

typedef struct {
	char file_name[255];
	int year_no;
	char month_no;
	int year_no2;
	char month_no2;
	char locale_id;
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

typedef struct {
	uint8_t month;
	uint16_t year;	
	uint32_t encoded_my;
	uint32_t count;
	int32_t sum_t;
	int8_t min_t;
	int8_t max_t;	
	float avg_t;
} monthReport;

void PrintAppTitle(const char title[]);

int ShowMenu(arguments* args);

void PrintTime();

void PrintCharString(int count, char fill_char, char next_line);

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

void ReportGetValues(int data_size, sensor* data, arguments app_args, 
	readFileResults read_file_results);
