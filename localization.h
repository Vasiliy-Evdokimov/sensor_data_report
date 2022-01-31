#define MSG_LENGTH 128

#define ENG 0
#define RUS 1

#define TEST_MSG 0

#define FILE_IS 	1
#define YEAR_IS 	2
#define MONTH_IS 	3
#define YEAR2_IS 	4
#define MONTH2_IS 	5
#define NOT_CHOSEN	6

#define APP_TITLE	10

#define MONTH_JAN 11
#define MONTH_FEB 12
#define MONTH_MAR 13
#define MONTH_APR 14
#define MONTH_MAY 15
#define MONTH_JUN 16
#define MONTH_JUL 17
#define MONTH_AUG 18
#define MONTH_SEP 19
#define MONTH_OCT 20
#define MONTH_NOV 21
#define MONTH_DEC 22

typedef struct {
	char LNG_ID;
	char MSG_ID;
	char MSG[MSG_LENGTH];
} lc_record;

extern int LOCALE_ID;

void InitLC(int a_lc_id);

void FinalizeLC();

const char* GetLC(char msg_id);
