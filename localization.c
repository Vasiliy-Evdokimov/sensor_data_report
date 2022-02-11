#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>

#include "localization.h"

lc_record* lc_array = NULL;
int lc_record_count = 0;

int LOCALE_ID = 0;
char APP_NAME[255];

void str_replace(char *string, char *find, char *replace, char *result) 
{
    int string_len = strlen(string);
    int find_len = strlen(find);
    int replace_len = strlen(replace);
    char buf[find_len + 1];    
    int i = 0, res_pos = 0;
    while (i < string_len)
    {	
		strncpy(buf, &string[i], find_len);
		buf[find_len] = '\0';
		// 			
		if (!strcmp(find, buf)) {
			strcpy(&result[res_pos], replace);
			res_pos += replace_len;
			i += find_len; 			
		} else
			result[res_pos++] = string[i++];
		result[res_pos] = '\0';						
	}
}

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

void InitLC(int a_lc_id, char a_app_name[])
{
	setlocale(0, "");
	//
	LOCALE_ID = a_lc_id;
	strcpy(APP_NAME, a_app_name); 
	//
	char buf[MSG_LENGTH];
	//
	AddLC(ENG, TEST_MSG, "ENG ENG ENG\n");
	AddLC(RUS, TEST_MSG, "РУС РУС РУС\n");
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
	str_replace(
		"This application reads specified csv-file containing temperature sensor data and forms statistics report.\n"	
		"Usage: %app_name% -f <file_name> [options]\n"	
		"Options:\n"	
		"    -h This help text\n"
		"    -f Specify corresponding csv-file. Option is required.\n"	
		"       Example: %app_name% -f data.csv\n"	
		"    -y Specify year of report or start year of report period.\n"	
		"       Example: %app_name% -f data.csv -y 2020\n"	
		"    -m Specify month number. The year option is required.\n"	
		"       Example: %app_name% -f data.csv -y 2020 -m 5\n"	
		"    -a Specify final year of report period. Start year option is required.\n"	
		"       Example: %app_name% -f data.csv -y 2020 -a 2021\n"	
		"    -b Specify final month of report period. The start and final year options are required.\n"	
		"       Example: %app_name% -f data.csv -y 2020 -m 5 -a 2021 -b 3\n"	
		"    -L Specify localization. 0 for ENG (default), 1 for RUS.\n"	
		"       Example: %app_name% -f data.csv -y 2020 -m 5 -L 1\n",
		"%app_name%", APP_NAME, buf
	);
	AddLC(ENG, HELP, buf);  	
	//
	//
	//
	AddLC(RUS, APP_TITLE, "..:: ОТЧЕТ ПО ДАТЧИКУ ТЕМПЕРАТУРЫ ::..");
	//
	AddLC(RUS, FILE_IS, "Файл \"%s\".\n");
	AddLC(RUS, YEAR_IS, "Год %d.\n");
	AddLC(RUS, MONTH_IS, "Месяц %s.\n");
	AddLC(RUS, YEAR2_IS, "Год2 %d.\n");
	AddLC(RUS, MONTH2_IS, "Месяц2 %s.\n");
	AddLC(RUS, NOT_CHOSEN, "(не выбран)");
	//
	AddLC(RUS, MONTH_JAN, "Январь");
	AddLC(RUS, MONTH_FEB, "Февраль");
	AddLC(RUS, MONTH_MAR, "Март");
	AddLC(RUS, MONTH_APR, "Апрель");
	AddLC(RUS, MONTH_MAY, "Май");
	AddLC(RUS, MONTH_JUN, "Июнь");
	AddLC(RUS, MONTH_JUL, "Июль");
	AddLC(RUS, MONTH_AUG, "Август");
	AddLC(RUS, MONTH_SEP, "Сентябрь");
	AddLC(RUS, MONTH_OCT, "Октябрь");
	AddLC(RUS, MONTH_NOV, "Ноябрь");
	AddLC(RUS, MONTH_DEC, "Декабрь");	
	//
	AddLC(RUS, UNKNOWN_ARGUMENT, "Неизвестный параметр");	
	AddLC(RUS, TRY_HELP, "Ключ -h для вызова справки");	
	AddLC(RUS, PROBLEMS_OPEN_FILE, "Возникли ошибки при открытии файла");	
	AddLC(RUS, RECORDS_PROCESSED, "\r%d записей обработано, %d ошибок найдено");	
	AddLC(RUS, LINE_ERROR, "Строка %d ОШИБКА! \"%s\"");	
	AddLC(RUS, LOADING_FILE, "Загрузка файла");	
	AddLC(RUS, LOADING_COMPLETED, "Загрузка файла завершена!");	
	//
	AddLC(RUS, READ_FILE_RESULTS_1, "Файл \"%s\" был успешно загружен!\n");	
	AddLC(RUS, READ_FILE_RESULTS_2, "%d строк обработано, %d одобрено, %d отклонено с ошибками\n");	
	AddLC(RUS, SEE_ERROR_FILE, "(файл \"%s\" содержит описания ошибок)\n");	
	AddLC(RUS, DATA_START_TIME, "Дата начала данных");	
	AddLC(RUS, DATA_FINAL_TIME, "Дата окончания данных");	
	//
	AddLC(RUS, REP_PERIOD, "Отчетный период");	
	AddLC(RUS, REP_YEAR, "Год");	
	AddLC(RUS, REP_MONTH, "Месяц");	
	AddLC(RUS, REP_AMOUNT, "Кол-во");	
	AddLC(RUS, REP_MIN, "Мин");	
	AddLC(RUS, REP_MAX, "Макс");	
	AddLC(RUS, REP_AVG, "Средн");	
	AddLC(RUS, REP_TOTAL, "Итого за период");	
	//
	AddLC(RUS, INTF_CHOOSE_ACTION, "Пожалуйста, выберите действие");	
	AddLC(RUS, INTF_REP_Y, "Отчет за год");	
	AddLC(RUS, INTF_REP_MY, "Отчет за месяц.год");	
	AddLC(RUS, INTF_REP_PERIOD, "Отчет за период месяц.год - месяц.год");	
	AddLC(RUS, SHOW_FILE_INFO, "Показать информацию о файле");	
	AddLC(RUS, SHOW_HELP, "Показать справку о программе");	
	AddLC(RUS, SWITCH_LOCALE, "Переключить язык интерфейса");	
	AddLC(RUS, QUIT, "Выход");	
	//
	AddLC(RUS, YOU_CHOSE, "Вы выбрали");	
	//
	AddLC(RUS, MONTH_YEAR, "Месяц.Год");	
	//
	str_replace(
		"Эта программа считывает из указанного csv-файла информацию о показаниях датчика температуры и формирует статистический отчет.\n"	
		"Использование: %app_name% -f <имя_файла> [параметры]\n"	
		"Параметры:\n"
		"    -h Вызов этой справки\n"	
		"    -f Укажите csv-файл для обработки. Обязательный параметр.\n"	
		"       Пример: %app_name% -f data.csv\n"	
		"    -y Укажите год отчета или год начала отчетного периода.\n"	
		"       Пример: %app_name% -f data.csv -y 2020\n"	
		"    -m Укажите номер месяца. Параметр Год должен быть обязательо указан.\n"	
		"       Пример: %app_name% -f data.csv -y 2020 -m 5\n"	
		"    -a Укажите конечный год отчетного периода. Год начала периода должен быть обязательно указан.\n"	
		"       Пример: %app_name% -f data.csv -y 2020 -a 2021\n"	
		"    -b Укажите конечный месяц отчетного периода. Годы начала и окончания периода должны быть обязательо указаны.\n"	
		"       Пример: %app_name% -f data.csv -y 2020 -m 5 -a 2021 -b 3\n"	
		"    -L Выбор локализации. 0 для ENG (default), 1 для RUS.\n"	
		"       Пример: %app_name% -f data.csv -y 2020 -m 5 -L 1\n",
		"%app_name%", APP_NAME, buf
	);
	AddLC(RUS, HELP, buf);  
}

void FinalizeLC()
{
	if (lc_array != NULL) 
		free(lc_array);
}
