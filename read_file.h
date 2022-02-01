#ifndef READ_FILE_H
#define READ_FILE_H

typedef struct {
	char file_name[255];
	int lines_processed;
	int lines_approved;
	int lines_rejected;
	unsigned long long min_datetime;
	unsigned long long max_datetime;
} readFileResults;

int CountFileLines(char file_name[]);

void WriteErrorsFile(const char file_name[], int errors_count, char* errors_array);

int ReadFile(char file_name[], int* size, sensor** data, readFileResults* rfr);

void PrintReadFileResults(readFileResults* rfr);

#endif
