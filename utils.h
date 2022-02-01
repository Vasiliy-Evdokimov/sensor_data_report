#ifndef UTILS_H
#define UTILS_H

extern const int months_days[];
	
const char* GetMonthName(char month_no);

int GetMonthDays(int year, int month);

void PrintTime();

void PrintCharString(int count, char fill_char, char next_line);

unsigned long long EncodeDateTime(short year, char month, char day, 
	char hour, char minute);
	
void DecodeDateTime(unsigned long long DateTime, short* year, char* month, 
	char* day, char* hour, char* minute);

#endif
