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
	char* result = "";
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
}

void FinalizeLC()
{
	if (lc_array != NULL) 
	{		
		free(lc_array);
		//printf("Localization array is released.\n");
	}	
}
