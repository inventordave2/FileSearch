// WS_FS_C

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "regex/wregex.h"
#include "searchfile.h"

static int _match( wregex_t *p, const char *s, const char *file, int line );
#define match(p, s)   _match(p, s, __FILE__, __LINE__)
#define regex( s,p ) match(p, s)

static int e, ep;

signed int searchfile( char* _, void* pattern, int type )	{
	
	wregex_t* p;
	if( type==FS_FC_PATTERN )
		p = (wregex_t*)pattern;
	else	{
		p = wrx_comp((char*)pattern, &e, &ep);	
	}
	
	subm_g = (wregmatch_t*)malloc( sizeof(wregmatch_t) );
	r_g    = (wregex_t*)   malloc( sizeof(wregex_t) );

	// loadFile. Test againstthe werneyEngine.
	int j = -1;
	if( regex( _,p ) )
		j = (int) (subm_g[0].end - _);
	
	return j;
}

static int _match( wregex_t *p, const char *s, const char *file, int line ) {

	//wregex_t *r;
	wregmatch_t *subm;

	//r = wrx_comp(p, &e, &ep);
	if(!p) {
		fprintf(stderr, "\n[%s:%d] ERROR......: %s\n%s\n%*c\n", file, line, wrx_error(e), "NULL WREGEX_T* P", ep + 1, '^');
		exit(EXIT_FAILURE);
	}

	if(p->n_subm > 0) {
		subm = calloc(sizeof *subm, p->n_subm);
		if(!subm) {
			fprintf(stderr, "Error: out of memory (submatches)\n");
			wrx_free(p);
			exit(EXIT_FAILURE);
		}
	} else
		subm = NULL;

	e = wrx_exec(p, s, &subm, &p->n_subm);

	if(e < 0) fprintf(stderr, "Error: %s\n", wrx_error(e));

	*subm_g = *subm;
	*r_g = *p;
	
	free(subm);
	//wrx_free(p);

	return e;
}

