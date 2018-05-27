#include "stdafx.h"
#include "CommonWindow.h"


INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

CCommonWindow::CCommonWindow(HINSTANCE hInstance, HWND hOwnerWnd, DLGPROC hDlgProc)
{
	m_hInstance = hInstance;
	m_hOwnerWnd = hOwnerWnd;
	m_hDlgProc = hDlgProc;
}

CCommonWindow::~CCommonWindow()
{
}

WORD * WordOffsetAlign(WORD * pVal)
{
	ULONG ulVal = (ULONG)pVal;
	ulVal += 3;
	ulVal >>= 2;
	ulVal <<= 2;
	return (WORD *)ulVal;
}

_TCHAR nEditTwo[MAX_CW_STR] = _T("从这儿输入字符串.");

INT_PTR CALLBACK CCommonWindow::DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, _T("我的对话框"));
		SetDlgItemText(hDlg, IDCW_EDIT, nEditTwo);
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDCW_EDIT:
		{
			GetDlgItemText(hDlg, IDCW_EDIT, nEditTwo, 127);
		}
		return TRUE;
		case IDCW_HELP:
		{
			MessageBox(NULL, nEditTwo, _T("输入数据"), MB_OK | MB_SYSTEMMODAL | MB_NOFOCUS);
		}
		return TRUE;
		case IDOK:
		case IDCANCEL:
		{
			EndDialog(hDlg, 0);
		}
		return TRUE;
		}
	}
	break;
	default:
		break;
	}
	return FALSE;
}

void DefineDialog(WORD ** ppwOffsetValue, DLGTEMPLATE * pInputDlgTemplate, _TCHAR * ptText)
{
	_TCHAR * ptTextData = 0;
	int nBytesofCharacter = 0;
	DLGTEMPLATE * pDlgTemplate = 0;
	WCHAR * pwText = 0;

	// Define a dialog box.
	(*ppwOffsetValue) = WordOffsetAlign((*ppwOffsetValue));
	pDlgTemplate = (DLGTEMPLATE *)(*ppwOffsetValue);

	memcpy_s(pDlgTemplate, sizeof(DLGTEMPLATE), pInputDlgTemplate, sizeof(DLGTEMPLATE));

	(*ppwOffsetValue) = (WORD *)(pDlgTemplate + 1);
	*(*ppwOffsetValue)++ = 0;   // no menu
	*(*ppwOffsetValue)++ = 0;   // predefined dialog box class (by default)

	ptTextData = (_TCHAR *)(*ppwOffsetValue);
#if !defined(_UNICODE) && !defined(UNICODE)
	pwText = GB2312_MBtoWC(ptText);
#else
	pwText = ptText;
#endif
	nBytesofCharacter = size_of_mem((BYTE *)pwText, sizeof(WCHAR));
	if (nBytesofCharacter)
	{
		memcpy_s(ptTextData, nBytesofCharacter * sizeof(WCHAR), pwText, nBytesofCharacter * sizeof(WCHAR));
	}
#if !defined(_UNICODE) && !defined(UNICODE)
	MWFree((void **)&pwText);
#else
	//no op
#endif
	(*ppwOffsetValue) += nBytesofCharacter + 1;
}

void DefineControl(WORD ** ppwOffsetValue, DLGITEMTEMPLATE * pInputDlgItemTemplate, _TCHAR * ptText, int nType)
{
	_TCHAR * ptTextData = 0;
	int nBytesofCharacter = 0;
	DLGITEMTEMPLATE * pDlgItemTemplate = 0;
	WCHAR * pwText = 0;

	//-----------------------
	// Define an OK button.
	//-----------------------
	(*ppwOffsetValue) = WordOffsetAlign((*ppwOffsetValue));

	pDlgItemTemplate = (DLGITEMTEMPLATE *)(*ppwOffsetValue);

	memcpy_s(pDlgItemTemplate, sizeof(DLGITEMTEMPLATE), pInputDlgItemTemplate, sizeof(DLGITEMTEMPLATE));

	(*ppwOffsetValue) = (WORD *)(pDlgItemTemplate + 1);
	*(*ppwOffsetValue)++ = 0xFFFF;
	if (nType > CTRLTYPE_NULL && nType < CTRLTYPE_MAXIMUM)
	{
		*(*ppwOffsetValue)++ = BASE_CTRL_VALUE + nType;

		ptTextData = (_TCHAR *)(*ppwOffsetValue);
#if !defined(_UNICODE) && !defined(UNICODE)
		pwText = GB2312_MBtoWC(ptText);
#else
		pwText = ptText;
#endif
		nBytesofCharacter = size_of_mem((BYTE *)pwText, sizeof(WCHAR));
		if (nBytesofCharacter)
		{
			memcpy_s(ptTextData, nBytesofCharacter * sizeof(WCHAR), pwText, nBytesofCharacter * sizeof(WCHAR));
		}
#if !defined(_UNICODE) && !defined(UNICODE)
		MWFree((void **)&pwText);
#else
		//no op
#endif
		(*ppwOffsetValue) += nBytesofCharacter + 1;
	}	
	
	*(*ppwOffsetValue)++ = 0;              // no creation data
}

