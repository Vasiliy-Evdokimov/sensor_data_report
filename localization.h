#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#define MSG_LENGTH 128

#define ENG 0
#define RUS 1

#define TEST_MSG 0

#define FILE_IS 			1
#define YEAR_IS 			2
#define MONTH_IS 			3
#define YEAR2_IS 			4
#define MONTH2_IS 			5
#define NOT_CHOSEN			6

#define APP_TITLE			10

#define MONTH_JAN 			11
#define MONTH_FEB 			12
#define MONTH_MAR 			13
#define MONTH_APR 			14
#define MONTH_MAY 			15
#define MONTH_JUN 			16
#define MONTH_JUL 			17
#define MONTH_AUG 			18
#define MONTH_SEP 			19
#define MONTH_OCT 			20
#define MONTH_NOV 			21
#define MONTH_DEC 			22

#define UNKNOWN_ARGUMENT 	23 //Unknown argument
#define TRY_HELP 			24 //Try -h for help
#define PROBLEMS_OPEN_FILE 	25//Some problems occurred while opening file
#define RECORDS_PROCESSED 	26 //\r%d records processed, %d errors found
#define LINE_ERROR 			27//Line %d ERROR! \"%s\"
#define LOADING_FILE 		28//Loading file
#define LOADING_COMPLETED	29//File loading completed!

#define READ_FILE_RESULTS_1 30 //File \"%s\" was successfully loaded!\n
#define READ_FILE_RESULTS_2 31 //"%d lines processed, %d approved, %d rejected with errors\n"
#define SEE_ERROR_FILE		32 //"(see \"%s\" file for errors details)\n"
#define DATA_START_TIME 	33 //"Data start time is %02d.%02d.%04d %02d:%02d\n"
#define DATA_FINAL_TIME 	34 //"Data final time is %02d.%02d.%04d %02d:%02d\n"

#define REP_PERIOD 			35 //Report period
#define REP_YEAR 			36 //Year
#define REP_MONTH			37 //Month
#define REP_AMOUNT			38 //Amount
#define REP_MIN				39 //Min
#define REP_MAX				40 //Max
#define REP_AVG				41 //Avg
#define REP_TOTAL			42 //Total for period

#define INTF_CHOOSE_ACTION	43//Please choose the action
#define INTF_REP_Y			44//Report for year
#define INTF_REP_MY			45//Report for month.year
#define INTF_REP_PERIOD		46//Report for period month.year - month.year
#define SHOW_FILE_INFO		47//Show file info
#define SWITCH_LOCALE		48//Switch localization
#define QUIT				49//Quit

#define YOU_CHOSE			50//You chose

#define MONTH_YEAR			51//Month.Year 

#define HELP_1				52//This application reads specified csv-file containing temperature sensor data and forms statistics report.
#define HELP_2				53//"Usage: %s -f <file_name> [options]\n"
#define HELP_3				54//Options
#define HELP_4				55//"    -h This help text"
#define HELP_5				56//"    -f Specify corresponding csv-file. Option is required.
#define HELP_6				57//"       Example: %s -f data.csv\n
#define HELP_7				58//"    -y Specify year of report or start year of report period.
#define HELP_8				59//"       Example: %s -f data.csv -y 2020\n
#define HELP_9				60//"    -m Specify month number. The year option is required.
#define HELP_10				61//"       Example: %s -f data.csv -y 2020 -m 5\n
#define HELP_11				62//"    -a Specify final year of report period. Start year option is required.
#define HELP_12				63//"       Example: %s -f data.csv -y 2020 -a 2021\n
#define HELP_13				64//"    -b Specify final month of report period. The start and final year options are required.
#define HELP_14				65//"       Example: %s -f data.csv -y 2020 -m 5 -a 2021 -b 3\n"
#define HELP_15				66//"    -L Specify localization. 0 for ENG (default), 1 for RUS.
#define HELP_16				67//"       Example: %s -f data.csv -y 2020 -m 5 -L 1\n

typedef struct {
	char LNG_ID;
	char MSG_ID;
	char MSG[MSG_LENGTH];
} lc_record;

extern int LOCALE_ID;

void InitLC(int a_lc_id);

void FinalizeLC();

const char* GetLC(char msg_id);

#endif
