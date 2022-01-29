#include <stdio.h>
#include <stdint.h>

#include <unistd.h>
#include <stdlib.h>

#include <malloc.h>

#include "temp_api.h"

#define CSV_POSITIONS_COUNT 6
#define CSV_LINE_WIDTH 32

#define PBWIDTH 20

char PBSTR[PBWIDTH];

const char *months[] = 
	{"January", "February", "March", "April", 
	"May", "June", "July", "August", 
	"September", "October", "November", "December"};
		
char file_name[255];
int year_no = 0;
char month_no = 0;

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

int ReadFile(char file_name[], int* size, sensor** data)
{
	const char status_format[] = "\r%d records loaded, %d errors found";
	const char show_status = 1;
	const char show_bar = 1;
	const int status_step = 10000;	
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
	int y, m, d, hh, mm, t;
	int r, line_count = 0, error_count = 0;
	char s[CSV_LINE_WIDTH];
	//
	printf("Loading file \"%s\"...\n", file_name);
	//
	while ((r = fscanf(fp, "%d;%d;%d;%d;%d;%d", 
						&y, &m, &d, &hh, &mm, &t)) > 0) 
	{	
		line_count++;
		if (r < CSV_POSITIONS_COUNT)
		{
			r = fscanf(fp, "%[^\n]", s);
			error_count++;
			//printf("Line %d ERROR! \"%s\" \n", line_count, s);			
		} else {
			SensorsAddRecord(data, (*size)++, y, m, d, hh, mm, t);  
			//printf("%d=%d;%d;%d;%d;%d;%d\n", r, y, m, d, hh, mm, t);		
		}	
		//
		if (show_status && (line_count % status_step == 0)) 	
		{	
			(show_bar)
				? PrintProgressBar((float)line_count / file_lines_count)
				: printf(status_format, line_count, error_count);
		}		
	}
	if (show_status && show_bar)
	{
		PrintProgressBar(1.0);
		printf("\n");
	}	
	printf(status_format, line_count, error_count);
	printf("\n");
	//
	fclose(fp);
	//
	return 1;
}

void PrintHelp(char app_name[])
{
	printf("This is example of list directory.\n");
	printf("Usage: clear [options]\n");
	printf("  -h This help text\n"); 
	printf("  -f Specify folder. Example: %s -f data.csv\n", app_name);
	printf("  -y Specify year. Example: %s -y 2020\n", app_name);				
	printf("  -m Specify month number. Example: %s -m 5\n", app_name);
}

int ProcessArguments(int argc, char *argv[])
{
	int res = 0;
	while ((res = getopt(argc, argv, "hf:y:m:")) != -1) 
	{
		switch (res) {
			case 'h':
				PrintHelp(argv[0]);
                return 0;
			case 'f':
				strcpy(file_name, optarg);
				printf("File is \"%s\".\n", file_name); 
				break;
			case 'y':
				year_no = atoi(optarg);
				printf("Year is %d.\n", year_no); 				
				break;	
			case 'm':
				month_no = atoi(optarg);
				printf("Month is %s.\n", months[month_no - 1]); 
				break;	
			case '?': 
				printf("Unknown argument: %s ", argv[optind - 1]);
				printf("Try -h for help\n");
                return 0;
		}
	}
	return 1;
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
	//
	/*
	printf("-----\n");
		for (int i = 0; i <= number; i++)
		printf("%04d-%02d-%02d %02d:%02d %llu t=%3d\n",
			(*info + i)->year,
			(*info + i)->month,
			(*info + i)->day,
			(*info + i)->hour,
			(*info + i)->minute,
			(*info + i)->encoded_datetime,
			(*info + i)->t
		);
	printf("-----\n");
	*/
}

void SensorsPrint(sensor* info, int number)
{
	printf("===================================\n");
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

void ReportGetHeader(uint16_t year, uint8_t month, uint8_t day, 
	uint8_t hour, uint8_t minute)
{
	//
}	

void ReportGetValues(sensor* info, uint64_t period_start, uint64_t period_final)
{
	//
}
