#include <string.h>
#include <stdlib.h>
#include "barnyard.h"

enum status setTxtTheme(char * theme)	{
	
	struct App App_;
	
	for (int i = 0; i < App_.ThemeCount; i++)		
		if(cmp(App_.Themes[i].name, theme))	{
			
			App_._Theme = App_.Themes[i];
			return SUCCESS_;
		}
	
	return ERROR_;
};

int cmp(char *a, char *b) { // returns true (1) if the 2 c-strings match, as it should...

	if (strcmp(a, b) == 0)
		return 1;

	return 0;
}

void rotate(unsigned int * argc, char * argv[], int * _free)	{

	/*
	if(*_free >= 1)
		free(argv[0]);
	*/

	for(int i = 0; i < (*argc - 1); i++)	{
		
		argv[i] = argv[i+1];
	}

	argv[*argc - 1] = '\0';
	*argc -= 1;
}
// fsd.exe
// -ci 
// -c off
// -f "fs.c" +i "Kit"
void flipstr(char * in, char * out)	{
	
	unsigned int len = strlen(in);
	
	for(signed int i = len - 1; i >= 0; i--)
		*(out++) = *(in+i);
	
	*out = '\0';
}
