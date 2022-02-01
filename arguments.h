#ifndef ARGUMENTS_H
#define ARGUMENTS_H

typedef struct {
	char file_name[255];
	int year_no;
	char month_no;
	int year_no2;
	char month_no2;
	char locale_id;
} arguments;

int ProcessArguments(int argc, char *argv[], arguments* args);

void PrintArguments(arguments* args);

#endif
