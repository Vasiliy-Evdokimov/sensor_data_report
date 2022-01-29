#include <stdio.h>
#include <locale.h>
#include <malloc.h>
#include <time.h>
#include "temp_api.h"

extern char file_name[255];
extern int year_no;
extern char month_no;

void AddInfo(int* size, sensor** info)
{
	SensorsAddRecord(info, (*size)++, 2021, 9, 16, 12, 11, 9);
	SensorsAddRecord(info, (*size)++, 2022, 9, 2, 12, 12, -9);
	SensorsAddRecord(info, (*size)++, 2021, 1, 7, 12, 13, 8);
	SensorsAddRecord(info, (*size)++, 2021, 9, 5, 12, 14, 1);
}

int main(int argc, char *argv[])
{
	int data_size = 0;
	sensor* info = NULL;
	//
	time_t timer;
	struct tm *ptr;
	//
	timer = time(NULL);
	ptr = localtime(&timer);
	printf("Started at %02d:%02d:%02d\n", 
		ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
	//
	setlocale(LC_ALL, "Russian");
	//	
	if (!ProcessArguments(argc, argv))
		return 1;	
	if (!ReadFile(file_name, &data_size, &info))
		return 1;
	printf("data_size = %d\n", data_size);
	//
	//AddInfo(&data_size, &info);
	//printf("data_size = %d\n", data_size);
	//SensorsPrint(info, data_size);
	/*
	//	
	printf("\nSort by t\n");
	SensorsOrderByT(info, data_size);
	SensorsPrint(info, data_size);
	//
	printf("\nSort by date\n");
	SensorsOrderByDate(info, data_size);
	SensorsPrint(info, data_size);
	*/
	//
	if (info != NULL) 
	{
		printf("Structures are relesed.\n");
		free(info);
	}	
	//
	timer = time(NULL);
	ptr = localtime(&timer);
	printf("Finished at %02d:%02d:%02d\n", 
		ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
	//
	getchar();	
	//
	return 0;
}
