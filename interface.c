#include <stdio.h>
#include <unistd.h>
#include <conio.h>

#include "structs.h"
#include "arguments.h"
#include "localization.h"
#include "utils.h"
#include "sensors.h"
#include "read_file.h"
#include "interface.h"

char PBSTR[PBWIDTH];

int ShowMenu(arguments* args, readFileResults* rfr)
{
	char choice = '0';
	//
	while(1)
	{
		printf("Please choose the action:\n");
		printf("1. Report for year\n");
		printf("2. Report for month.year\n");
		printf("3. Report for period month.year - month.year\n");
		printf("4. Show file info\n");	
		printf("5. Switch localization\n");		
		printf("0. Quit\n");
		//		
		int y, m;
		choice = getch();
		if ((int)choice != 13)
			printf("You choose (%c)\n", choice);	
		switch (choice) {
			case '1':				
				printf("Year: ");
				scanf("%d", &y);
				args->year_no = y;  
				args->month_no = 0;
				args->year_no2 = 0;
				args->month_no2 = 0;	
				goto OUT;
			case '2':
				printf("Month.Year: ");
				scanf("%d.%d", &m, &y);
				args->year_no = y;  
				args->month_no = m;
				args->year_no2 = 0;
				args->month_no2 = 0;			
				goto OUT;
			case '3':
				printf("Month.Year #1: ");
				scanf("%d.%d", &m, &y);
				args->year_no = y;  
				args->month_no = m;
				printf("Month.Year #2: ");
				scanf("%d.%d", &m, &y);
				args->year_no2 = y;  
				args->month_no2 = m;
				goto OUT;
			case '4':
				PrintReadFileResults(rfr);
				continue;				
			case '5':
				args->locale_id = !args->locale_id;
				LOCALE_ID = args->locale_id; 
				continue;
			case '0':
			case 'q':
				return 0;	
		}
	}
OUT:		
	return choice - '0';
}	

void PrintAppTitle(const char title[])
{
	int len = strlen(title);
	int mid = (DELIMETER_WIDTH / 2) - (len / 2) - 1;
	PrintCharString(mid, '=', 0);
	printf(" %s ", title);
	len += mid + 2; 
	while (len < DELIMETER_WIDTH)
	{	
		printf("%c", '=');
		len++;
	}	
	printf("\n");
}

void InitProgressBar()
{
	for(int i = 0; i < PBWIDTH; i++)
		PBSTR[i] = '>';
}

void PrintProgressBar(double percentage) 
{
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;    
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);    
}

void PrintHelp(char app_name[])
{
	printf("This application reads specified csv-file containing temperature sensor data and forms statistics report.\n");
	printf("Usage: %s -f <file_name> [options]\n", app_name);
	printf("Options:\n"); 
	printf("    -h This help text\n"); 
	printf("    -f Specify corresponding csv-file. Option is required.\n");
	printf("       Example: %s -f data.csv\n", app_name);
	printf("    -y Specify year of report or start year of report period.\n");
	printf("       Example: %s -f data.csv -y 2020\n", app_name);
	printf("    -m Specify month number. The year option is required.\n");
	printf("       Example: %s -f data.csv -y 2020 -m 5\n", app_name);
	printf("    -a Specify final year of report period. Start year option is required.\n");
	printf("       Example: %s -f data.csv -y 2020 -a 2021\n", app_name);
	printf("    -b Specify final month of report period. The start and final year options are required.\n");
	printf("       Example: %s -f data.csv -y 2020 -m 5 -a 2021 -b 3\n", app_name);
	printf("    -L Specify localization. 0 for ENG (default), 1 for RUS.\n");
	printf("       Example: %s -f data.csv -y 2020 -m 5 -L 1\n", app_name);	
}
