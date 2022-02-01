#ifndef READ_FILE_H
#define READ_FILE_H

int CountFileLines(char file_name[]);

void WriteErrorsFile(const char file_name[], int errors_count, char* errors_array);

int ReadFile(char file_name[], int* size, sensor** data, readFileResults* rfr);

void PrintReadFileResults(readFileResults* rfr);

#endif
