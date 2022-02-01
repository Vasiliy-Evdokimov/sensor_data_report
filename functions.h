#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define DEBUG

#ifdef DEBUG
	#define DBG if(1)
#else
	#define DBG if(0)
#endif

// STRUCTURES

typedef struct {
	char app_name[255];
	char file_name[255];
	int year_no;
	char month_no;
	int year_no2;
	char month_no2;
	char locale_id;
} arguments;

typedef struct {
	char file_name[255];
	int lines_processed;
	int lines_approved;
	int lines_rejected;
	unsigned long long min_datetime;
	unsigned long long max_datetime;
} readFileResults;

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
	char month;
	short year;	
	int encoded_my;
	int count;
	signed int sum_t;
	signed char min_t;
	signed char max_t;	
	float avg_t;
} monthReport;

// APPLICATION ARGUMENTS

int ProcessArguments(int argc, char *argv[], arguments* args);

void PrintArguments(arguments* args);

// READ FILE

int CountFileLines(char file_name[]);

void WriteErrorsFile(const char file_name[], int errors_count, char* errors_array);

int ReadFile(char file_name[], int* size, sensor** data, readFileResults* rfr);

void PrintReadFileResults(readFileResults* rfr);

// SENSORS

unsigned long long SensorsEncodeDateTime(sensor* info);

void SensorsSwapByIndex(sensor* info, int i, int j);

void SensorsOrderByT(sensor* info, int n);

void SensorsOrderByDate(sensor* info, int n);

void SensorsAddRecord(sensor** info, int number,
	short year, char month, char day, 
	char hour, char minute, signed char t);

void SensorPrint(sensor* item);

void SensorsPrint(sensor* info, int number);

// REPORT

int MonthReportIndex(int months_count, monthReport* months, 
	int year_no, int month_no);

void MonthPrint(monthReport* month);

void MonthsPrint(int months_size, monthReport* months);

void MonthInit(monthReport** report_month, short year, char month, signed char t);

void MonthProcessNewData(monthReport** report_month, signed char t);

void MonthsSwapByIndex(monthReport* months, int i, int j);

void MonthOrderByDate(int month_size, monthReport* months);

void PrintReportLine(short year, char month, short count,
	signed char min_t, signed char max_t, float avg_t);

void ReportGetPeriodFromArgs(arguments app_args, 
	unsigned long long* start_date, unsigned long long* final_date);

void ReportPrintTitle(arguments app_args, unsigned long long start_date, 
	unsigned long long final_date);

void ReportGetValues(int data_size, sensor* data, arguments app_args, 
	readFileResults read_file_results);

// USER INTERFACE

int ShowMenu(arguments* args, readFileResults* rfr);

void PrintAppTitle(const char title[]);

void InitProgressBar();

void PrintProgressBar(double percentage) ;

void PrintHelp(char app_name[]);

#endif
