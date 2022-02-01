#ifndef REPORT_H
#define REPORT_H

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

#endif
