#include <stdint.h>

#define MSG_LENGTH 255

#define ENG 0
#define RUS 1

#define TEST_MSG 0

#define FILE_IS 1
#define YEAR_IS 2
#define MONTH_IS 3

typedef struct {
	uint8_t LNG_ID;
	uint8_t MSG_ID;
	char MSG[MSG_LENGTH];
} lc_record;

void InitLC(int a_lc_id);

void FinalizeLC();

const char* GetLC(uint8_t msg_id);
