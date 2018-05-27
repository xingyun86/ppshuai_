// AdminClass.cpp: implementation of the CAdminClass class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLiteDbManager.h"
#include "SQLiteDbManagerDlg.h"
#include "AdminClass.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

tstring g_strDbFileName = TEXT("");
tstring g_strTableName = TEXT("");

CAdminClass::CAdminClass(TCHAR * pDbFileName/* = TEXT("")*/, TCHAR * pTableName/* = TEXT("")*/)
{
	if (!pDbFileName)
	{
		g_strDbFileName = TEXT("");
	}
	else
	{
		g_strDbFileName = pDbFileName;
	}
	if (!pTableName)
	{
		g_strTableName = TEXT("");
	}
	else
	{
		g_strTableName = pTableName;
	}	
}

CAdminClass::~CAdminClass()
{
}

void SetDbFileName(TCHAR * pDbFileName)
{
	g_strDbFileName = pDbFileName ? pDbFileName : TEXT("");
}

void SetTableName(TCHAR * pTableName)
{
	g_strTableName = pTableName ? pTableName : TEXT("");
}

INT_PTR CAdminClass::DisplayDialog()
{
	if (g_strTableName.length())
	{
		CSQLiteDbManagerDlg dlg(g_strDbFileName, g_strTableName);	

		return dlg.DoModal();
	}
	return 0;
}