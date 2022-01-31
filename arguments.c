#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "temp_api.h"
#include "localization.h"
#include "utils.h"
#include "interface.h"
#include "arguments.h"

int ProcessArguments(int argc, char *argv[], arguments* args)
{	
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
				printf("Unknown argument: %s ", argv[optind - 1]);
				printf("Try -h for help\n");
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
