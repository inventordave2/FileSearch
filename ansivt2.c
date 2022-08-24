#include <string.h>
#include "ansivt2.h"

char resetAnsiVtCodes(char f)	{
	
	if(f == 0)	{

		strcpy((char *)FG_BLACK, "");
		strcpy((char *)FG_RED, "");
		strcpy((char *)FG_GREEN, "");
		strcpy((char *)FG_YELLOW, "");
		strcpy((char *)FG_BLUE, "");
		strcpy((char *)FG_MAGENTA, "");
		strcpy((char *)FG_CYAN, "");
		strcpy((char *)FG_WHITE, "");

		strcpy((char *)FG_BRIGHT_BLACK, "");
		strcpy((char *)FG_BRIGHT_RED, "");
		strcpy((char *)FG_BRIGHT_GREEN, "");
		strcpy((char *)FG_BRIGHT_YELLOW, "");
		strcpy((char *)FG_BRIGHT_BLUE, "");
		strcpy((char *)FG_BRIGHT_MAGENTA, "");
		strcpy((char *)FG_BRIGHT_CYAN, "");
		strcpy((char *)FG_BRIGHT_WHITE, "");

		strcpy((char *)BG_BLACK, "");
		strcpy((char *)BG_RED, "");
		strcpy((char *)BG_GREEN, "");
		strcpy((char *)BG_YELLOW, "");
		strcpy((char *)BG_BLUE, "");
		strcpy((char *)BG_MAGENTA, "");
		strcpy((char *)BG_CYAN, "");
		strcpy((char *)BG_WHITE, "");

		strcpy((char *)BG_BRIGHT_BLACK, "");
		strcpy((char *)BG_BRIGHT_RED, "");
		strcpy((char *)BG_BRIGHT_GREEN, "");
		strcpy((char *)BG_BRIGHT_YELLOW, "");
		strcpy((char *)BG_BRIGHT_BLUE, "");
		strcpy((char *)BG_BRIGHT_MAGENTA, "");
		strcpy((char *)BG_BRIGHT_CYAN, "");
		strcpy((char *)BG_BRIGHT_WHITE, "");

		strcpy((char *)NORMAL, "");
	}
	
	else if(f == 1)	{
		
		strcpy((char *)FG_BLACK, "[30m");
		strcpy((char *)FG_RED, "[31m");
		strcpy((char *)FG_GREEN, "[32m");
		strcpy((char *)FG_YELLOW, "[33m");
		strcpy((char *)FG_BLUE, "[34m");
		strcpy((char *)FG_MAGENTA, "[35m");
		strcpy((char *)FG_CYAN, "[36m");
		strcpy((char *)FG_WHITE, "[37m");

		strcpy((char *)FG_BRIGHT_BLACK, "[90m");
		strcpy((char *)FG_BRIGHT_RED, "[91m");
		strcpy((char *)FG_BRIGHT_GREEN, "[92m");
		strcpy((char *)FG_BRIGHT_YELLOW, "[93m");
		strcpy((char *)FG_BRIGHT_BLUE, "[94m");
		strcpy((char *)FG_BRIGHT_MAGENTA, "[95m");
		strcpy((char *)FG_BRIGHT_CYAN, "[96m");
		strcpy((char *)FG_BRIGHT_WHITE, "[97m");

		strcpy((char *)BG_BLACK, "[40m");
		strcpy((char *)BG_RED, "[41m");
		strcpy((char *)BG_GREEN, "[42m");
		strcpy((char *)BG_YELLOW, "[43m");
		strcpy((char *)BG_BLUE, "[44m");
		strcpy((char *)BG_MAGENTA, "[45m");
		strcpy((char *)BG_CYAN, "[46m");
		strcpy((char *)BG_WHITE, "[47m");

		strcpy((char *)BG_BRIGHT_BLACK, "[100m");
		strcpy((char *)BG_BRIGHT_RED, "[101m");
		strcpy((char *)BG_BRIGHT_GREEN, "[102m");
		strcpy((char *)BG_BRIGHT_YELLOW, "[103m");
		strcpy((char *)BG_BRIGHT_BLUE, "[104m");
		strcpy((char *)BG_BRIGHT_MAGENTA, "[105m");
		strcpy((char *)BG_BRIGHT_CYAN, "[106m");
		strcpy((char *)BG_BRIGHT_WHITE, "[107m");

		strcpy((char *)NORMAL, "[0m");
	}
	
	return (ansivt = f);
}
