#ifndef _BYPASS_ANSIVT
	#define _BYPASS_ANSIVT 0
#endif

#if _BYPASS_ANSIVT == 0

	#define FG_BLACK "[30m"
	#define FG_RED "[31m"
	#define FG_GREEN "[32m"
	#define FG_YELLOW "[33m"
	#define FG_BLUE "[34m"
	#define FG_MAGENTA "[35m"
	#define FG_CYAN "[36m"
	#define FG_WHITE "[37m"

	#define FG_BRIGHT_BLACK "[90m"
	#define FG_BRIGHT_RED "[91m"
	#define FG_BRIGHT_GREEN "[92m"
	#define FG_BRIGHT_YELLOW "[93m"
	#define FG_BRIGHT_BLUE "[94m"
	#define FG_BRIGHT_MAGENTA "[95m"
	#define FG_BRIGHT_CYAN "[96m"
	#define FG_BRIGHT_WHITE "[97m"

	#define BG_BLACK "[40m"
	#define BG_RED "[41m"
	#define BG_GREEN "[42m"
	#define BG_YELLOW "[43m"
	#define BG_BLUE "[44m"
	#define BG_MAGENTA "[45m"
	#define BG_CYAN "[46m"
	#define BG_WHITE "[47m"

	#define BG_BRIGHT_BLACK "[100m"
	#define BG_BRIGHT_RED "[101m"
	#define BG_BRIGHT_GREEN "[102m"
	#define BG_BRIGHT_YELLOW "[103m"
	#define BG_BRIGHT_BLUE "[104m"
	#define BG_BRIGHT_MAGENTA "[105m"
	#define BG_BRIGHT_CYAN "[106m"
	#define BG_BRIGHT_WHITE "[107m"

	#define NORMAL "[0m"


#else

	#define FG_BLACK ""
	#define FG_RED ""
	#define FG_GREEN ""
	#define FG_YELLOW ""
	#define FG_BLUE ""
	#define FG_MAGENTA ""
	#define FG_CYAN ""
	#define FG_WHITE ""

	#define FG_BRIGHT_BLACK ""
	#define FG_BRIGHT_RED ""
	#define FG_BRIGHT_GREEN ""
	#define FG_BRIGHT_YELLOW ""
	#define FG_BRIGHT_BLUE ""
	#define FG_BRIGHT_MAGENTA ""
	#define FG_BRIGHT_CYAN ""
	#define FG_BRIGHT_WHITE ""

	#define BG_BLACK ""
	#define BG_RED ""
	#define BG_GREEN ""
	#define BG_YELLOW ""
	#define BG_BLUE ""
	#define BG_MAGENTA ""
	#define BG_CYAN ""
	#define BG_WHITE ""

	#define BG_BRIGHT_BLACK ""
	#define BG_BRIGHT_RED ""
	#define BG_BRIGHT_GREEN ""
	#define BG_BRIGHT_YELLOW ""
	#define BG_BRIGHT_BLUE ""
	#define BG_BRIGHT_MAGENTA ""
	#define BG_BRIGHT_CYAN ""
	#define BG_BRIGHT_WHITE ""

	#define NORMAL ""


#endif
