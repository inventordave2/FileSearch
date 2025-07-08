#ifndef DAVELIB_FILESEARCH_H_
#define DAVELIB_FILESEARCH_

#include <windows.h>
char* test = "Hiya!\n";

typedef struct FileSearch	{
	
	void (*seperateFilesFromFolders)( WIN32_FIND_DATA[], WIN32_FIND_DATA[], WIN32_FIND_DATA[], char* );
	void (*listFilesInDirectory)( char[], WIN32_FIND_DATA[] );
	void (*search)( char*, char*[], int*, wregex_t* );
	
	void (*init)();
	void (*finally)();
} FileSearch;

extern struct FileSearch fs;
extern void InitFileSearchLib();

#endif

