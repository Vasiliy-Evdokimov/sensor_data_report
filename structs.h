#ifndef STRUCTS_H
#define STRUCTS_H

#define DEBUG

#ifdef DEBUG
	#define DBG if(1)
#else
	#define DBG if(0)
#endif

typedef struct {
	char day;
	char month;
	short year;
	char hour;
	char minute;
	unsigned long long encoded_datetime;
	signed char t;
} sensor;

typedef struct {
	char month;
	short year;	
	int encoded_my;
	int count;
	signed int sum_t;
	signed char min_t;
	signed char max_t;	
	float avg_t;
} monthReport;

#endif
