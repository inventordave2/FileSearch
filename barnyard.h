#ifndef _BARNYARD_

	#define _BARNYARD_ "The cows, the cows are calling..."
	#define MAX_FILE_PATH_LENGTH 261

	enum status	{
		
		OK,
		TOO_LONG,
		NO_INPUT,
		
		SUCCESS_,
		ERROR_,
		
		QUIT,
	};

	//enum status setTxtTheme(char*); // for use in colourizing text output to console.
	int cmp( char*, char* ); // returns true (1) if the 2 c-strings match, as it should...
	void rotate( unsigned* argc, char*argv[] ); // ruddy useful, shifts the cmd-line arguments array left, removing the one at argv[0]
	void flipstr( char* in, char* out ); // places string at in into out in reverse sequence.

/*
	struct Theme	{
		
		char name[100]; // e.g. "Standard Green"
		char text[17]; //TEXT_COLOR
		char num[17]; //NUM_COLOR 
		char bgColor[17]; //BG_COLOR
		char sym[17]; //SYMBOLS
		char cap; // CAPITALISE
		char block; // BLOCK LETTER
	};
*/

/*
	struct App	{
		
		struct Theme _Theme;
		struct Theme Themes[50];
		int ThemeCount;
		
		int GUI; // set to 0, or to a GUI system to use.
		char cd[261];
		char ansicon;
		char AppName[261];
		char Developer[100];
	};
*/

#endif
