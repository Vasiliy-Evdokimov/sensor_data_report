#include <stdio.h>
#include <malloc.h>

#include "localization.h"
#include "utils.h"
#include "functions.h"

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
	InitLC(app_args.locale_id, app_args.app_name);
	DBG printf(GetLC(TEST_MSG));
	//
	PrintAppTitle(GetLC(APP_TITLE));
	//	
	DBG PrintArguments(&app_args);	
	if (!ReadFile(app_args.file_name, &data_size, &info, &read_file_results))
		return 1;
	PrintReadFileResults(&read_file_results);		
	//
	while (1)
	{
		ReportGetValues(data_size, info, app_args, read_file_results);
		//
		PrintAppTitle(GetLC(APP_TITLE));
		//
		if (!ShowMenu(&app_args, &read_file_results))
			break;						
	}
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
	return 0;
}
