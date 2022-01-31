#include <stdio.h>
#include <malloc.h>
#include "temp_api.h"
#include "localization.c"

int main(int argc, char *argv[])
{
	int data_size = 0;
	sensor* info = NULL;
	readFileResults read_file_results;
	arguments app_args;
	//
	//
	DBG PrintTime();
	//	
	if (!ProcessArguments(argc, argv, &app_args))
		return 1;	
	//
	InitLC(app_args.locale_id);
	DBG printf(GetLC(TEST_MSG));
	//	
	DBG PrintArguments(&app_args);	
	if (!ReadFile(app_args.file_name, &data_size, &info, &read_file_results))
		return 1;
	PrintReadFileResults(&read_file_results);		
	//
	//SensorsPrint(info, data_size);	
	//
	ReportGetValues(data_size, info, app_args, read_file_results);
	//
	if (info != NULL) 
	{		
		free(info);
		DBG printf("Structures array is released.\n");
	}	
	//
	FinalizeLC();
	//
	DBG PrintTime();
	//
	getchar();	
	//
	return 0;
}
