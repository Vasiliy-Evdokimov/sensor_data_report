#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#include "temp_api.h"
#include "localization.h"
#include "utils.h"
#include "interface.h"


int ProcessArguments(int argc, char *argv[], arguments* args)
{	
	args->year_no = 0;
	args->month_no = 0;
	args->year_no2 = 0;
	args->month_no2 = 0;
	args->locale_id = 0;
	//
	int res = 0;
	while ((res = getopt(argc, argv, "hf:y:m:a:b:L:")) != -1) 
	{
		switch (res) {
			case 'h':
				PrintHelp(argv[0]);
                return 0;
			case 'f':				
				strcpy(args->file_name, optarg);
				break;
			case 'y':
				args->year_no = atoi(optarg); 			
				break;	
			case 'm':
				args->month_no = atoi(optarg); 
				break;	
			case 'a':
				args->year_no2 = atoi(optarg); 			
				break;	
			case 'b':
				args->month_no2 = atoi(optarg); 
				break;	
			case 'L':
				args->locale_id = atoi(optarg); 
				break;	
			case '?': 
				printf("Unknown argument: %s ", argv[optind - 1]);
				printf("Try -h for help\n");
                return 0;
		}
	}
	return 1;
}

void PrintArguments(arguments* args)
{
	PrintCharString(DELIMETER_WIDTH, '-', 1);
	printf(GetLC(FILE_IS), args->file_name);
	printf(GetLC(YEAR_IS), args->year_no); 
	printf(GetLC(MONTH_IS), 
		(args->month_no > 0) 
			? GetMonthName(args->month_no)
			: GetLC(NOT_CHOSEN));
	printf(GetLC(YEAR2_IS), args->year_no2); 
	printf(GetLC(MONTH2_IS), 
		(args->month_no2 > 0) 
			? GetMonthName(args->month_no2)
			: GetLC(NOT_CHOSEN));			
	PrintCharString(DELIMETER_WIDTH, '-', 1);
}

unsigned long long SensorsEncodeDateTime(sensor* info)
{
	return 
		EncodeDateTime(
			info->year,
			info->month,
			info->day,
			info->hour,
			info->minute
		);
}	

void SensorsSwapByIndex(sensor* info, int i, int j)
{
	sensor temp;
	temp = info[i];
	info[i] = info[j];
	info[j] = temp;
}

void SensorsOrderByT(sensor* info, int n)
{
	for (int i = 0; i < n; i++)
		for(int j = i; j < n; j++)
			if(info[i].t >= info[j].t)
				SensorsSwapByIndex(info, i, j);
}

void SensorsOrderByDate(sensor* info, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			if ((info + i)->encoded_datetime >
				(info + j)->encoded_datetime)
				SensorsSwapByIndex(info, i, j);
}

void SensorsAddRecord(sensor** info, int number,
	short year, char month, char day, 
	char hour, char minute, signed char t)
{
	*info = (sensor*)realloc(*info, (number + 1) * sizeof(sensor));
	if (*info == NULL)
	{	
		printf("Memory not allocated.\n");
		return;
	}        
	//
	sensor* new_value = *info + number;
	//
	new_value->year = year;
	new_value->month = month;
	new_value->day = day;
	new_value->hour = hour;
	new_value->minute = minute;
	new_value->encoded_datetime = 
		SensorsEncodeDateTime(new_value);
	new_value->t = t;
}

void SensorPrint(sensor* item)
{
	printf("%04d-%02d-%02d %02d:%02d %llu t=%3d\n",
		item->year,
		item->month,
		item->day,
		item->hour,
		item->minute,
		item->encoded_datetime,
		item->t
	);
}

void SensorsPrint(sensor* info, int number)
{
	PrintCharString(DELIMETER_WIDTH, '=', 1);
	for (int i = 0; i < number; i++)
		SensorPrint(&info[i]);
}

int MonthReportIndex(int months_count, monthReport* months, 
	int year_no, int month_no)
{
	int result = -1;
	for (int i = 0; i < months_count; i++)
		if ((months[i].year == year_no) &&
			(months[i].month == month_no))
		{
			result = i;
			break;
		}	
	return result;	
}

