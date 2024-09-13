#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "barnyard.h"

#include "str.h"
#include "ansivt2.h"


enum status input(char*prmpt, char*buff, size_t sz)	{

    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {

		printf("%s ", prmpt);
        fflush (stdout);
    }
	
	fgets(buff, sz, stdin);


    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
		
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        
		//return (extra == 1) ? TOO_LONG : OK;
    }
	
	
    buff[strlen(buff)-1] = '\0';
		
	if(strlen(buff)==0)
		return NO_INPUT;
	
	char* p = (char*)malloc(100);
	strcpy(p, buff);
	
	if( cmp(p, "quit") || cmp(p, "q") )	{
		
		free(p);
		return QUIT;
	}
	free(p);
	
    return OK;	
}

enum status inputNum(char*prmpt, char*buff, size_t sz)	{
	
	if(input(prmpt, buff, 1) != OK)
		return NO_INPUT;
	
    if (atoi(buff) == 0)
        return NO_INPUT;

	return OK;
}


/**
	char* strc(char*str1, char*str2);
	char* strci(char*str1, char*str2);

	char* strd(char*str1, char*str2);
	char* strdi(char*str1, char*str2);

	void strp(char*str1, char*str2);
	void strpi(char*str1, char*str2);

	void strpd(char*str1, char*str2);
	void strpdi(char*str1, char*str2);
*/

/**
	STRC (str1, str2)	Returns a (char*) of str1 + str2. (+ is a concatenation operator)
	
*/
char* strc(char*str1, char*str2)	{

		// Leaks. Heap-allocated storage for 'r' can only be freed if the handle to it is
		// preserved. Embedding the strc( function as an argument in a calling function call
		// will mean the reference is lost, thus the memory will never be freed.
		
		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		
		strcpy(r, str2);
		strcat(r, str1);
		
		return r;
}

char* strci(char*str1, char*str2)	{
	
	char* output = (char*)malloc(MAX_FILE_PATH_LENGTH);
	
	strcpy(output, str2);
	strcat(output, str1);
	strcat(output, NORMAL);
	
	return output;
}

char* strd(char*str1, char*str2)	{

		// Leaks. Heap-allocated storage for 'r' can only be freed if the handle to it is
		// preserved. Embedding the strc( function as an argument in a calling function call
		// will mean the reference is lost, thus the memory will never be freed.

		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		
		r = strc(str1, str2);
		
		free(str1); free(str2);
		
		return r;
}

char* strdi(char*str1, char*str2)	{

		// Leaks. Heap-allocated storage for 'r' can only be freed if the handle to it is
		// preserved. Embedding the strc( function as an argument in a calling function call
		// will mean the reference is lost, thus the memory will never be freed.

		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		
		r = strci(str1, str2);
		
		free(str1); free(str2);
		
		return r;
}

/**
	STRP(str1, str2)	- PRINT 2 strcings to STDOUT -
	
	Concatenates, and prints the 2 strcings to STDOUT.
	
	str1, str2 are preserved.
	
*/
void strp(char*str1, char*str2)	{

		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		r = strc(str1, str2);
		
		printf("%s", r);
		free(r);
}

void strpi(char*str1, char*str2)	{

		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		r = strci(str1, str2); 
		
		printf("%s", r);
		free(r);
}

/**
	STRPD(str1, str2)	- PRINT, then free() str1 & str2 (DELETE) -
	
	Concatenates, and prints the 2 strcings to STDOUT, but then
	frees/deletes the references to str1 & str2.
	
*/
void strpd(char*str1, char*str2)	{

		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		r = strd(str1, str2);
		
		free(str1);
		free(str2);
		
		printf("%s", r);
		free(r);
}

void strpdi(char*str1, char*str2)	{

		char* r = (char*)malloc(MAX_FILE_PATH_LENGTH);
		r = strdi(str1, str2);
		
		free(str1);
		free(str2);
		
		printf("%s", r);
		free(r);
}
