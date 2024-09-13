#ifndef _DAVE_STR_CONCAT_PRINT_

	#define _DAVE_STR_CONCAT_PRINT_

	// Simple cstring library. Helper functions for:
	//
	// Concatenating 2 strings, as again but also destroying input string references/heap-allocated memory.
	// As above, but also print to stdout
	//
	// strc, strci, strd, strdi
	//
	// strp, strpi, strpd, strpdi
	//

	// Also, stdin functions:
	//
	// input, inputNum

	/**
		STRC (str1, str2)	Returns a (char*) of str1 + str2. (+ is a concatenation operator)
		
	*/
	char* strc(char*str1, char*str2);
	char* strci(char*str1, char*str2);

	char* strd(char*str1, char*str2);
	char* strdi(char*str1, char*str2);

	/**
		STRP(str1, str2)	- PRINT 2 strcings to STDOUT -
		
		Concatenates, and prints the 2 strcings to STDOUT.
		
		str1, str2 are preserved.
		
	*/
	void strp(char*str1, char*str2);
	void strpi(char*str1, char*str2);

	/**
		STRPD(str1, str2)	- PRINT, then free() str1 & str2 (DELETE) -
		
		Concatenates, and prints the 2 strcings to STDOUT, but then
		frees/deletes the references to str1 & str2.
		
	*/
	void strpd(char*str1, char*str2);
	void strpdi(char*str1, char*str2);

	enum status input(char*prmpt, char*buff, size_t sz);
	enum status inputNum(char*prmpt, char*buff, size_t sz);


#endif