void MonthPrint(monthReport* month)
{
	printf("%d %d %d %d %d %d %d\n",
		month->year,
		month->month,
		month->encoded_my,
		month->count,
		month->sum_t,
		month->min_t,
		month->max_t
	);
}

void MonthsPrint(int months_size, monthReport* months)
{
	PrintCharString(DELIMETER_WIDTH, '=', 1);
	for (int i = 0; i < months_size; i++)
		MonthPrint(&months[i]);
}

void MonthInit(monthReport** report_month, short year, char month, signed char t)
{
	monthReport* m = *report_month;
	m->year = year;
	m->month = month;
	m->encoded_my = year * 100 + month;
	m->count = 0;
	m->sum_t = 0;
	m->min_t = t;
	m->max_t = t;	
}

void MonthProcessNewData(monthReport** report_month, signed char t)
{
	monthReport* m = *report_month;
	m->count++;
	m->sum_t += t;
	if (m->min_t > t)
		m->min_t = t;
	if (m->max_t < t)
		m->max_t = t;	
}

void MonthsSwapByIndex(monthReport* months, int i, int j)
{
	monthReport temp;
	temp = months[i];
	months[i] = months[j];
	months[j] = temp;
}

void MonthOrderByDate(int month_size, monthReport* months)
{
	for (int i = 0; i < month_size; i++)
		for (int j = i; j < month_size; j++)
			if ((months + i)->encoded_my >
				(months + j)->encoded_my)
				MonthsSwapByIndex(months, i, j);
}

void PrintReportLine(short year, char month, short count,
	signed char min_t, signed char max_t,	float avg_t)
{
	printf("%8d | %10s | %8d | %8d | %8d | %10.3f |\n",
		year,  GetMonthName(month), count, min_t, max_t, avg_t);
}	

void ReportGetPeriodFromArgs(arguments app_args, 
	unsigned long long* start_date, unsigned long long* final_date)
{
	*start_date = 0;
	*final_date = 0;
	//
	if (app_args.year_no && !app_args.month_no && !app_args.year_no2)
	{
		*start_date = EncodeDateTime(app_args.year_no, 1, 0, 0, 0);
		*final_date = EncodeDateTime(app_args.year_no, 12, 31, 23, 59);
	}	
	if (app_args.year_no && app_args.month_no && !app_args.year_no2)
	{		
		*start_date = EncodeDateTime(app_args.year_no, app_args.month_no, 
			1, 0, 0);
		*final_date = EncodeDateTime(app_args.year_no, app_args.month_no, 
			GetMonthDays(app_args.year_no, app_args.month_no), 23, 59);			
	}
	if (app_args.year_no && !app_args.month_no && app_args.year_no2)
	{		
		*start_date = EncodeDateTime(app_args.year_no, 1, 0, 0, 0);
		*final_date = EncodeDateTime(app_args.year_no2, 12, 31, 23, 59);			
	}
	if (app_args.year_no && app_args.month_no && 
		app_args.year_no2 && !app_args.month_no2)
	{		
		*start_date = EncodeDateTime(app_args.year_no, app_args.month_no, 
			1, 0, 0);
		*final_date = EncodeDateTime(app_args.year_no2, 12, 31, 23, 59);			
	}
	if (app_args.year_no && app_args.month_no && 
		app_args.year_no2 && app_args.month_no2)
	{		
		*start_date = EncodeDateTime(app_args.year_no, app_args.month_no, 
			1, 0, 0);
		*final_date = EncodeDateTime(app_args.year_no2, app_args.month_no2, 
			GetMonthDays(app_args.year_no2, app_args.month_no2), 23, 59);		
	}
}

