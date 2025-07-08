// FS_C

/* WIN32 API INCLUDES */
#include <windows.h>
#include <shellapi.h>
#include <winnt.h>

/* C STDLIB INCLUDES */
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* THE VENERABLE DAVELIB INCLUDES */
#include "./barnyard.h"
#include "./../stringy/stringy.h"

#define _BYPASS_ANSIVT 0
#include "./../colour/colour.h"
#include "./error.h"
#include "./str.h"
#include "./../regex_w/wregex.h"
#include "./../regex_w/wrx_prnt.h"

/* FS_H */
#include "./fs.h"


/* FS_C INTERNAL DATA */
static FILE* f;
static HANDLE StdHandle;
static LPDWORD resultCode;
#define MAX_NUM_FOLDERS 2000
#define MAX_NUM_FILES 2000
#define ENTRIES 25
#define IL_ENTRIES ENTRIES
#define WL_ENTRIES ENTRIES
static char * filename;
static char * ignoreList;
static char * whiteList;
static char * defaultIgnoreList;
static char * defaultWhiteList;
#define DIL_ENTRIES ENTRIES
#define DWL_ENTRIES ENTRIES
static BOOL color;
static char outputFile[261] = "\0";
static char search_string[1025] = "\0";
static WIN32_FIND_DATA NullEntry;
static char* msg_str;
static char* s;
static char* os;
static char invalid = '\0';
static char regExp = 0;
#define DEFAULT ""
#define backslash "\\"
#define star "*"
#define cd "."
#define bd ".."
static int matches = 0;
static char FLAGS = 0;
#define RECURSE 1
#define OTF 2 // "output to file" flag, for sending a copy of search results to file.
#define DIR 4 // flag is set if cmd-line option to search for a directory name, not file name.
#define FILE_CONTENTS 8 // flag is set if option to search for string inside files set
#define HTML 16
#define ASTRSK 32
#define CASE_INSENSITIVE 64
#define TA_ONLINE 128
#define TA_OUTPUT "System online."
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

/*
  CON, PRN, AUX, NUL 
  COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9
  LPT1, LPT2, LPT3, LPT4, LPT5, LPT6, LPT7, LPT8, LPT9
  
  All reserved windows filenames, with or without
  arbitrary extension e.g. CON.txt.
*/

static char* reservedwin32="CON|PRN|AUX|NUL|COM1|COM2|COM3|COM4|COM5|COM6|COM7|COM8|COM9|LPT1|LPT2|LPT3|LPT4|LPT5|LPT6|LPT7|LPT8|LPT9";

uint8_t checkFileNameValidity( char* fn )	{

	if( stringy.strlen(fn)>MAX_FILE_PATH_LENGTH )
		return 0;
	
	char* ptn = (char*)malloc( 1 + 1 + strlen(reservedwin32) + 1 + 3 + 1 );
	// ptn = "^" [concat] reservedwin32 [concat] "\\."
	
	
	strcpy( ptn, "^" );
	strcat( ptn, "(" );
	strcat( ptn, reservedwin32 );
	strcat( ptn, ")" );
	strcat( ptn, "\\." );
	
	if ( regex( ptn, fn )==1 )
		return 0;
	
	strcpy( ptn, "[/");
	
	#ifdef WIN32
	strcat( ptn, "<>:\"\\\\\\|\\?\\*" );
	#endif
	
	strcat( ptn, "]" );
	
	if( regex( ptn, fn )==1 )
		return 0;
	
	return 1;
}

/* FS_C INTERNAL METHODS */
static void init(void);
static void finally(void);
static BOOL print(char *);
static void output(char *, char *, int);
static void seperateFilesFromFolders(WIN32_FIND_DATA[], WIN32_FIND_DATA[], WIN32_FIND_DATA[], char *);
static void listFilesInDirectory(char[], WIN32_FIND_DATA[]);
static char cmpPatterns(wregex_t *, char *); // return 1 if the 2 input filenames have the same ext 
static void search(char *, char * [], int *, wregex_t *);
static signed int getOptions(int * argc, char * argv[], int * _free);

// 1. set a breakpoint "break fs.c:333" 
// 2. step/next/print


