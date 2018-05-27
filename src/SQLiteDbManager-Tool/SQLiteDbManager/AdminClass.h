// AdminClass.h: interface for the CAdminClass class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class AFX_EXT_CLASS CAdminClass : public CObject  
{
public:
	CAdminClass(TCHAR * pDbFileName = TEXT(""), TCHAR * pTableName = TEXT(""));
	virtual ~CAdminClass();

public:
	void SetDbFileName(TCHAR * pDbFileName);
	void SetTableName(TCHAR * pTableName);
	INT_PTR DisplayDialog();
};
