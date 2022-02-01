#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>

#include "localization.h"
#include "utils.h"
#include "functions.h"

#define CSV_POSITIONS_COUNT 6
#define CSV_LINE_WIDTH 64
#define CSV_LINE_SIZE CSV_LINE_WIDTH * sizeof(char)

#define DELIMETER_WIDTH 70
#define PBWIDTH 63

const char error_log_file[] = "error.log";

char PBSTR[PBWIDTH];

// APPLICATION ARGUMENTS

int ProcessArguments(int argc, char *argv[], arguments* args)
{	
	strcpy(args->app_name, argv[0]);
	//
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
				printf("%s: %s ", GetLC(UNKNOWN_ARGUMENT), argv[optind - 1]);
				printf("%s\n", GetLC(TRY_HELP));
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

// READ FILE

int CountFileLines(char file_name[])
{
    int count = 0;
    char line[CSV_LINE_WIDTH];
	FILE *fp;
	//
	fp = fopen(file_name, "r");	
	if (fp == NULL)
		return count;
	//	
    while(fgets(line, CSV_LINE_WIDTH, fp) != NULL)
		count++;
    fclose(fp);
    //
	return count;
}

void WriteErrorsFile(const char file_name[], int errors_count, char* errors_array)
{
	FILE *fp;
	if ((fp = fopen(file_name, "w")) == NULL)
	{
		printf("%s \"%s\"!\n", GetLC(PROBLEMS_OPEN_FILE), file_name);
		return;
	}
	//
	for (int i = 0; i < errors_count; i++)
		fprintf(fp, "%s\n", errors_array + i * CSV_LINE_SIZE);
	//
	fclose(fp);
}

int ReadFile(char file_name[], int* size, sensor** data, readFileResults* rfr)
{
	const char show_status = 1;
	const char show_bar = 1;
	const int status_step = 10000;
	//
	char *errors = NULL;	
	//
	FILE *fp;
	//		
	if ((fp = fopen(file_name, "r")) == NULL)
	{
		printf("%s \"%s\"!\n", GetLC(PROBLEMS_OPEN_FILE), file_name);
		return 0;
	}	
	//
	int file_lines_count = 0;
	if (show_status && show_bar) 
	{
		InitProgressBar();
		file_lines_count = CountFileLines(file_name);
	}	
	//
	int y, m, d, hh, mm, t, r;
	char s[CSV_LINE_WIDTH];
	//
	printf("%s \"%s\"...\n", GetLC(LOADING_FILE), file_name);
	//
	int lines_count = 0, lines_error = 0;
	unsigned long long new_dt, min_dt = 0, max_dt = 0;
	while ((r = fscanf(fp, "%d;%d;%d;%d;%d;%d", 
						&y, &m, &d, &hh, &mm, &t)) > 0) 
	{	
		lines_count++;
		if (r < CSV_POSITIONS_COUNT)
		{
			r = fscanf(fp, "%[^\n]", s);
			lines_error++;
			//
			errors = (char*)realloc(errors, lines_error * CSV_LINE_SIZE);
			sprintf(errors + (lines_error - 1) * CSV_LINE_SIZE, 
				GetLC(LINE_ERROR), lines_count, s);
			s[0] = '\0';	
		} else {
			SensorsAddRecord(data, (*size)++, y, m, d, hh, mm, t); 
			new_dt = (*data + *size - 1)->encoded_datetime;
			//
			if (!min_dt) {
				min_dt = new_dt;
				max_dt = min_dt;
			} else {
				if (new_dt < min_dt)
					min_dt = new_dt;
				if (new_dt > max_dt)
					max_dt = new_dt;	
			}			
		}	
		//
		if (show_status && (lines_count % status_step == 0)) 	
		{	
			(show_bar)
				? PrintProgressBar((float)lines_count / file_lines_count)
				: printf(GetLC(RECORDS_PROCESSED), lines_count, lines_error);
		}		
	}
	if (show_status) 
	{
		if (show_bar) 
		{
			PrintProgressBar(1.0);
		} else {
			printf(GetLC(RECORDS_PROCESSED), lines_count, lines_error);
		}				
	}
	printf("\n");
	//
	printf("%s\n", GetLC(LOADING_COMPLETED));
	//
	if ((lines_error > 0) && (errors != NULL))
		WriteErrorsFile(error_log_file, lines_error, errors);
	//
	if (errors != NULL) 
	{		
		free(errors);
		DBG printf("Errors array is released.\n");
	}	
	//
	strcpy(rfr->file_name, file_name);
	rfr->lines_processed = lines_count;
	rfr->lines_approved = lines_count - lines_error;
	rfr->lines_rejected = lines_error;
	rfr->min_datetime = min_dt;
	rfr->max_datetime = max_dt;
	//
	fclose(fp);
	//
	return 1;
}

void PrintReadFileResults(readFileResults* rfr)
{
	char day1, month1, hour1, minute1,
		day2, month2, hour2, minute2;
	short year1, year2;
	//
	DecodeDateTime(rfr->min_datetime, &year1, &month1, &day1, 
		&hour1, &minute1); 
	DecodeDateTime(rfr->max_datetime, &year2, &month2, &day2, 
		&hour2, &minute2);
	//
	PrintCharString(DELIMETER_WIDTH, '-', 1);
	printf(GetLC(READ_FILE_RESULTS_1), rfr->file_name);
	printf(GetLC(READ_FILE_RESULTS_2),
		rfr->lines_processed, rfr->lines_approved, rfr->lines_rejected);
	if (rfr->lines_rejected)
			printf(GetLC(SEE_ERROR_FILE), error_log_file);
	printf("%s %02d.%02d.%04d %02d:%02d\n", GetLC(DATA_START_TIME),
			day1, month1, year1, hour1, minute1);
	printf("%s %02d.%02d.%04d %02d:%02d\n", GetLC(DATA_FINAL_TIME),	
			day2, month2, year2, hour2, minute2);			
	PrintCharString(DELIMETER_WIDTH, '-', 1);		
}

// SENSORS

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
		DBG printf("Memory not allocated.\n");
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

// REPORT

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

void PrintReportLine(short year, char month, int count,
	signed char min_t, signed char max_t, float avg_t)
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

void ReportPrintTitle(arguments app_args, unsigned long long start_date, 
	unsigned long long final_date)
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
					DBG printf("Memory not allocated.\n");
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
	printf("\t%s: ", GetLC(REP_PERIOD));
	ReportPrintTitle(app_args, start_date, final_date);
	PrintCharString(DELIMETER_WIDTH, '=', 1);
	//
	printf("%8s | %10s | %8s | %8s | %8s | %10s |\n",
		GetLC(REP_YEAR), GetLC(REP_MONTH), GetLC(REP_AMOUNT), 
		GetLC(REP_MIN), GetLC(REP_MAX), GetLC(REP_AVG));
	PrintCharString(DELIMETER_WIDTH, '-', 1);
	//
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
		GetLC(REP_TOTAL), count, min_t, max_t, (float)sum_t / count);	
	PrintCharString(DELIMETER_WIDTH, '-', 1);	
	//
	if (months != NULL) 
	{		
		free(months);
		DBG printf("Months report array is released.\n");
	}	
}