static signed int getOptions(int * argc, char * argv[], int * _free)	{

	enum status rc;
	char buff[200];
		
	char i = 0, j = 0;
	char suffix[2];
	suffix[0] = '\0';
	suffix[1] = '\0';
	
	filename[0] = '\0';
	
	if(*argc == 0)	{
		
		rc = input( strci(FG_BRIGHT_YELLOW, strc("Please enter the cmd-line arguments to pass to 'fs': ", FG_YELLOW)), buff, 200 );
	
		if(rc == QUIT)	{
		
			printf( "\n%sExiting Program. Dave & Heck thank you! Come again!%s", FG_BRIGHT_GREEN, NORMAL );
			printf( " %s%shttps://www.inventordave.com/%s\n", FG_BRIGHT_WHITE, BG_BLUE, NORMAL );
			exit(0);
		}
		
		if (rc == NO_INPUT) {

			printf( "\n%sNo input.%s\n", FG_BRIGHT_RED, NORMAL );
			return -1;
		}
		
		// 2nd go-around. Interactively take in search term & flags (TODO).
		
		char *argv2[10];
		
		argv2[0] = (char *)malloc(100);
		strcpy(argv2[0], "fs"); // exe name isn't processed.
		++(*argc);
		//getOptions(&argc, argv2, &_free);
		
		argv = argv2;
		
		/* */
		while(1)	{
		
			argv[*argc] = (char *)malloc(200 * sizeof(char *));
			
			suffix[0] = '\0';
			strcpy(argv[*argc], (char *)suffix);
			
			//printf( "\targ: '%s'\n", argv[*argc] );
			
			j = 0;
			while( (buff[i] != ' ') && (buff[i] != '\0') )	{
				
				if( (j == 0) && (buff[i] == '"') )	{
					
					++j;
					++i;
					continue;
				}
				
				suffix[0] = buff[i];
				strcat(argv[*argc], (char *)suffix);	
				
				++i;
			}
			
			(argv[*argc][strlen(argv[*argc])-1]=='"') && (argv[*argc][strlen(argv[*argc])-1] = '\0');
			
			printf( "Arg: '%s'\n", argv[*argc] );
			++(*argc);
			
			if('\0' == buff[i])
				break;
			
			++i;
		}

	}

	else if( (*argc == 1) && (*_free == 0) )	{ // no cmd-line args passed on invocation
	
		printf( "\n%s%sUsage%s", FG_BRIGHT_YELLOW, BG_BRIGHT_BLUE, NORMAL );
		printf( "%s:%s ", FG_BRIGHT_RED, NORMAL );
		printf( "fs [-r] [-o outputfile] [-c [on|off]] [-re|-regexp] [[-f|-d] \"file/dirname\"] [-html] [-fc \"file content\"]\n" );

		exit(1);
	}
	
	FLAGS |= TA_ONLINE;		

	for(;;)	{
		
		//printf( "argc == %d\n", *argc );

		rotate(argc, argv, _free); // consume last cmd-line component, first time around, that's the fs exe name.

		if(*argc==0)		
			// all cmd-line args consumed.
			break;
		
		//printf( "Processing args...\n" );
		
		if(cmp(argv[0], "-r"))	{
			
			FLAGS |= RECURSE;
			continue;
		}
		
		if(cmp(argv[0], "-o"))	{

			strcpy(outputFile, argv[1]);
			FLAGS |= OTF;
			
			rotate(argc, argv, _free);
			// 2 components,
			// -f then "filename"
			// delete 1st, 2nd will be shifted
			// on loop-around
			
			continue;
		}
		
		if(cmp(argv[0], "-c"))	{
			
			if(cmp(argv[1], "off") || cmp(argv[1], "0"))	{
				
				resetAnsiVtCodes(0);
				printf( "Colour output DISABLED.\n" );
				rotate(argc, argv, _free);
			}
			else	{

				if(color!=0)	{
					
					resetAnsiVtCodes(1);
					
					sprintf(msg_str, "%s%s%s\n", FG_BRIGHT_GREEN, "Colour output ENABLED.", NORMAL );
					printf( msg_str );
				}
				
				if(cmp(argv[1], "on") || cmp(argv[1], "1"))
					rotate(argc, argv, _free);
			}	
			
			continue;
		}
		
		if( cmp(argv[0], "-dir")||cmp(argv[0], "-d") )	{
			
			if(argv[1][0]!='-')	{ // next arg is directory name to find (but can do -f filename to set the dirname.
			
				strcpy(filename, argv[1]);
				rotate(argc, argv, _free); // shunts off "-d", next loop-around, the dirname will be automatically shunted off the arg list.
			}
			
			FLAGS |= DIR;
			continue;
		}
		
		if( cmp(argv[0], "-filecontents") || cmp(argv[0], "-fc") )	{
			
			strcpy(search_string, argv[1]);
			FLAGS |= FILE_CONTENTS;
			
			rotate(argc, argv, _free);
			// 2 components,
			// -filecontents then "search string (max 1023 octets/ANSI)"
			// delete 1st, 2nd will be shifted
			// on loop-around			
			continue;
		}
		
		if( cmp(argv[0], "-regexp") || cmp(argv[0], "-re") )	{
			
			regExp = 1;
			continue;
		}
		
		if(cmp(argv[0], "-html"))	{
			
			FLAGS |= HTML;
			continue;
		}
		
		if(cmp(argv[0], "-ci"))	{
			
			FLAGS |= CASE_INSENSITIVE;
			continue;
		}
		
		if(cmp(argv[0], "-i"))	{
			
			if(argv[1][0] == '-')	{
				
				print( "Used '-i' switch, but passed no DIR_BLOCK list! Assuming default list.\n" );
				strcpy(ignoreList, defaultIgnoreList);
			}
			else	{
				
				strcpy(ignoreList, argv[1]);
				rotate(argc, argv, _free);
			}
			
			continue;
		}
		
		if(cmp(argv[0], "+i"))	{
			
			if( (argv[1][0] == '-') || (argv[1][0] == '+') )	{
				
				print( "Used '+i' switch, but passed no DIR_WHITELIST list! Assuming default list.\n" );
				strcpy(whiteList, defaultWhiteList);
			}
			else	{
				
				strcpy(whiteList, argv[1]);
				rotate(argc, argv, _free);
			}
			
			continue;
		}
		
		if( cmp(argv[0], "-f") || cmp(argv[0], "-filename") )	{ // optional switch, can just pass filename pattern as a stand-alone arg.

			if(argv[1][0]!='-')	{ // next arg is filename to find (but can do -d dirname to set the dirname).

				strcpy(filename, argv[1]);
				rotate(argc, argv, _free); // shunts off "-d", next loop-around, the dirname will be automatically shunted off the arg list.
			}

			continue;
		}
		
		strcpy(filename, argv[0]);
	}
	
	if( cmp(filename, "") )
		strcpy(filename, "*");
	
	int t = 0;
	char temp[100];
	
	char* fn = filename;
	
	while((*fn) != '\0')
		if((*(fn++)) < 32)	{
			
			invalid = (*fn);
			printf( "%sInvalid non-printable char (%d)('%c') @ offset(%d).%s\n", FG_BRIGHT_RED, invalid, invalid, (fn - filename), NORMAL );
			continue;
		}
	
	fn = filename;
	if((*fn) == ':')	{
		
		regExp = 1;
		filename++;
	}
	if(regExp == 1)
		;
	else
		while((*fn) != '\0')	{
			
			switch(*fn)	{

				case 92: // \ backslash
					
					break;
				case 34: // " double-quote
				//case 42: // * star, we allow this for wc-search
				case 47: // / forward-slash
				case 58: // : colon
				case 60: // < lt
				case 62: // > gt
				case 63: // ? question-mark

					
				case 124:// | pipe
				
					invalid = *fn;
					printf( "%s%sInvalid char ('%s%c%s') (%s%d%s).\n", BG_WHITE, FG_BRIGHT_RED, FG_BRIGHT_BLUE, invalid, FG_BRIGHT_RED, FG_BRIGHT_BLUE, invalid, FG_BRIGHT_RED );

					// char* fmtd_msg = colour.fmt( "[bg:white,fg:brightred]Invalid char ('[brightblue]%c[brightred]') ([brightblue]%d[brightred]).\n" );
					// fprintf( stderr, fmtd_msg, invalid, invalid );

					break;
				
				default:
					//printf( "Char ok: '%c'\n", (*fn) );
					break;	
			}
			
			++fn;
		}
	;
	
	++(*_free);
}