void * InitDlgParam()
{
	HGLOBAL hGlobal = 0;
	DLGTEMPLATE * pDlgTemplate = 0;
	DLGITEMTEMPLATE * pDlgItemTemplate = 0;
	WORD * pwOffsetValue = 0;
	_TCHAR * ptTextData = 0;
	int nBytesofCharacter = 0;

	hGlobal = GlobalAlloc(GMEM_ZEROINIT, MAX_CW_MEM);
	if (hGlobal)
	{
		pDlgTemplate = (DLGTEMPLATE *)GlobalLock(hGlobal);
		pwOffsetValue = (WORD *)pDlgTemplate;

		// Define a dialog.
		DLGTEMPLATE dtDlg = {
			DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU,
			0, 3, 10, 10, 100, 70
		};
		DefineDialog(&pwOffsetValue, &dtDlg, _T("Dialog"));

		//-----------------------
		// Define an OK button.
		//-----------------------

		DLGITEMTEMPLATE ditExitButton = {
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			0, 10, 50, 80, 15, IDOK
		};

		DefineControl(&pwOffsetValue, &ditExitButton, _T("Button"), CTRLTYPE_BUTTON);

		//-----------------------
		// Define a Help button.
		//-----------------------
		DLGITEMTEMPLATE ditHelpButton = {
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			0, 10, 30, 80, 15, IDCW_HELP
		};

		DefineControl(&pwOffsetValue, &ditHelpButton, _T("Button"), CTRLTYPE_BUTTON);

		//-----------------------
		// Define a EDIT.
		//-----------------------
		DLGITEMTEMPLATE ditEditButton = {
			ES_LEFT | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE,
			0, 10, 10, 80, 12, IDCW_EDIT
		};

		DefineControl(&pwOffsetValue, &ditEditButton, _T("Edit"), CTRLTYPE_EDIT);		
	}

	return hGlobal;
}

void ExitDlgParam(void ** p)
{
	if (p && *p)
	{
		GlobalUnlock(*p);
		GlobalFree(*p);
		(*p) = 0;
	}
}

UINT_PTR ShowDialog(HINSTANCE hInstance, HWND hOwnerWnd, DLGTEMPLATE *pDlgTemplate, DLGPROC dlgProc, BOOL bModal)
{
	UINT_PTR iResult = 0;

	if (pDlgTemplate)
	{
		if (bModal)
		{
			//模态对话框
			iResult = DialogBoxIndirect(hInstance, pDlgTemplate, hOwnerWnd, dlgProc);
		}
		else
		{
			//非模态对话框
			HWND hWnd = CreateDialogIndirect(hInstance, pDlgTemplate, hOwnerWnd, dlgProc);
			if (hWnd)
			{
				iResult = ::ShowWindow(hWnd, SW_SHOW);
			}
		}
	}
	
	ExitDlgParam((void **)&pDlgTemplate);

	return iResult;
}

void CCommonWindow::SetDlgProcCallBack(DLGPROC hDlgProc)
{
	m_hDlgProc = hDlgProc;
}

INT_PTR CCommonWindow::DoModal()
{
	INT_PTR iResult = (-1);
	DLGTEMPLATE *pDlgTemplate = (DLGTEMPLATE *)InitDlgParam();
	if (pDlgTemplate)
	{
		iResult = ShowDialog(m_hInstance, m_hOwnerWnd, pDlgTemplate, m_hDlgProc, TRUE);
	}	

	return iResult;
}

INT_PTR CCommonWindow::DoModalLess()
{
	INT_PTR iResult = (-1);
	DLGTEMPLATE *pDlgTemplate = (DLGTEMPLATE *)InitDlgParam();
	if (pDlgTemplate)
	{
		iResult = ShowDialog(m_hInstance, m_hOwnerWnd, pDlgTemplate, m_hDlgProc, FALSE);
	}

	return iResult;
}