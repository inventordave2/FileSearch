#ifndef _DAVE_FILESEARCH_

	#define _DAVE_FILESEARCH_

	#include <windows.h>
	#include "regex/wregex.h"

	//#define MAX_NUM_FOLDERS 2000
	//#define MAX_NUM_FILES 2000
	#define MAX_ENTRIES 10000
	
	#define ENTRIES 25

	char* ignoreList;
	char* whiteList;
	#define IL_ENTRIES ENTRIES
	#define WL_ENTRIES ENTRIES

	char* defaultIgnoreList;
	char* defaultWhiteList;
	#define DIL_ENTRIES ENTRIES
	#define DWL_ENTRIES ENTRIES

	char* filename;	
	char* msg_str;
	char*s;
	char*os;
	
	char invalid = '\0';
	char regExp = 0;
	
	BOOL color;

	static void init( void );
	static void finally( void );

	char outputFile[261] = { '\0' };
	char search_string[1025] = { '\0' };

	WIN32_FIND_DATA NullEntry;

	#define DEFAULT ""
	#define backslash "\\"
	#define star "*"
	#define cd "."
	#define bd ".."

	static FILE* f;
	int matches = 0;

	char FLAGS = 0;
	#define RECURSE 1
	#define OTF 2 // "output to file" flag, for sending a copy of search results to file.
	#define DIR 4 // flag is set if cmd-line option to search for a directory name, not file name.
	#define FILE_CONTENTS 8 // flag is set if option to search for string inside files set
	#define HTML 16
	#define ASTRSK 32
	#define CASE_INSENSITIVE 64
	#define TA_ONLINE 128
	#define TA_OUTPUT "System online."

	BOOL print( char* );
	void output( char*, char*, int );
	void seperateFilesFromFolders( WIN32_FIND_DATA[], WIN32_FIND_DATA[], WIN32_FIND_DATA[], char* );
	void listFilesInDirectory( char[], WIN32_FIND_DATA[] );
	char cmpPatterns( wregex_t*, char* ); // return 1 if the 2 input filenames have the same ext 

	void search( char*, char* [], int*, wregex_t* );

	#ifndef STD_OUTPUT_HANDLE
	#define STD_OUTPUT_HANDLE ((DWORD)-11)
	#endif
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
	#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif

	HANDLE StdHandle;
	LPDWORD resultCode;
	//char* test = "Hiya!\n";

#endif

/*
  CON, PRN, AUX, NUL 
  COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9
  LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, LPT9
  
  The above strings are all reserved windows filenames, with or without
  arbitrary extension e.g. CON.txt.
*/