int main( int argc, char** argv )	{
	
	int _free = 0;
	
	init();
	
	while(getOptions(&argc, argv, &_free) == -1)
		;
	
	StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	color = SetConsoleMode(
		StdHandle,
		0x0001 | 0x0002 | ENABLE_VIRTUAL_TERMINAL_PROCESSING
	);

	
	//sprintf( msg_str, "ResponseCode(SetConsoleMode) := '%s'.\n", (color == 0 ? "FAIL" : "SUCCESS") );
	//print( msg_str );
	
	if(color == 0)
		Error( TEXT("WriteConsole()") );
	
	strcpy(defaultIgnoreList, ".git|.vscode|node_modules" ); /** ".git|.vscode|node_modules" */
	strcpy(defaultWhiteList, "./\0" ); /** "src|x64|test|node_modules|debug|release|htdocs" */
	
	if(_BYPASS_ANSIVT == 1)	{
		
		//print( "ANSI/VT Colour mode for fileSearch (fs) has been bypassed at build time.\nFor colour support, please restart 'fs' using cmd-line switch \"-c\", or recompile changing '_BYPASS_ANSIVT' at the top of 'fs.c' to any value but 1.\n\n" );
		colour.resetAnsiVtCodes(0);
	}
	else if(color==0)	{
		
		printf( "ANSI/VT Color mode is not available.\n" );
	}
	else
		resetAnsiVtCodes(1), printf( "%sANSI/VT %smode has been activated.%s\n", FG_BRIGHT_YELLOW, FG_GREEN, NORMAL );
		// Alt print method: colour.fmtp( "[brightyellow]ANSI/VT [green]mode has been activated.[reset]\n" );
	printf( "Filename/Pattern: '%s'\n", filename );
	
	char * fn = (char*) malloc( MAX_FILE_PATH_LENGTH + 1 );
	fn[0] = '\0';
	
	while(1)	{

		if(invalid != 0)	{
			
			printf( "%sThere are illegal characters in the file/dir name ('%c'). Exiting...%s\n", FG_BRIGHT_RED, invalid, NORMAL );
			// char* msg = colour.fmt( "[brightred]There are illegal characters in the file/dir name ('%c'). Exiting...[reset]\n" );
			// fprintf( stderr, msg, invalid );
			break;
		}		
		
		fn = filename;
		char * fn_copy = (char*)malloc( MAX_FILE_PATH_LENGTH + 1 );
		strcpy(fn_copy, filename);
		
		if(regExp == 0)	{
			
			//print("Inside wildcard (*) replace block.\n");
			
			char * fn2 = (char *)malloc(100 * sizeof(char *));
			fn2[0] = '^';
			fn2[1] = '\0';
			
			char * sub = (char *)malloc(100 * sizeof(char *));
			sub[0] = '\0';
			
			while((*fn) != '\0')	{
				
				if((*fn) == '*')
					strcat(fn2, "[A-Za-z0-9\\\\!\\\\#\\\\%\\\\&\\\\'\\\\,\\\\-\\\\;\\\\=\\\\@\\\\_\\\\`\\\\~\\\\.]*");
				else if( (*fn == '.') || (*fn == '-') )	{
					
					//printf ( "%sDetected special char: '%s%c%s'%s\n", FG_BRIGHT_BLUE, FG_WHITE, (*fn), FG_BRIGHT_BLUE, NORMAL );
					sub[0] = '\\';
					sub[1] = (*fn);
					sub[2] = '\0';
					strcat(fn2, sub);
				}
				else	{
					
					short unsigned k = 0;
					
					if(FLAGS&CASE_INSENSITIVE)	{
						
						sub[k++] = '[';
						sub[k++] = (*fn);
						sub[k++] = '|';
						sub[k++] = ( tolower(*fn)==(*fn) ? toupper(*fn) : tolower(*fn) );
						sub[k++] = ']';
					}
					else
						sub[k++] = (*fn);
					
					sub[k] = '\0';
					strcat(fn2, sub);
				}
				
				//printf ("Filename so far: '%s'\n", fn2);
				++fn;
			}	
			
			sub[0] = '$';
			sub[1] = '\0';
			strcat(fn2, sub);
			strcpy(filename, fn2);
			
			//printf( "Filename modified by preprocessor to '%s'\n", fn2 );
			//printf( "%sProcessed non-RegExp input search pattern for internal RegExp engine!%s\n", FG_YELLOW, NORMAL );
			
			free(sub); free(fn2);
		}
	
		int e, ep;
		
		if(FLAGS&CASE_INSENSITIVE)	{
			
			char *temp = (char*)malloc( MAX_FILE_PATH_LENGTH + 1 );
			strcpy(temp, filename);
			strcpy(filename, "\\i");
			strcat(filename, temp);
			free(temp);
		}
		
		wregex_t* regexp = wrx_comp(filename, &e, &ep);
		// DEBUG regxp NFA
		//wrx_print_nfa(regexp);
		//printf("Survived printf_nfa().\n");

		if(FLAGS&OTF)	{
			
			f = fopen(outputFile, "w");
			if (f == NULL)	{
				
				fprintf( stderr, "Error opening/creating file '%s'!\n", outputFile );
				finally();
				exit(1);
			}
			
			if(FLAGS&HTML)	{
				
				char* styles = "\nbody\t{\n\tcolor: brown;\n}\nbody > span\t{\n\tcolor: magenta;\n\tfont-weight: bold;\n}\ndiv > a\t{\n\tcolor: #00f;\n}\ndiv > span > a\t{\n\tcolor: #0f0;\n}\n";
				
				sprintf(s, "<!doctype HTML>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<style>%s</style>\n<body>\n", styles);
				
				sprintf(s, "%s%s", s, "Here are your search results for query: ");
				sprintf(s, "%s'<span>%s</span>'%s", s, fn_copy, "<br>\n<br>\n");
				
			}
			else	{
				
				sprintf(s, "%s%s", s, "Here are your search results for query: ");
				sprintf(s, "%s'%s'%s", s, fn_copy, "\n\n");
			}
			
			for (int i = 0; s[i] != '\0'; i++)
				/* write to file using fputc() function */
				fputc(s[i], f);
		}
		
		free(fn_copy);
		
		#define MAX_NUM_RESULTS 10000
		char* Results[ MAX_NUM_RESULTS ];
		int o = 0;
		
		char path[] = ".\\";
		
		if(FLAGS&DIR)
			printf( "Searching for Directory.\n" );
		
		printf( "\n" );
		
		search((char *)path, Results, &o, regexp);
		printf( "\n%sCOMPLETED. Number of Matches: %s%d%s\n", FG_GREEN, FG_BRIGHT_GREEN, matches, NORMAL );
		// char* fmtd_msg = colour.fmt( "[green]COMPLETED. Number of Matches: [brightgreen]%d[reset]\n" );
		// fprintf( stdout, fmtd_msg, matches );
		

		/*  The search results have all been collected, the church bells all were broken...
			
			Now allow the user to enter the search result number of a listing in the results. Their number is printfed right next to them in [blue].
			The files will open in the Windows handler app for the specific file type.
			
		*/
		
		enum status rc;
		char buff[10];
		char quit = 0;
		
		for(;;)	{ // Open a file, Quit program, or enter a new search query.

			// setTxtTheme(int);			
			rc = input( strci("To Open a File, enter it's number here, enter 0 for a new search,\nor type 'quit' or 'q' to exit the program: ", FG_BRIGHT_CYAN), buff, 10 );
			
			if(rc == QUIT)	{
			
				printf( "\n%sExiting Program. Dave & Heck thank you! Come again!%s", FG_BRIGHT_GREEN, NORMAL );
				printf( " %s%shttps://www.inventordave.com/%s\n", FG_BRIGHT_WHITE, BG_BLUE, NORMAL );
				quit = 1;
				break;
			}
			
			if (rc == NO_INPUT) {

				printf("\n%sNo input.%s\n", FG_BRIGHT_RED, NORMAL);
				// char* fmtd_msg = colour.fmt( "\n[brightred]No Input.[reset]\n" );
				// fprintf( stderr, fmtd_msg );
				continue;
			}

			if (rc == TOO_LONG) {
				
				printf("\n%sInput number too long.%s\n", FG_BRIGHT_RED, NORMAL);
				continue;
			}
			
			if(atoi(buff) == 0)
				break;
			
			if(atoi(buff) < 0)
				o = matches + atoi(buff);
			else
				o = atoi(buff) - 1;
			
			if(o < 0)	{
				
				printf( strc("\nIndice too small. Min search result indice:", FG_BRIGHT_RED) );
				printf( strc(" %d, ", FG_BRIGHT_YELLOW), 1 );
				printf( strc("Indice selected:", FG_BRIGHT_RED) );
				printf( strci(" %d\n", FG_BRIGHT_YELLOW), o+1 );
				
				continue;
			}
			
			else if(o >= matches)	{
				
				printf( strc("\nIndice too large. Max search result indice:", FG_BRIGHT_RED) );
				printf( strc(" %d, ", FG_BRIGHT_YELLOW), matches );
				printf( strc("Indice selected:", FG_BRIGHT_RED) );
				printf( strci(" %d\n", FG_BRIGHT_YELLOW), o+1 );
				
				continue;
			}
				
			printf( strci("\nOK. ('%d' entered.)\n", FG_BRIGHT_BLUE), (o)+1 );
			printf( "%sFILE SELECTED:\t", FG_BRIGHT_YELLOW );
			printf( "%s'%s'%s\n", FG_BRIGHT_GREEN, Results[o], NORMAL );
			printf( "%sFile has been opened in it's handler app (e.g. a .txt file may open in 'notepad.exe')%s\n\n", FG_BRIGHT_RED, NORMAL );
			
			//printf( "Text('%s')\n", Results[o] );
			
			HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			
			//printf( "...\n" );
			
			/** */
			SHELLEXECUTEINFOW pExecInfo;
			memset(&pExecInfo, 0, sizeof(SHELLEXECUTEINFOW));
			
			pExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
			pExecInfo.fMask  = ( SEE_MASK_NOASYNC | SEE_MASK_WAITFORINPUTIDLE | SEE_MASK_NO_CONSOLE | SEE_MASK_WAITFORINPUTIDLE | SEE_MASK_FLAG_LOG_USAGE );
			pExecInfo.hwnd = NULL;
			pExecInfo.lpVerb = (LPCWSTR) L"open" ;
			
			wchar_t* wcstring;
			{
				// newsize describes the length of the
				// wchar_t string called wcstring in terms of the number
				// of wide characters, not the number of bytes.
				size_t newsize = strlen(Results[o]) + 1;
				
				wcstring = (wchar_t *)malloc(newsize * sizeof(wchar_t));

				// Convert char* string to a wchar_t* string.
				//size_t convertedChars = 0; //
				//mbstowcs_s(&convertedChars, wcstring, newsize, Results[o], _TRUNCATE); //
				
				
				swprintf(wcstring, newsize, L"%hs", Results[o]);
			}
			
			pExecInfo.lpFile = (LPCWSTR) wcstring;
			pExecInfo.lpParameters = NULL; // params to callee app.
			pExecInfo.lpDirectory = NULL; // HERE
			pExecInfo.nShow = SW_SHOW;
			pExecInfo.hInstApp = NULL;


			char * txt = (char *)malloc(100);
			txt[0] = '\0';
			
			int ret = ShellExecuteExW( &pExecInfo );
			//printf( "ret := '%d'\n", ret );
			
			{
				char * msg = (char *)malloc(100);
				msg[0] = '\0';
				
				char * num_str = (char *)malloc(5);
				
				if(ret<0 || ret>32) /* not failed */
					;
				else
					switch(ret)	{
						
						case 0:
							strcpy(txt, "The operating system is out of memory or resources.");
							break;
							
						case 2:
							strcpy(txt, "The specified file was not found.");
							break;

						case 3:
							strcpy(txt, "The specified path was not found.");
							break;
							
						case 10:
							strcpy(txt, "Wrong Windows version.");
							break;
							
						case 11:
							strcpy(txt, "The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).");
							break;
							
						case 12:
							strcpy(txt, "Application was designed for a different operating system.");
							break;

						case 15:
							strcpy(txt,  "Attempt to load a real-mode program.");
							break;

						case 20:
							strcpy(txt, "Dynamic-link library (DLL) file failure.");
							break;
							
						case 27:
							strcpy(txt, "The filename association is incomplete or invalid.");
							break;
							
						case 31:
							strcpy(txt, "There is no application associated with the given filename extension.");
							break;

						default:
							
							strcpy(msg, "General Failure. Look it up! Error code: ");
							
							snprintf (num_str, 10, "%d", ret);
							//itoa(ret, num_str, 10);
							strcat( msg, num_str );
							strcat(msg, ".");
							strcpy(txt, msg);

							break;
				
					}

				free(msg);
				free(num_str);

			}
			//if(strlen(txt)!=0)
				//Error(TEXT("ShellExecute!!"));
			
			/* Alternatively, you can do:
			
				if(!(ret<0 || ret>32))
					Error(TEXT("ShellExecute!!"));
				
				And a local string describing the error can be found in 'txt'.
			
			*/
			
		}
		
		if( (FLAGS&OTF)&&(FLAGS&HTML) )	{

			char * _s = "</body>\n</html>\n";
			for (int i = 0; _s[i] != '\0'; i++)
			/* write to file using fputc() function */
				fputc(_s[i], f);
		}
		
		printf( "ok...\n" );
		wrx_free(regexp);
		
		if(quit==1)
			break;
		
		matches = 0;
		invalid = 0;
		
		//break;
		
		while (getOptions(&argc, argv, &_free) == -1)
			;
		// loop;
	}

	//free(fn);
	
	//finally();
	return 0;
}

