#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#define MSG_LENGTH 2048

#define ENG 0
#define RUS 1

#define TEST_MSG 0

#define FILE_IS 			1
#define YEAR_IS 			2
#define MONTH_IS 			3
#define YEAR2_IS 			4
#define MONTH2_IS 			5
#define NOT_CHOSEN			6

#define SHOW_HELP			7

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

#define UNKNOWN_ARGUMENT 	23
#define TRY_HELP 			24
#define PROBLEMS_OPEN_FILE 	25
#define RECORDS_PROCESSED 	26
#define LINE_ERROR 			27
#define LOADING_FILE 		28
#define LOADING_COMPLETED	29

#define READ_FILE_RESULTS_1 30
#define READ_FILE_RESULTS_2 31
#define SEE_ERROR_FILE		32
#define DATA_START_TIME 	33
#define DATA_FINAL_TIME 	34

#define REP_PERIOD 			35
#define REP_YEAR 			36
#define REP_MONTH			37
#define REP_AMOUNT			38
#define REP_MIN				39
#define REP_MAX				40
#define REP_AVG				41
#define REP_TOTAL			42

#define INTF_CHOOSE_ACTION	43
#define INTF_REP_Y			44
#define INTF_REP_MY			45
#define INTF_REP_PERIOD		46
#define SHOW_FILE_INFO		47
#define SWITCH_LOCALE		48
#define QUIT				49

#define YOU_CHOSE			50

#define MONTH_YEAR			51

#define HELP				52

typedef struct {
	char LNG_ID;
	char MSG_ID;
	char MSG[MSG_LENGTH];
} lc_record;

extern int LOCALE_ID;

void InitLC(int a_lc_id, char a_app_name[]);

void FinalizeLC();

const char* GetLC(char msg_id);

#endif
