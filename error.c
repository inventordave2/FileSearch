

#include <windows.h>

#define WEOF 65535

#include <stdio.h>
#include <string.h>

#include "error.h"

void Error(LPTSTR lpszFunction)	{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message.

/*
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
*/		
	int len = (strlen((LPCTSTR)lpMsgBuf) + strlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR);
	
	char * str;
	char * num_str = (char *)malloc(10);
	snprintf (num_str, 10, "%d", dw);
	str = (char *)malloc(len);
	strcpy(str, lpszFunction);
	strcat(str, " failed with error ");
	strcat(str, num_str);
	strcat(str, ": ");
	strcat(str, (char *)lpMsgBuf);
	
	//
/**	
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
*/		
    MessageBox(NULL, (LPCTSTR) str/*lpDisplayBuf*/, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