static void init()	{
	
	filename = (char*) malloc( MAX_FILE_PATH_LENGTH + 1 );
	filename[0] = '\0';
	
	ignoreList = (char *)malloc( (MAX_FILE_PATH_LENGTH + 1) * IL_ENTRIES );
	ignoreList[0] = '\0';	

	whiteList = (char *)malloc( (MAX_FILE_PATH_LENGTH + 1) * WL_ENTRIES );
	whiteList[0] = '\0';
	
	defaultIgnoreList = (char *)malloc( (MAX_FILE_PATH_LENGTH + 1) * DIL_ENTRIES );
	defaultIgnoreList[0] = '\0';	

	defaultWhiteList = (char *)malloc( (MAX_FILE_PATH_LENGTH  + 1) * DWL_ENTRIES );
	defaultWhiteList[0] = '\0';
	
	s = (char *)malloc(MAX_FILE_PATH_LENGTH + 1);
	s[0] = '\0';
	// function to take as input a value (eg '\0'), and the number of slots in a memory space, and the size in bytes of each slot, useful for group-allocating from heap with a single malloc() call, using one allocated array.
	
	/**
	
	bool allocate(ref, bytes, octet)
	where 'ref' is a reference to the origin of the return value of the malloc() call, 'bytes' is the number of bytes per slot, and octet is the specific 8-bit value to populate the slot offset origin, e.g. '\0'
	returns 1 if the operation completes, 0 if the operation fails.
	*/
	os = (char *)malloc(MAX_FILE_PATH_LENGTH + 1);
	os[0] = '\0';
	msg_str = (char *)malloc(MAX_FILE_PATH_LENGTH + 1);
	msg_str[0] = '\0';
}

