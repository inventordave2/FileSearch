#include <string.h>
#include <stdlib.h>
#include "barnyard.h"

int cmp( char* a, char* b ) { // returns true (1) if the 2 c-strings match, as it should...

	return ( strcmp(a, b) == 0 );
}

void rotate( int* argc, char* argv[] )	{

	for( int i=0; i < (*argc - 1); i++ )
		argv[i] = argv[i+1];

	if( *argc>0 )
		argv[*argc - 1] = '\0';

	*argc -= 1;
}

void flipstr( char* in, char* out )	{
	
	unsigned len = strlen(in);
	
	for( signed i = len - 1; i >= 0; i-- )
		*(out++) = *(in+i);
	
	*out = '\0';
}

