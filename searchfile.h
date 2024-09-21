#ifndef _FS_SEARCHFILE_SUB_
#define _FS_SEARCHFILE_SUB_

	#ifndef FS_FC_PATTERN
	#define FS_FC_PATTERN 1
	#define FS_FC_STRING 2
	#endif
	
extern wregmatch_t* subm_g;
extern wregex_t* r_g;

extern signed int searchfile( char*, void*, int );

#endif

