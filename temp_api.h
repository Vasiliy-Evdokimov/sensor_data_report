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
	char day;
	char month;
	short year;
	char hour;
	char minute;
	unsigned long long encoded_datetime;
	signed char t;
} sensor;

typedef struct {
	char file_name[255];
	int lines_processed;
	int lines_approved;
	int lines_rejected;
	unsigned long long min_datetime;
	unsigned long long max_datetime;
} readFileResults;

typedef struct {
	char month;
	short year;	
	int encoded_my;
	int count;
	signed int sum_t;
	signed char min_t;
	signed char max_t;	
	float avg_t;
} monthReport;

void PrintAppTitle(const char title[]);

int ShowMenu(arguments* args, readFileResults* rfr);

void PrintTime();

void PrintCharString(int count, char fill_char, char next_line);

void PrintReadFileResults(readFileResults* rfr);

int ReadFile(char file_name[], int* size, sensor** data, readFileResults* rfr);

void PrintHelp(char app_name[]);

int ProcessArguments(int argc, char *argv[], arguments* args);

void PrintArguments(arguments* args);

unsigned int DateToInt(sensor* info);

unsigned long long EncodeDateTime(short year, char month, char day, 
	char hour, char minute);

void DecodeDateTime(unsigned long long DateTime, short* year, char* month, 
	char* day, char* hour, char* minute);	

void SensorsSwapByIndex(sensor* info, int i, int j);

void SensorsOrderByT(sensor* info, int n);

void SensorsOrderByDate(sensor* info, int n);

void SensorsAddRecord(sensor** info, int number,
	short year, char month, char day, 
	char hour, char minute, signed char t);

void SensorsPrint(sensor* info, int number);

unsigned long long SensorsEncodeDateTime(sensor* info);

void ReportGetValues(int data_size, sensor* data, arguments app_args, 
	readFileResults read_file_results);
