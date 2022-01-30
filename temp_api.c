#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
#include <stdlib.h>

#include <malloc.h>

#include <math.h>

#include "temp_api.h"
#include "localization.h"

#define CSV_POSITIONS_COUNT 6
#define CSV_LINE_WIDTH 64
#define CSV_LINE_SIZE CSV_LINE_WIDTH * sizeof(char)

#define PBWIDTH 20

const char error_log_file[] = "error.log";

char PBSTR[PBWIDTH];

const char *months[] = 
	{"January", "February", "March", "April", 
	"May", "June", "July", "August", 
	"September", "October", "November", "December"};
	
const int months_days[] = 
	{31, 28, 31, 30, 
	31, 30, 31, 31,
	30, 31, 30, 31};

int GetMonthDays(int year, int month)
{
	if (month != 2)
		return months_days[month - 1];
	if (year % 4 == 0)
		return 29;
	else
		return 28;
}

void PrintCharString(int count, char fill_char)
{
	for (int i = 0; i < count; i++)
		printf("%c", fill_char);
	if (count > 0)
		printf("\n");
}

void InitProgressBar()
{
	for(int i = 0; i < PBWIDTH; i++)
		PBSTR[i] = '>';
}

void PrintProgressBar(double percentage) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;    
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);    
}

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
	fp = fopen(file_name, "w"); 
	if (fp == NULL)
	{
		printf("Some problems occurred while opening file \"%s\"!", file_name);
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
	const char status_format[] = "\r%d records processed, %d errors found";
	const char show_status = 1;
	const char show_bar = 1;
	const int status_step = 10000;
	//
	char *errors = NULL;	
	//
	FILE *fp;
	//
	fp = fopen(file_name, "r");	
	if (fp == NULL)
	{
		printf("Some problems occurred while opening file \"%s\"!", file_name);
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
	printf("Loading file \"%s\"...\n", file_name);
	//
	int lines_count = 0, lines_error = 0;
	uint64_t new_dt, min_dt = 0, max_dt = 0;
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
				"Line %d ERROR! \"%s\"", lines_count, s);
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
				: printf(status_format, lines_count, lines_error);
		}		
	}
	if (show_status) 
	{
		if (show_bar) 
		{
			PrintProgressBar(1.0);
		} else {
			printf(status_format, lines_count, lines_error);
		}				
	}
	printf("\n");
	//
	printf("File loading completed!\n");
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
	uint8_t day1, month1, hour1, minute1,
		day2, month2, hour2, minute2;
	uint16_t year1, year2;
	//
	DecodeDateTime(rfr->min_datetime, &year1, &month1, &day1, 
		&hour1, &minute1); 
	DecodeDateTime(rfr->max_datetime, &year2, &month2, &day2, 
		&hour2, &minute2);
	//
	PrintCharString(40, '-');
	printf("File \"%s\" was successfully loaded!\n", rfr->file_name);
	printf("%d lines processed, %d approved, %d rejected with errors\n",
		rfr->lines_processed, rfr->lines_approved, rfr->lines_rejected);
	if (rfr->lines_rejected)
			printf("(see \"%s\" file for errors details)\n", error_log_file);
	printf("Data start time is %02d.%02d.%04d %02d:%02d\n",
			day1, month1, year1, hour1, minute1);
	printf("Data final time is %02d.%02d.%04d %02d:%02d\n",
			day2, month2, year2, hour2, minute2);			
	PrintCharString(40, '-');		
}

void PrintHelp(char app_name[])
{
	printf("This is example of list directory.\n");
	printf("Usage: clear [options]\n");
	printf("  -h This help text\n"); 
	printf("  -f Specify folder. Example: %s -f data.csv\n", app_name);
	printf("  -m Specify month number. Example: %s -m 5\n", app_name);
	printf("  -y Specify year. Example: %s -y 2020\n", app_name);
}

int ProcessArguments(int argc, char *argv[], arguments* args)
{	
	args->locale_id = 0;
	//
	int res = 0;
	while ((res = getopt(argc, argv, "hf:y:m:l:")) != -1) 
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
			case 'l':
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
	PrintCharString(40, '-');
	printf(GetLC(FILE_IS), args->file_name);
	printf(GetLC(YEAR_IS), args->year_no); 
	printf(GetLC(MONTH_IS), months[args->month_no - 1]);	
	PrintCharString(40, '-');
}

uint64_t EncodeDateTime(uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute)
{
	return (((year * 100ull + 
				month) * 100 + 
					day) * 100 + 
						hour) * 100 + 
							minute;
}	

void DecodeDateTime(uint64_t DateTime, uint16_t* year, uint8_t* month, 
	uint8_t* day, uint8_t* hour, uint8_t* minute)
{
	*year = DateTime / (int)pow(10, 8); 
	*month = (DateTime % (int)pow(10, 8)) / (int)pow(10, 6);
	*day = (DateTime % (int)pow(10, 6)) / (int)pow(10, 4);
	*hour = (DateTime % (int)pow(10, 4)) / 100;
	*minute = DateTime % 100;
}		

unsigned int DateToInt(sensor* info)
{
	return info->year << 16 | info->month << 8 | info->day;
}

uint64_t SensorsEncodeDateTime(sensor* info)
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
			if (SensorsEncodeDateTime(info + i) >=
				SensorsEncodeDateTime(info + j))
			/*
			if ((info + i)->encoded_datetime >= 
				(info + j)->encoded_datetime)
			*/ 
				SensorsSwapByIndex(info, i, j);
}

void SensorsAddRecord(sensor** info, int number,
	uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute, int8_t t)
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
		//EncodeDateTime(year, month, day, hour, minute);
	new_value->t = t;
}

void SensorsPrint(sensor* info, int number)
{
	PrintCharString(40, '=');
	for (int i = 0; i < number; i++)
		printf("%04d-%02d-%02d %02d:%02d %llu t=%3d\n",
			info[i].year,
			info[i].month,
			info[i].day,
			info[i].hour,
			info[i].minute,
			info[i].encoded_datetime,
			info[i].t
		);
}

void GetHelpInfo()
{
	//
}

void ReportGetHeader(arguments* args)
{
	//
}	

void ReportGetValues(sensor* info, uint64_t period_start, uint64_t period_final)
{
	//
}