// USER INTERFACE

int ShowMenu(arguments* args, readFileResults* rfr)
{
	char choice = '0';
	//
	while(1)
	{
		printf("%s:\n", GetLC(INTF_CHOOSE_ACTION));
		printf("1. %s\n", GetLC(INTF_REP_Y));
		printf("2. %s\n", GetLC(INTF_REP_MY));
		printf("3. %s\n", GetLC(INTF_REP_PERIOD));
		printf("4. %s\n", GetLC(SHOW_FILE_INFO));	
		printf("5. %s\n", GetLC(SHOW_HELP));	
		printf("6. %s\n", GetLC(SWITCH_LOCALE));		
		printf("0. %s\n", GetLC(QUIT));
		//		
		int y, m;
		choice = getch();
		if ((int)choice != 13)
			printf("%s (%c)\n", GetLC(YOU_CHOSE), choice);	
		switch (choice) {
			case '1':				
				printf("%s: ", GetLC(REP_YEAR));
				scanf("%d", &y);
				args->year_no = y;  
				args->month_no = 0;
				args->year_no2 = 0;
				args->month_no2 = 0;	
				goto OUT;
			case '2':
				printf("%s: ", GetLC(MONTH_YEAR));
				scanf("%d.%d", &m, &y);
				args->year_no = y;  
				args->month_no = m;
				args->year_no2 = 0;
				args->month_no2 = 0;			
				goto OUT;
			case '3':
				printf("%s #1: ", GetLC(MONTH_YEAR));
				scanf("%d.%d", &m, &y);
				args->year_no = y;  
				args->month_no = m;
				printf("%s #2: ", GetLC(MONTH_YEAR));
				scanf("%d.%d", &m, &y);
				args->year_no2 = y;  
				args->month_no2 = m;
				goto OUT;
			case '4':
				PrintReadFileResults(rfr);
				continue;
			case '5':
				PrintHelp(args->app_name);
				continue;					
			case '6':
				args->locale_id = !args->locale_id;
				LOCALE_ID = args->locale_id; 
				continue;
			case '0':
			case 'q':
				return 0;	
		}
	}
OUT:		
	return choice - '0';
}	

void PrintAppTitle(const char title[])
{
	int len = strlen(title);
	int mid = (DELIMETER_WIDTH / 2) - (len / 2) - 1;
	PrintCharString(mid, '=', 0);
	printf(" %s ", title);
	len += mid + 2; 
	while (len < DELIMETER_WIDTH)
	{	
		printf("%c", '=');
		len++;
	}	
	printf("\n");
}

void InitProgressBar()
{
	for(int i = 0; i < PBWIDTH; i++)
		PBSTR[i] = '>';
}

void PrintProgressBar(double percentage) 
{
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;    
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);    
}

void PrintHelp(char app_name[])
{
	printf("%s\n", GetLC(HELP_1));
	printf(GetLC(HELP_2), app_name);
	printf("%s:\n", GetLC(HELP_3)); 
	printf("%s\n", GetLC(HELP_4)); 
	printf("%s\n", GetLC(HELP_5));
	printf(GetLC(HELP_6), app_name);
	printf("%s\n", GetLC(HELP_7));
	printf(GetLC(HELP_8), app_name);
	printf("%s\n", GetLC(HELP_9));
	printf(GetLC(HELP_10), app_name);
	printf("%s\n", GetLC(HELP_11));
	printf(GetLC(HELP_12), app_name);
	printf("%s\n", GetLC(HELP_13));
	printf(GetLC(HELP_14), app_name);
	printf("%s\n", GetLC(HELP_15));
	printf(GetLC(HELP_16), app_name);	
}
