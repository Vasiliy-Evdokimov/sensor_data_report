#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <malloc.h>
#include "temp_api.h"

int main(int argc, char *argv[])
{
	int data_size = 0;
	sensor* info = NULL;
	readFileResults read_file_results;
	arguments app_args;
	//
	time_t timer;
	struct tm *ptr;
	//
	timer = time(NULL);
	ptr = localtime(&timer);
	DBG printf("Started at %02d:%02d:%02d\n", 
		ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
	//
	setlocale(LC_ALL, "Russian");
	//	
	if (!ProcessArguments(argc, argv, &app_args))
		return 1;	
	PrintArguments(&app_args);	
	if (!ReadFile(app_args.file_name, &data_size, &info, &read_file_results))
		return 1;
	PrintReadFileResults(&read_file_results);		
	//
	//SensorsPrint(info, data_size);
	//
	if (info != NULL) 
	{		
		free(info);
		DBG printf("Structures array is released.\n");
	}	
	//
	timer = time(NULL);
	ptr = localtime(&timer);
	DBG printf("Finished at %02d:%02d:%02d\n", 
		ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
	//
	getchar();	
	//
	return 0;
}
