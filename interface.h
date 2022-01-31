#define DELIMETER_WIDTH 70
#define PBWIDTH 63

int ShowMenu(arguments* args, readFileResults* rfr);

void PrintAppTitle(const char title[]);

void InitProgressBar();

void PrintProgressBar(double percentage) ;

void PrintHelp(char app_name[]);