void ReportPrintTitle(arguments app_args, unsigned long long start_date, unsigned long long final_date)
{
	if (app_args.year_no && !app_args.month_no && !app_args.year_no2)
		printf("%d", app_args.year_no);  
	if (app_args.year_no && app_args.month_no && !app_args.year_no2)
		printf("%s %d", GetMonthName(app_args.month_no), app_args.year_no);
	if (app_args.year_no && !app_args.month_no && app_args.year_no2)
		printf("%d - %d", app_args.year_no, app_args.year_no2);
	if (app_args.year_no && app_args.month_no && 
		app_args.year_no2 && !app_args.month_no2)
		printf("%s %d - %d", 
			GetMonthName(app_args.month_no), app_args.year_no,
			app_args.year_no2);
	if (app_args.year_no && app_args.month_no && 
		app_args.year_no2 && app_args.month_no2)
		printf("%s %d - %s %d", 
			GetMonthName(app_args.month_no), app_args.year_no,
			GetMonthName(app_args.month_no2), app_args.year_no2);
	//
	if (!app_args.year_no && !app_args.month_no && 
		!app_args.year_no2 && !app_args.month_no2)
	{
		short y; 
		char m, d, hh, mm;
		DecodeDateTime(start_date, &y, &m, &d, &hh, &mm);
		printf("%02d.%02d.%04d %02d:%02d - ", d, m, y, hh, mm); 
		DecodeDateTime(final_date, &y, &m, &d, &hh, &mm);
		printf("%02d.%02d.%04d %02d:%02d", d, m, y, hh, mm);
	}		
	//		
	printf("\n");			
}	

void ReportGetValues(int data_size, sensor* data, arguments app_args, 
	readFileResults read_file_results)
{
	monthReport* months = NULL;
	int months_count = 0;
	monthReport* new_month = NULL;
	//
	unsigned long long start_date, final_date;
	//
	ReportGetPeriodFromArgs(app_args, &start_date, &final_date);
	if (!start_date)
		start_date = read_file_results.min_datetime;
	if (!final_date)
		final_date = read_file_results.max_datetime;
	DBG printf("start_date=%llu final_date=%llu\n", start_date, final_date);	
	//
	int d_year, d_month, d_t;
	int current_year = 0;
	int current_month = 0;
	int item_index = 0;
	for (int i = 0; i < data_size; i++)
	{
		if ((data[i].encoded_datetime < start_date) ||
			(data[i].encoded_datetime > final_date))
			continue;
		//
		d_year = data[i].year;
		d_month = data[i].month;
		d_t = data[i].t;
		//
		if ((current_year != d_year) || 
			(current_month != d_month))
		{			
			if ((item_index = MonthReportIndex(months_count, months, 
					d_year, d_month)) < 0)
			{
				months_count++;
				months = (monthReport*)realloc(months, months_count * sizeof(monthReport));
				if (months == NULL)
				{	
					printf("Memory not allocated.\n");
					return;
				}  
				new_month = months + months_count - 1;
				MonthInit(&new_month, d_year, d_month, d_t);
			} else {
				new_month = months + item_index;
			}	
			//
			current_year = d_year;
			current_month = d_month;		
		}
		//
		MonthProcessNewData(&new_month, d_t);
	}
	//
	MonthOrderByDate(months_count, months);
	//
	DBG MonthsPrint(months_count, months);
	//
	PrintCharString(DELIMETER_WIDTH, '=', 1);
	printf("\tReport period: ");
	ReportPrintTitle(app_args, start_date, final_date);
	PrintCharString(DELIMETER_WIDTH, '=', 1);
	//
	printf("%8s | %10s | %8s | %8s | %8s | %10s |\n",
		"Year", "Month", "Amount", "Min", "Max", "Avg");
	PrintCharString(DELIMETER_WIDTH, '-', 1);
	
	int count = 0, sum_t = 0, min_t = 0, max_t = 0;	
	for (int i = 0; i < months_count; i++)
	{
		months[i].avg_t = (float)months[i].sum_t / months[i].count;
		//
		count += months[i].count;
		sum_t += months[i].sum_t;
		if (!i) 
		{
			min_t = months[i].min_t;
			max_t = months[i].max_t;
		}
		if (min_t > months[i].min_t)
			min_t = months[i].min_t;
		if (max_t < months[i].max_t)
			max_t = months[i].max_t;		 	
		//	
		PrintReportLine(
			months[i].year,
			months[i].month,
			months[i].count,
			months[i].min_t,
			months[i].max_t,
			months[i].avg_t
		);
	}	
	PrintCharString(DELIMETER_WIDTH, '-', 1);
	printf("%21s | %8d | %8d | %8d | %10.3f |\n",
		"Total for period", count, min_t, max_t, (float)sum_t / count);	
	PrintCharString(DELIMETER_WIDTH, '-', 1);	
	//
	if (months != NULL) 
	{		
		free(months);
		DBG printf("Months report array is released.\n");
	}	
}
