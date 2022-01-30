#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#include "localization.h"

lc_record* lc_array = NULL;
int lc_record_count = 0;

static int LOCALE_ID = 0;

void AddLC(uint8_t lng_id, uint8_t msg_id, char msg[MSG_LENGTH])
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

const char* GetLC(uint8_t msg_id)
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
	AddLC(RUS, TEST_MSG, "ÐÓÑ ÐÓÑ ÐÓÑ\n");
	//
	AddLC(ENG, FILE_IS, "File is \"%s\".\n");
	AddLC(ENG, YEAR_IS, "Year is %d.\n");
	AddLC(ENG, MONTH_IS, "Month is %s.\n");
	//
	AddLC(RUS, FILE_IS, "Ôàéë \"%s\".\n");
	AddLC(RUS, YEAR_IS, "Ãîä %d.\n");
	AddLC(RUS, MONTH_IS, "Ìåñÿö is %s.\n");
}

void FinalizeLC()
{
	if (lc_array != NULL) 
	{		
		free(lc_array);
		printf("Localization array is released.\n");
	}	
}
