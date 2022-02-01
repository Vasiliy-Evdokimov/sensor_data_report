#include <stdio.h>
#include <malloc.h>

#include "structs.h"
#include "arguments.h"
#include "localization.h"
#include "utils.h"
#include "interface.h"
#include "sensors.h"

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
