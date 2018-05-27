#ifndef __COMMONWINDOW_H
#define __COMMONWINDOW_H

#pragma once

#define MAX_CW_STR	128
#define MAX_CW_MEM	1024

#define IDCW_HELP   150
#define IDCW_EDIT	180
#define IDCW_TEXT   200

#define BASE_CTRL_VALUE 0x007F
typedef enum {
	CTRLTYPE_NULL = 0,
	CTRLTYPE_BUTTON,
	CTRLTYPE_EDIT,
	CTRLTYPE_STATIC,
	CTRLTYPE_LISTBOX,
	CTRLTYPE_SCROLLBAR,
	CTRLTYPE_COMBOBOX,
	CTRLTYPE_MAXIMUM
}CONTROL_TYPE;

class CCommonWindow
{
public:
	CCommonWindow();
	CCommonWindow(HINSTANCE hInstance, HWND hOwnerWnd, DLGPROC hDlgProc);
	~CCommonWindow();

	virtual INT_PTR DoModal();
	virtual INT_PTR DoModalLess();
protected:

	// API中注册的消息处理函数，不能是成员函数，因为成员函数有this指针
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE m_hInstance;
	HWND  m_hOwnerWnd;
	DLGPROC m_hDlgProc;
public:
	void SetDlgProcCallBack(DLGPROC hDlgProc);
};


//根据当前设定的编码类型计算内存占用字符个数（WCHAR基数为2，CHAR基数为1）
__inline int size_of_mem(BYTE * p, int nCodeType)
{
	int n = 0;

	if (nCodeType > 0)
	{
		while (p)
		{
			n++;
			if (!*(p + nCodeType))
			{
				break;
			}			
			else
			{
				p += nCodeType;
			}		
		}
	}

	return n;
}

__inline WCHAR * MTW(UINT uiCodePage, CHAR *pM)
{
	WCHAR *pW = NULL;
	int nW = MultiByteToWideChar(uiCodePage, NULL, pM, -1, NULL, NULL);
	if (nW)
	{
		pW = (WCHAR *)LocalAlloc(LMEM_ZEROINIT, (nW + 1) * sizeof(WCHAR));
		if (pW)
		{
			MultiByteToWideChar(uiCodePage, NULL, pM, -1, pW, nW);
		}
	}
	return pW;
}
__inline CHAR * WTM(UINT uiCodePage, WCHAR *pW)
{
	CHAR *pM = NULL;
	int nMLen = WideCharToMultiByte(uiCodePage, NULL, pW, -1, NULL, NULL, NULL, NULL);
	if (nMLen)
	{
		pM = (CHAR *)LocalAlloc(LMEM_ZEROINIT, (nMLen + 1) * sizeof(CHAR));
		if (pM)
		{
			WideCharToMultiByte(uiCodePage, NULL, pW, -1, pM, nMLen, NULL, NULL);
		}
	}
	return pM;
}
__inline WCHAR * UTF8_MBtoWC(CHAR *pM)
{
	return MTW(CP_UTF8, pM);
}
__inline CHAR * UNICODE_WCtoMB(WCHAR *pW)
{
	return WTM(CP_UTF8, pW);
}
__inline WCHAR * GB2312_MBtoWC(CHAR *pM)
{
	return MTW(CP_ACP, pM);
}
__inline CHAR * GB2312_WCtoMB(WCHAR *pW)
{
	return WTM(CP_ACP, pW);
}
__inline void MWFree(void ** pV)
{
	if (*pV)
	{
		LocalFree((*pV));
		(*pV) = 0;
	}
}
#endif//__COMMONWINDOW_H