static void finally()	{
	
	free(filename);
	free(ignoreList);
	free(whiteList);
	free(defaultIgnoreList);
	free(defaultWhiteList);
	
	free(s); free(os); free(msg_str);

	if(FLAGS&OTF)
		fclose(f);
}

// outputs found item to screen, and optionally, to an output file.
static void output(char *path, char *filename, int o)	{	

	sprintf(os, "%s", path);
	sprintf(os, "%s\t%s%s%s", os, FG_BRIGHT_GREEN, filename, NORMAL);
	printf( "%s", os );

	// Write To File
	if(FLAGS&OTF)	{

		if(FLAGS&HTML)	{

			sprintf(s, "<div id=\"result%d\"><a href=\"%s\">%s</a>", o, path, path);
			sprintf(s, "%s&nbsp;&nbsp;&nbsp;&nbsp;<span><a href=\"%s\%s\">%s</a></span></div>\n", s, path, filename, filename);
		}
		else	{
		
			sprintf(s, "%s", path);
			sprintf(s, "%s\t%s\n", s, filename);
		}

		for (int i = 0; s[i] != '\0'; i++)
			/* write to file using fputc() function */
			fputc(s[i], f);
	}
	
	//free(s); free(os); // see: finally()
}

/*
Future refactor:

#ifdef WIN32
#define FIND_DATA WIN32_FIND_DATA
#else
#define FIND_DATA LINUX_FIND_DATA
#endif
*/
static void search(char* path, char* ResultObj[], int* o, wregex_t* regexp) {

	WIN32_FIND_DATA* files = (WIN32_FIND_DATA*)calloc(MAX_NUM_FILES, sizeof(WIN32_FIND_DATA));;
	WIN32_FIND_DATA* dirs = (WIN32_FIND_DATA*)calloc(MAX_NUM_FOLDERS, sizeof(WIN32_FIND_DATA));;

	char* All = (char*)calloc(MAX_FILE_PATH_LENGTH + 1);;
	WIN32_FIND_DATA* entries = (WIN32_FIND_DATA*)calloc((MAX_NUM_FILES + MAX_NUM_FOLDERS), sizeof(WIN32_FIND_DATA));
	
	strcpy(All, path);
	strcat(All, star);

	listFilesInDirectory(All, entries);
	seperateFilesFromFolders(entries, files, dirs, path);
	free(entries);

	if(!(FLAGS&DIR))
		for (unsigned int i = 0; i < MAX_NUM_FILES; i++)	{
			
			if (cmp(files[i].cFileName, ""))
				break;

			char ignoreFile = 0;
			{	// Checks to see if we should skip the current File, based on a list provided by the user on the cmd-line, via the "-i" switch.				

				int h = 0, j = 0, k = 0;
				
				char temp[MAX_FILE_PATH_LENGTH+1];
				temp[0] = '\0';
				
				while(files[i].cFileName[h] != '\0')	{
					
					temp[0] = '\0';
					
					while( (ignoreList[j] != '|') && (ignoreList[j] != '\0') )	{
					
						if(files[i].cFileName[h] == ignoreList[j])	{
							
							char t[2];
							t[0] = ignoreList[j];
							t[1] = '\0';
							
							strcat(temp, t);
						}
						else
							strcat(temp, "&&%%"); // nonsense to trigger next if-condition to resolve False.
						++j;
						++h;
					}
					
					if(cmp(files[i].cFileName, temp))	{

						printf( "Detected a File on the block list ('%s'). Ignoring.\n", files[i].cFileName );						
						ignoreFile = 1;
						break;
					}
					
					if(ignoreList[j] == '\0')
						break;
						
					++j;
					h = 0;
				}
			}
				
			if(ignoreFile == 1)
				continue;

			if (cmpPatterns(regexp, files[i].cFileName))	{ // Match Found.

				output(path, files[i].cFileName, (*o)+1);
			
				char * Result = (char *)calloc(MAX_FILE_PATH_LENGTH + 1);
				strcpy(Result, path);
				strcat(Result, files[i].cFileName);
				ResultObj[*o] = Result;
				
				*o = *o + 1;
				
				printf( " %s[%d]%s\n", FG_BRIGHT_BLUE, *o, NORMAL );
				
				if( ( ((*o) % 100) == 0 ) && (*o != 0) )
					printf( "\n%s%s-- %d Matches so far! --%s\n\n", FG_BRIGHT_YELLOW, BG_BRIGHT_BLUE, *o, NORMAL );
				
				matches++;
				
				if(*o == 10000)
					goto _return;
			}
		}
	else
		for (unsigned int i = 0; i < MAX_NUM_FOLDERS; i++)	{
			
			if (cmp(dirs[i].cFileName, ""))
				break;

			char ignoreDir = 0;
			{	// Checks to see if we should skip the current Dir, based on a list provided by the user on the cmd-line, via the "-i" switch.				

				int h = 0, j = 0, k = 0;
				
				char temp[MAX_FILE_PATH_LENGTH + 1];
				temp[0] = '\0';
				
				while(dirs[i].cFileName[h] != '\0')	{
					
					temp[0] = '\0';
					
					while( (ignoreList[j] != '|') && (ignoreList[j] != '\0') )	{
					
						if(dirs[i].cFileName[h] == ignoreList[j])	{
							
							char t[2];
							t[0] = ignoreList[j];
							t[1] = '\0';
							
							strcat(temp, t);
						}
						else
							strcat(temp, "&&%%"); // nonsense to trigger next if-condition to resolve False.
						++j;
						++h;
					}
					
					if(cmp(dirs[i].cFileName, temp))	{

						printf( "Detected a Dir on the block list ('%s'). Ignoring.\n", dirs[i].cFileName );						
						ignoreDir = 1;
						break;
					}
					
					if(ignoreList[j] == '\0')
						break;
						
					++j;
					h = 0;
				}
			}
				
			if(ignoreDir == 1)
				continue;
			
			if ( cmpPatterns(regexp, dirs[i].cFileName) && !(cmp(dirs[i].cFileName, cd) || cmp(dirs[i].cFileName, bd)) )	{ // Match Found.

				output(path, dirs[i].cFileName, (*o) + 1);
			
				char * Result = (char *)calloc(MAX_FILE_PATH_LENGTH + 1);
				strcpy(Result, path);
				strcat(Result, dirs[i].cFileName);
				ResultObj[*o] = Result;
				
				*o = *o + 1;
				
				printf( " %s[%d]%s\n", FG_BRIGHT_BLUE, *o, NORMAL );
				// char* fmtd_msg = colour.fmt( "[brightblue]\[%d][reset]\n" );
				// Then the printf() in the following if-construct would be:
				// fprintf( stdout, fmtd_msg, *o );
				if( ( ((*o) % 100) == 0 ) && (*o != 0) )
					printf( "\n%s%s-- %d Matches so far! --%s\n\n", FG_BRIGHT_YELLOW, BG_BRIGHT_BLUE, *o, NORMAL );
				
				matches++;
				
				if(*o == MAX_NUM_RESULTS)
					goto _return;
			}
		}
	;
	
	//printf( "Okay....\n" );
	
	// If flag set, recursively search each sub-directory.
	if((FLAGS&RECURSE) == 1)	{
		
		//printf( "Recursing...\n" );
		
		for (unsigned int i = 0; i < MAX_NUM_FOLDERS; i++) {

			char ignoreDir = 0;
			
			if (cmp(dirs[i].cFileName, ""))
				break;

			// REMEMBER: DON'T RECURSIVELY SEARCH . OR .. !!!!!
			if (!cmp(dirs[i].cFileName, cd) && !cmp(dirs[i].cFileName, bd)) {

				{	// Checks to see if we should skip the current Dir, based on a list provided by the user on the cmd=line, via the "-i" switch.				

					int h = 0, j = 0, k = 0;
					
					char temp[MAX_FILE_PATH_LENGTH + 1];
					temp[0] = '\0';
					
					while(dirs[i].cFileName[h] != '\0')	{
						
						temp[0] = '\0';
						
						while( (ignoreList[j] != '|') && (ignoreList[j] != '\0') )	{
						
							if(dirs[i].cFileName[h] == ignoreList[j])	{
								
								char t[2];
								t[0] = ignoreList[j];
								t[1] = '\0';
								
								strcat(temp, t);
							}
							else
								strcat(temp, "&&%%"); // nonsense to trigger next if-condition to resolve False.
							++j;
							++h;
						}
						
						if(cmp(dirs[i].cFileName, temp))	{
							
							printf( "Detected a Dir on the block list ('%s'). Ignoring.\n", dirs[i].cFileName );
							
							ignoreDir = 1;
							break;
						}
						
						if(ignoreList[j] == '\0')
							break;
							
						++j;
						h = 0;
					}
				}
				
				if(ignoreDir == 1)	{
				
					//printf( "IGNORE_DIR: '%s'\n", dirs[i].cFileName );
					ignoreDir = 0;
					continue;
				}


				char* subdirectory = (char*)calloc(MAX_FILE_PATH_LENGTH + 1);
				char* path2 = (char*)calloc(MAX_FILE_PATH_LENGTH + 1);;

				strcpy(subdirectory, dirs[i].cFileName);
				strcpy(path2, (char *)path);
				strcat(path2, subdirectory);
				strcat(path2, backslash);

				free(subdirectory);

				if(strlen(path2) > MAX_FILE_PATH_LENGTH)
					printf("%sRuh-roh! The length of the current path is > %d.%s", FG_BRIGHT_RED, MAX_FILE_PATH_LENGTH, NORMAL);


				search(path2,  ResultObj, o, regexp);

				free(path2);
				
				if(*o == MAX_NUM_RESULTS)
					goto _return;
			}
		}
	}
	
	_return:
	
	//printf( "@ _return:\n" );
	free(dirs);
	free(files);
	free(All);
}


