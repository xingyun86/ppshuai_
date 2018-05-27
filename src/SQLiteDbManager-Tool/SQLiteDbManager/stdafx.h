
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

//#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#if !defined(_DEBUG) && !defined(DEBUG)
//如果是RELEASE选项

#if !defined(_UNICODE) && !defined(UNICODE)
//如果是RELEASE选项下的ANSI选项
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#pragma comment(lib, "nafxcw.lib")
#else
//如果是RELEASE选项下的UNICODE选项
#pragma comment(linker,"/ENTRY:wWinMainCRTStartup")
#pragma comment(lib, "uafxcw.lib")
#endif

#pragma comment(lib, "libcmt.lib")

#else
//如果是DEBUG选项

#if !defined(_UNICODE) && !defined(UNICODE)
//如果是DEBUG选项下的ANSI选项
#pragma comment(linker,"/ENTRY:WinMainCRTStartup")
#pragma comment(lib, "nafxcwd.lib")
#else
//如果是DEBUG选项下的UNICODE选项
//#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
#pragma comment(linker,"/ENTRY:wWinMainCRTStartup")
#pragma comment(lib, "uafxcwd.lib")
#endif

#pragma comment(lib, "libcmtd.lib")

#endif

//#pragma comment(linker,"/verbose:lib")
//#pragma comment(linker, "/nodefaultlib")

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#define SQLITE3_WINDOW_TITLE TEXT("Sqlite3数据库管理工具")

//数据库路径
static _TCHAR G_DB_PATH[MAX_PATH] = { 0 };

#include <vector>
#include <map>
#if !defined(_UNICODE) && !defined(UNICODE)
#define tstring	std::string
#else
#define tstring	std::wstring
#endif

#define min_val(a,b) (((a) < (b)) ? (a) : (b))
#define max_val(a,b) (((a) > (b)) ? (a) : (b))

typedef std::vector<int> IntegerVector;
typedef std::vector<int>::iterator IntegerVectorIt;

typedef std::vector<tstring> StringVector;
typedef std::vector<tstring>::iterator StringVectorIt;
typedef std::vector<StringVector> StringVectorVector;
typedef std::vector<StringVector>::iterator StringVectorVectorIt;

//添加sqlite3库支持
#include <sqlitedb.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <iostream>
__inline BOOL InitConsoleWindow()
{ 
	BOOL bRet = FALSE;

	if(AllocConsole())
	{
		//Retrieves a handle to the specified standard device (standard input, standard output, or standard error). 
		//Associates a C run-time file descriptor with an existing operating-system file handle. 
		int hIN = _open_osfhandle((intptr_t)::GetStdHandle (STD_INPUT_HANDLE), _O_TEXT | _O_BINARY);
		int hOUT = _open_osfhandle((intptr_t)::GetStdHandle (STD_OUTPUT_HANDLE), _O_TEXT | _O_BINARY);
		int hERR = _open_osfhandle((intptr_t)::GetStdHandle (STD_ERROR_HANDLE), _O_TEXT | _O_BINARY); 
		// When a program opens a file, the operating system returns a corresponding file descriptor that the program refers to 
		// in order to process the file. A file descriptor is a low positive integer. The first three file descriptors (0,1, and 2,) 
		// are associated with the standard input (stdin), the standard output (stdout), and the standard error (stderr), respectively. 
		// Thus, the function scanf() uses stdin and the function printf() uses stdout. You can override the default setting and 
		// re-direct the process's I/O to different files by using different file descriptors: 
		// #include <cstdio> 
		// fprintf(stdout, "writing to stdout"); //write to stdout instead of a physical file 
		*stdin = *_tfdopen(hIN, TEXT("rb"));
		*stdout = *_tfdopen(hOUT, TEXT("wb"));
		*stderr = *_tfdopen(hERR, TEXT("wb"));
		std::ios_base::sync_with_stdio();
		bRet = TRUE;
	}

	return bRet;
}

__inline void ExitConsoleWindow()
{
	if (stdin)
	{
		fclose(stdin);
	}
	if (stdout)
	{
		fclose(stdout);
	}
	if (stderr)
	{
		fclose(stderr);
	}

	FreeConsole();
}


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


