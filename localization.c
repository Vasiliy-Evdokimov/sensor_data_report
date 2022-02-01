#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>

#include "localization.h"

lc_record* lc_array = NULL;
int lc_record_count = 0;

int LOCALE_ID = 0;

void AddLC(char lng_id, char msg_id, char msg[MSG_LENGTH])
{
	lc_record_count++;
	lc_array = (lc_record*)realloc(lc_array, lc_record_count * sizeof(lc_record));
	if (lc_array == NULL)
	{	
		printf("Memory not allocated.\n");
		return;
	}        
	//
	lc_record* new_value = lc_array + lc_record_count - 1;
	//
	new_value->LNG_ID = lng_id;
	new_value->MSG_ID = msg_id;
	strcpy(new_value->MSG, msg);
}

const char* GetLC(char msg_id)
{	
	char* result = "???";
	for (int i = 0; i < lc_record_count; i++)
	{
		if ((lc_array[i].LNG_ID == LOCALE_ID) && 
			(lc_array[i].MSG_ID == msg_id))
		{
			result = lc_array[i].MSG;			
			break; 
		}
	}		 
	return result;
}

void InitLC(int a_lc_id)
{
	setlocale(0, "");
	//
	LOCALE_ID = a_lc_id;
	//
	AddLC(ENG, TEST_MSG, "ENG ENG ENG\n");
	AddLC(RUS, TEST_MSG, "��� ��� ���\n");
	//
	AddLC(ENG, APP_TITLE, "..:: TEMPERATURE SENSOR REPORT ::..");
	//
	AddLC(ENG, FILE_IS, "File is \"%s\".\n");
	AddLC(ENG, YEAR_IS, "Year is %d.\n");
	AddLC(ENG, MONTH_IS, "Month is %s.\n");
	AddLC(ENG, YEAR2_IS, "Year2 is %d.\n");
	AddLC(ENG, MONTH2_IS, "Month2 is %s.\n");
	AddLC(ENG, NOT_CHOSEN, "(not chosen)");
	//
	AddLC(ENG, MONTH_JAN, "January");
	AddLC(ENG, MONTH_FEB, "February");
	AddLC(ENG, MONTH_MAR, "March");
	AddLC(ENG, MONTH_APR, "April");
	AddLC(ENG, MONTH_MAY, "May");
	AddLC(ENG, MONTH_JUN, "June");
	AddLC(ENG, MONTH_JUL, "July");
	AddLC(ENG, MONTH_AUG, "August");
	AddLC(ENG, MONTH_SEP, "September");
	AddLC(ENG, MONTH_OCT, "October");
	AddLC(ENG, MONTH_NOV, "November");
	AddLC(ENG, MONTH_DEC, "December");	
	//
	AddLC(ENG, UNKNOWN_ARGUMENT, "Unknown argument");	
	AddLC(ENG, TRY_HELP, "Try -h for help");	
	AddLC(ENG, PROBLEMS_OPEN_FILE, "Some problems occurred while opening file");	
	AddLC(ENG, RECORDS_PROCESSED, "\r%d records processed, %d errors found");	
	AddLC(ENG, LINE_ERROR, "Line %d ERROR! \"%s\"");	
	AddLC(ENG, LOADING_FILE, "Loading file");	
	AddLC(ENG, LOADING_COMPLETED, "File loading completed!");	
	//
	AddLC(ENG, READ_FILE_RESULTS_1, "File \"%s\" was successfully loaded!\n");	
	AddLC(ENG, READ_FILE_RESULTS_2, "%d lines processed, %d approved, %d rejected with errors\n");	
	AddLC(ENG, SEE_ERROR_FILE, "(see \"%s\" file for errors details)\n");	
	AddLC(ENG, DATA_START_TIME, "Data start time is");	
	AddLC(ENG, DATA_FINAL_TIME, "Data final time is");	
	//
	AddLC(ENG, REP_PERIOD, "Report period");	
	AddLC(ENG, REP_YEAR, "Year");	
	AddLC(ENG, REP_MONTH, "Month");	
	AddLC(ENG, REP_AMOUNT, "Amount");	
	AddLC(ENG, REP_MIN, "Min");	
	AddLC(ENG, REP_MAX, "Max");	
	AddLC(ENG, REP_AVG, "Avg");	
	AddLC(ENG, REP_TOTAL, "Total for period");	
	//
	AddLC(ENG, INTF_CHOOSE_ACTION, "Please choose the action");	
	AddLC(ENG, INTF_REP_Y, "Report for year");	
	AddLC(ENG, INTF_REP_MY, "Report for month.year");	
	AddLC(ENG, INTF_REP_PERIOD, "Report for period month.year - month.year");	
	AddLC(ENG, SHOW_FILE_INFO, "Show file info");	
	AddLC(ENG, SHOW_HELP, "Show help");
	AddLC(ENG, SWITCH_LOCALE, "Switch localization");	
	AddLC(ENG, QUIT, "Quit");	
	//
	AddLC(ENG, YOU_CHOSE, "You chose");	
	//
	AddLC(ENG, MONTH_YEAR, "Month.Year");	
	//
	AddLC(ENG, HELP_1,  "This application reads specified csv-file containing temperature sensor data and forms statistics report.");	
	AddLC(ENG, HELP_2,  "Usage: %s -f <file_name> [options]\n");	
	AddLC(ENG, HELP_3,  "Options");	
	AddLC(ENG, HELP_4,  "    -h This help text");	
	AddLC(ENG, HELP_5,  "    -f Specify corresponding csv-file. Option is required.");	
	AddLC(ENG, HELP_6,  "       Example: %s -f data.csv\n");	
	AddLC(ENG, HELP_7,  "    -y Specify year of report or start year of report period.");	
	AddLC(ENG, HELP_8,  "       Example: %s -f data.csv -y 2020\n");	
	AddLC(ENG, HELP_9,  "    -m Specify month number. The year option is required.");	
	AddLC(ENG, HELP_10, "       Example: %s -f data.csv -y 2020 -m 5\n");	
	AddLC(ENG, HELP_11, "    -a Specify final year of report period. Start year option is required.");	
	AddLC(ENG, HELP_12, "       Example: %s -f data.csv -y 2020 -a 2021\n");	
	AddLC(ENG, HELP_13, "    -b Specify final month of report period. The start and final year options are required.");	
	AddLC(ENG, HELP_14, "       Example: %s -f data.csv -y 2020 -m 5 -a 2021 -b 3\n");	
	AddLC(ENG, HELP_15, "    -L Specify localization. 0 for ENG (default), 1 for RUS.");	
	AddLC(ENG, HELP_16, "       Example: %s -f data.csv -y 2020 -m 5 -L 1\n");	
	//
	//
	//
	AddLC(RUS, APP_TITLE, "..:: ����� �� ������� ����������� ::..");
	//
	AddLC(RUS, FILE_IS, "���� \"%s\".\n");
	AddLC(RUS, YEAR_IS, "��� %d.\n");
	AddLC(RUS, MONTH_IS, "����� %s.\n");
	AddLC(RUS, YEAR2_IS, "���2 %d.\n");
	AddLC(RUS, MONTH2_IS, "�����2 %s.\n");
	AddLC(RUS, NOT_CHOSEN, "(�� ������)");
	//
	AddLC(RUS, MONTH_JAN, "������");
	AddLC(RUS, MONTH_FEB, "�������");
	AddLC(RUS, MONTH_MAR, "����");
	AddLC(RUS, MONTH_APR, "������");
	AddLC(RUS, MONTH_MAY, "���");
	AddLC(RUS, MONTH_JUN, "����");
	AddLC(RUS, MONTH_JUL, "����");
	AddLC(RUS, MONTH_AUG, "������");
	AddLC(RUS, MONTH_SEP, "��������");
	AddLC(RUS, MONTH_OCT, "�������");
	AddLC(RUS, MONTH_NOV, "������");
	AddLC(RUS, MONTH_DEC, "�������");	
	//
	AddLC(RUS, UNKNOWN_ARGUMENT, "����������� ��������");	
	AddLC(RUS, TRY_HELP, "���� -h ��� ������ �������");	
	AddLC(RUS, PROBLEMS_OPEN_FILE, "�������� ������ ��� �������� �����");	
	AddLC(RUS, RECORDS_PROCESSED, "\r%d ������� ����������, %d ������ �������");	
	AddLC(RUS, LINE_ERROR, "������ %d ������! \"%s\"");	
	AddLC(RUS, LOADING_FILE, "�������� �����");	
	AddLC(RUS, LOADING_COMPLETED, "�������� ����� ���������!");	
	//
	AddLC(RUS, READ_FILE_RESULTS_1, "���� \"%s\" ��� ������� ��������!\n");	
	AddLC(RUS, READ_FILE_RESULTS_2, "%d ����� ����������, %d ��������, %d ��������� � ��������\n");	
	AddLC(RUS, SEE_ERROR_FILE, "(���� \"%s\" �������� �������� ������)\n");	
	AddLC(RUS, DATA_START_TIME, "���� ������ ������");	
	AddLC(RUS, DATA_FINAL_TIME, "���� ��������� ������");	
	//
	AddLC(RUS, REP_PERIOD, "�������� ������");	
	AddLC(RUS, REP_YEAR, "���");	
	AddLC(RUS, REP_MONTH, "�����");	
	AddLC(RUS, REP_AMOUNT, "���-��");	
	AddLC(RUS, REP_MIN, "���");	
	AddLC(RUS, REP_MAX, "����");	
	AddLC(RUS, REP_AVG, "�����");	
	AddLC(RUS, REP_TOTAL, "����� �� ������");	
	//
	AddLC(RUS, INTF_CHOOSE_ACTION, "����������, �������� ��������");	
	AddLC(RUS, INTF_REP_Y, "����� �� ���");	
	AddLC(RUS, INTF_REP_MY, "����� �� �����.���");	
	AddLC(RUS, INTF_REP_PERIOD, "����� �� ������ �����.��� - �����.���");	
	AddLC(RUS, SHOW_FILE_INFO, "�������� ���������� � �����");	
	AddLC(RUS, SHOW_HELP, "�������� ������� � ���������");	
	AddLC(RUS, SWITCH_LOCALE, "����������� ���� ����������");	
	AddLC(RUS, QUIT, "�����");	
	//
	AddLC(RUS, YOU_CHOSE, "�� �������");	
	//
	AddLC(RUS, MONTH_YEAR, "�����.���");	
	//
	AddLC(RUS, HELP_1,  "��� ��������� ��������� �� ���������� csv-����� ���������� � ���������� ������� ����������� � ��������� �������������� �����.");	
	AddLC(RUS, HELP_2,  "�������������: %s -f <���_�����> [���������]\n");	
	AddLC(RUS, HELP_3,  "���������");	
	AddLC(RUS, HELP_4,  "    -h ����� ���� �������");	
	AddLC(RUS, HELP_5,  "    -f ������� csv-���� ��� ���������. ������������ ��������.");	
	AddLC(RUS, HELP_6,  "       ������: %s -f data.csv\n");	
	AddLC(RUS, HELP_7,  "    -y ������� ��� ������ ��� ��� ������ ��������� �������.");	
	AddLC(RUS, HELP_8,  "       ������: %s -f data.csv -y 2020\n");	
	AddLC(RUS, HELP_9,  "    -m ������� ����� ������. �������� ��� ������ ���� ���������� ������.");	
	AddLC(RUS, HELP_10, "       ������: %s -f data.csv -y 2020 -m 5\n");	
	AddLC(RUS, HELP_11, "    -a ������� �������� ��� ��������� �������. ��� ������ ������� ������ ���� ����������� ������.");	
	AddLC(RUS, HELP_12, "       ������: %s -f data.csv -y 2020 -a 2021\n");	
	AddLC(RUS, HELP_13, "    -b ������� �������� ����� ��������� �������. ���� ������ � ��������� ������� ������ ���� ���������� �������.");	
	AddLC(RUS, HELP_14, "       ������: %s -f data.csv -y 2020 -m 5 -a 2021 -b 3\n");	
	AddLC(RUS, HELP_15, "    -L ����� �����������. 0 ��� ENG (default), 1 ��� RUS.");	
	AddLC(RUS, HELP_16, "       ������: %s -f data.csv -y 2020 -m 5 -L 1\n");	
}

void FinalizeLC()
{
	if (lc_array != NULL) 
		free(lc_array);
}