static void listFilesInDirectory(char path[], WIN32_FIND_DATA entries[]) {

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(path, &data);
	
	int i = 0;
	int folderCount = 0;
	int fileCount = 0;

	if (hFind != INVALID_HANDLE_VALUE) {

		do {
			// data.cFileName
			entries[i] = data;
			
			if(entries[i].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				folderCount += 1;
			else
				fileCount += 1;
			
			++i;
			
		} while ( (i < (MAX_NUM_FOLDERS + MAX_NUM_FILES)) && FindNextFile(hFind, &data) );

		FindClose(hFind);
	}
		
	entries[i] = NullEntry;
	
	return;
}

static void seperateFilesFromFolders(WIN32_FIND_DATA entries[], WIN32_FIND_DATA files[], WIN32_FIND_DATA dirs[], char * pathname) {

	int i = 0;
	int di2 = 0;
	int fi2 = 0;
	while (!cmp(entries[i].cFileName, "")) {

		if (entries[i].dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	{

			if(di2>(MAX_NUM_FOLDERS-1))	{
				
				i++; di2++;
				continue;
			}
			
			dirs[di2++] = entries[i];
		}	
		else	{

			if(fi2>(MAX_NUM_FILES-1))	{
				
				i++; fi2++;
				continue;
			}
			
			files[fi2++] = entries[i];
		}

		++i;
	}

	if (fi2 >= (MAX_NUM_FILES-1))
		; //printf("File count for %s is %d.\n", pathname, fi2-1);

	if (di2 >= (MAX_NUM_FOLDERS-1))
		; //printf("Folder count for %s is %d.\n", pathname, di2-1);
}

static char cmpPatterns(wregex_t * regexp, char * pattern2)	{

	wregmatch_t dummy;
	wregmatch_t ** subm/*[10]*/ = (wregmatch_t **)calloc(10, sizeof(wregmatch_t *));
	for(int i = 0; i < 10; i++)
		subm[i] = (wregmatch_t *)calloc(1, sizeof(wregmatch_t *)), *subm[i] = dummy;
	
	int nsm = 10;
	int result = wrx_exec(regexp, pattern2, subm, &nsm);
	
	if(result != 1)
		// NO MATCH.
		return 0;

	return 1;
}

static BOOL print(char * str)	{
	
	return
		WriteConsole(
		StdHandle,
		(VOID *) str,
		strlen(str),
		resultCode,
		NULL
	);
}
