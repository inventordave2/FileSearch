#ifndef DAVELIB_FILESEARCH_H_
#define DAVELIB_FILESEARCH_

#include <windows.h>

typedef struct FileSearchOptions	{
	
	char* fileName;
	char* fileContents;
	
	BOOL r;
	BOOL c;
	BOOL fd;
	BOOL fc;
	BOOL html;
	BOOL fo;
	char outputFileName[ MAX_FILE_PATH_LENGTH ];
	
	BOOL regex;

} FileSearchOptions;


typedef struct FileSearch	{
	
	void (*seperateFilesFromFolders)( WIN32_FIND_DATA[], WIN32_FIND_DATA[], WIN32_FIND_DATA[], char* );
	void (*listFilesInDirectory)( char[], WIN32_FIND_DATA[] );
	void (*search)( char*, char*[], int*, wregex_t* );
	// "fs [-r] [-o outputfile] [-c [on|off]] [-re|-regexp] [[-f|-d] \"file/dirname\"] [-html] [-fc \"file content\"]\n"
	void (*output)( 
	struct FileSearchOptions* searchOptions;
	void (*init)();
	void (*finally)();

} FileSearch;

extern const struct FileSearch* fs;
extern void InitFileSearchLib();

#endif
