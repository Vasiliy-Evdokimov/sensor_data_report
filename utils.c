#include <stdio.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#include "temp_api.h"
#include "localization.h"
#include "utils.h"

const int months_days[] = 
	{31, 28, 31, 30, 
	31, 30, 31, 31,
	30, 31, 30, 31};

const char* GetMonthName(char month_no)
{
	switch (month_no) 
	{
		case 1:	 return GetLC(MONTH_JAN);
		case 2:	 return GetLC(MONTH_FEB);
		case 3:	 return GetLC(MONTH_MAR);
		case 4:	 return GetLC(MONTH_APR);
		case 5:	 return GetLC(MONTH_MAY);
		case 6:	 return GetLC(MONTH_JUN);
		case 7:	 return GetLC(MONTH_JUL);
		case 8:	 return GetLC(MONTH_AUG);
		case 9:	 return GetLC(MONTH_SEP);
		case 10: return GetLC(MONTH_OCT);
		case 11: return GetLC(MONTH_NOV);
		case 12: return GetLC(MONTH_DEC);
		default: return GetLC(NOT_CHOSEN);	
	}
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

unsigned int DateToInt(sensor* info)
{
	return info->year << 16 | info->month << 8 | info->day;
}
