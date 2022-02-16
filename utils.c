#include <stdio.h>
#include <math.h>
#include <time.h>

#include "localization.h"
#include "utils.h"

const int months_days[] = 
	{31, 28, 31, 30, 
	31, 30, 31, 31,
	30, 31, 30, 31};

const char* GetMonthName(char month_no)
{
	if ((month_no >= 1) && (month_no <= 12))
		GetLC(MONTH_JAN + month_no - 1);
	else 
		return GetLC(NOT_CHOSEN);
}	

int GetMonthDays(int year, int month)
{
	if (month <= 0)
		return 0;
	if (month != 2)
		return months_days[month - 1];
	if (year % 4 == 0)
		return 29;
	else
		return 28;
}

void PrintTime()
{
	time_t timer;
	struct tm *ptr;
	//
	timer = time(NULL);
	ptr = localtime(&timer);
	printf("Now is %02d:%02d:%02d\n", 
		ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
}	

void PrintCharString(int count, char fill_char, char next_line)
{
	for (int i = 0; i < count; i++)
		printf("%c", fill_char);
	if (next_line)
		printf("\n");
}

unsigned long long EncodeDateTime(short year, char month, char day, 
	char hour, char minute)
{
	return (((year * 100ull + 
				month) * 100 + 
					day) * 100 + 
						hour) * 100 + 
							minute;
}	

void DecodeDateTime(unsigned long long DateTime, short* year, char* month, 
	char* day, char* hour, char* minute)
{
	*year = DateTime / (int)pow(10, 8); 
	*month = (DateTime % (int)pow(10, 8)) / (int)pow(10, 6);
	*day = (DateTime % (int)pow(10, 6)) / (int)pow(10, 4);
	*hour = (DateTime % (int)pow(10, 4)) / 100;
	*minute = DateTime % 100;
}
