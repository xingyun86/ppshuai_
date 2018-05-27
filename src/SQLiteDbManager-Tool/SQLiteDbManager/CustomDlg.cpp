// CustomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SQLiteDbManager.h"
#include "SQLiteDbManagerDlg.h"
#include "CustomDlg.h"
#include "afxdialogex.h"


// CCustomDlg dialog

IMPLEMENT_DYNAMIC(CCustomDlg, CDialogEx)

CCustomDlg::CCustomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomDlg::IDD, pParent)
{

}

CCustomDlg::CCustomDlg(tstring tsTableName, StringVector * psvDesc, StringVector *psvData, 
	CWnd* pParent /*=NULL*/, BOOL bCreate/* = FALSE*/, INT nLineNumber/* = DEF_LINENUMBER*/)
	: CDialogEx(CCustomDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psvDesc = psvDesc;
	m_psvData = psvData;
	m_pSQLiteDbManagerDlg = (CSQLiteDbManagerDlg *)pParent;
	m_bCreate = bCreate;
	m_nLineNumber = nLineNumber;
	m_strTableName = tsTableName;
}

CCustomDlg::~CCustomDlg()
{
}

void CCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

size_t CCustomDlg::DynamicCreateGroupControl(size_t stStartIndex, size_t stCreateNumber, size_t stLineNumber, size_t &stLineIndex)
{
	CWnd *pWnd = NULL;
	DWORD dwStyle = 0;
	DWORD dwWndID = 0;
	CRect rect(0,0,0,0);

	//动态创建静态控件
	rect.top = BORDER_SPACE + stLineIndex * (STATIC_HEIGHT + EDIT_HEIGHT);
	rect.bottom = rect.top + STATIC_HEIGHT;
	for (size_t stIdx = stStartIndex; stIdx < stCreateNumber; stIdx++)
	{
		pWnd = new CWnd();
		if (pWnd)
		{
			rect.left = BORDER_SPACE + stIdx * (STATIC_WIDTH + SPACE_WIDTH);
			rect.right = rect.left + STATIC_WIDTH;

			dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER;
			dwWndID = STATIC_ID + stLineNumber * stLineIndex + stIdx;

			pWnd->Create(WC_STATIC, m_psvDesc->at(stIdx + stLineNumber * stLineIndex).c_str(), dwStyle, rect, this, dwWndID);
			m_wmStatic.insert(MAKE_PAIR(dwWndID, pWnd));
		}
	}

	//动态创建编辑框控件
	rect.top = BORDER_SPACE + stLineIndex * (STATIC_HEIGHT + EDIT_HEIGHT) + STATIC_HEIGHT;
	rect.bottom = rect.top + EDIT_HEIGHT;
	for (size_t stIdx = stStartIndex; stIdx < stCreateNumber; stIdx++)
	{		
		pWnd = new CWnd();
		if (pWnd)
		{
			rect.left = BORDER_SPACE + stIdx * (STATIC_WIDTH + SPACE_WIDTH);		
			rect.right = rect.left + EDIT_WIDTH;
			if (stIdx == 0 && stLineIndex == 0 && !m_bCreate)
			{
				dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_READONLY | WS_DISABLED | ES_CENTER | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN;
			}
			else
			{
				dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN;
			}
			dwWndID = EDIT_ID + stLineNumber * stLineIndex + stIdx;
			pWnd->Create(WC_EDIT, m_psvData->at(stIdx + stLineNumber * stLineIndex).c_str(), dwStyle, rect, this, dwWndID);
			m_wmEdit.insert(MAKE_PAIR(dwWndID, pWnd));
		}
	}

	if(stCreateNumber)
	{
		stLineIndex += 1;//行数加一
	}

	return stLineIndex;
}

// CCustomDlg message handlers


BOOL CCustomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO:  Add extra initialization here
	CRect rect(0, 0, 0, 0);
	size_t stLineIndex = 0;
	size_t stControlNumber = m_psvData->size();
	int TITLEBAR_HEIGHT = GetSystemMetrics(SM_CYCAPTION);

	if (m_nLineNumber > 0 && stControlNumber > 0)
	{
		if (m_nLineNumber > DEFAULT_LINENUMBER)
		{
			m_nLineNumber = DEFAULT_LINENUMBER;
		}
		for (size_t nIdx = 0; nIdx < stControlNumber / m_nLineNumber; nIdx++)
		{
			stLineIndex = DynamicCreateGroupControl(0, m_nLineNumber, m_nLineNumber, stLineIndex);
		}
		stLineIndex = DynamicCreateGroupControl(0, stControlNumber % m_nLineNumber, m_nLineNumber, stLineIndex);

		if (stLineIndex > 1)
		{
			m_nLineNumber = min_val(stControlNumber, (size_t)m_nLineNumber);
		}

		rect.left = rect.top = 0;
		rect.right = m_nLineNumber * (STATIC_WIDTH + SPACE_WIDTH) + BORDER_SPACE * 2;
		rect.bottom = (STATIC_HEIGHT + EDIT_HEIGHT) * stLineIndex + BORDER_SPACE * 2;

		CRect rcButton(0,0,0,0);
		rcButton.left = (rect.right - SPACE_WIDTH - 2 * BUTTON_WIDTH) / 2;
		rcButton.top = rect.bottom;
		rcButton.right = rcButton.left + BUTTON_WIDTH;
		rcButton.bottom = rcButton.top + BUTTON_HEIGHT;
		m_ButtonConfirm.Create(TEXT("Button"), TEXT("确认修改(&Y)"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, rcButton, this, IDOK);

		rcButton.left = (rect.right - SPACE_WIDTH - 2 * BUTTON_WIDTH) / 2 + BUTTON_WIDTH + SPACE_WIDTH;
		rcButton.top = rect.bottom;
		rcButton.right = rcButton.left + BUTTON_WIDTH;
		rcButton.bottom = rcButton.top + BUTTON_HEIGHT;
		m_ButtonCancel.Create(WC_BUTTON, TEXT("关闭窗口(&N)"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, rcButton, this, IDCANCEL);

		//计算主窗口宽度和高度
		rect.right += 2 * BORDER_SPACE + SPACE_WIDTH;
		rect.bottom += BUTTON_HEIGHT + BORDER_SPACE + SPACE_HEIGHT + TITLEBAR_HEIGHT + 2 * SPACE_HEIGHT + BORDER_SPACE;

		SetWindowPos(&CWnd::wndNoTopMost, 0, 0, rect.right, rect.bottom, SWP_NOMOVE);

		if (m_bCreate)
		{
			m_ButtonConfirm.SetWindowText(TEXT("确认添加(&Y)"));
			SetWindowText(TEXT("新增数据字段信息"));
		}
		else
		{
			m_ButtonConfirm.SetWindowText(TEXT("确认修改(&Y)"));
			SetWindowText(TEXT("修改数据字段信息"));
		}
	}
	else
	{
		EndDialog(NULL);
	}

	return FALSE;//TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCustomDlg::ReleaseControl()
{
	// TODO: Add your message handler code here and/or call default
	WndMapIt it;

	for (it = m_wmStatic.begin(); it != m_wmStatic.end(); it++)
	{
		if (it->second)
		{
			delete (CStatic *)it->second;
			it->second = NULL;
		}
	}

	for (it = m_wmEdit.begin(); it != m_wmEdit.end(); it++)
	{
		if (it->second)
		{
			delete (CEdit *)it->second;
			it->second = NULL;
		}
	}
}


void CCustomDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRet = FALSE;
	size_t stStartIndex = 0;
	tstring tsFirst = TEXT("");
	tstring tsResult = TEXT("");
	tstring tsSqlCmd = TEXT("");	
	CSQLiteDatabase * l_pSQLiteDb = (CSQLiteDatabase *)m_pSQLiteDbManagerDlg->GetSQLiteDatabase();
	m_svData.clear();
	
	if(m_bCreate)
	{
		tsSqlCmd = TEXT("INSERT INTO ");
		tsSqlCmd.append(m_strTableName.c_str()).append(TEXT(" VALUES('"));
		stStartIndex = 0;
	}
	else
	{
		tsSqlCmd = TEXT("UPDATE ");
		tsSqlCmd.append(m_strTableName.c_str()).append(TEXT(" SET "));
		stStartIndex = 1;
		tsFirst = m_psvData->at(0);
		m_svData.push_back(tsFirst.c_str());
	}
	
	for (size_t stIdx = stStartIndex; stIdx < m_psvDesc->size(); stIdx++)
	{
		CString cs(TEXT(""));
		GetDlgItemText(EDIT_ID + stIdx, cs);
		m_svData.push_back(tstring((LPCTSTR)cs));
		if (m_bCreate)
		{
			tsSqlCmd.append((LPCTSTR)cs).append(TEXT("','"));
		}
		else
		{
			tsSqlCmd.append(m_psvDesc->at(stIdx).c_str()).append(TEXT("='")).append((LPCTSTR)cs).append(TEXT("',"));
		}		
	}
	if (m_bCreate)
	{
		size_t npos = tsSqlCmd.rfind(TEXT("','"));
		if (npos != tstring::npos)
		{
			tsSqlCmd.replace(npos, 3, TEXT("')"));
		}
	}
	else
	{
		size_t npos = tsSqlCmd.rfind(TEXT("',"));
		if (npos != tstring::npos)
		{
			tsSqlCmd.replace(npos, 2, TEXT("' "));
		}
	
		tsSqlCmd.append(TEXT("WHERE ")).append(m_psvDesc->at(0).c_str()).append(TEXT("='")).append(tsFirst.c_str()).append(TEXT("'"));
	}
	bRet = l_pSQLiteDb->ExecuteCommand(tsSqlCmd);
	if(bRet)
	{
		CListCtrl * l_pListCtrl = (CListCtrl *)m_pSQLiteDbManagerDlg->GetDlgItem(IDC_LIST_TABLEVIEW);
		if (l_pListCtrl)
		{
			if (m_bCreate)
			{
				size_t stIdx = 0;
				int nItemCount = l_pListCtrl->GetItemCount();
				l_pListCtrl->InsertItem(nItemCount, m_svData.at(stIdx).c_str());
				for (stIdx = 1; stIdx < m_psvDesc->size(); stIdx++)
				{
					l_pListCtrl->SetItemText(nItemCount, stIdx, m_svData.at(stIdx).c_str());
				}
				m_pSQLiteDbManagerDlg->InsertRecord(m_svData);
			}
			else
			{
				POSITION pt = l_pListCtrl->GetFirstSelectedItemPosition();
				if (pt)
				{
					int nItem = l_pListCtrl->GetNextSelectedItem(pt);
					if (nItem != -1)
					{
						for (size_t stIdx = 1; stIdx < m_svData.size(); stIdx++)
						{
							l_pListCtrl->SetItemText(nItem, stIdx, m_svData.at(stIdx).c_str());
						}
					}
					m_pSQLiteDbManagerDlg->UpdateRecord(m_svData);
				}
			}
		}
		if (m_bCreate)
		{
			tsResult = TEXT("新增记录成功");
		}
		else
		{
			tsResult = TEXT("修改记录成功");
		}
		
		m_pSQLiteDbManagerDlg->UpdateStatusBarInfo();

		::MessageBox(this->GetSafeHwnd(), tsResult.c_str(), TEXT("提示"), MB_ICONINFORMATION);

		ReleaseControl();

		CDialogEx::OnOK();
	}
	else
	{
		if (m_bCreate)
		{
			tsResult.append(TEXT("新增记录失败!\r\n错误原因: ")).append(l_pSQLiteDb->GetLastError());
		}
		else
		{
			tsResult.append(TEXT("修改记录失败!\r\n错误原因: ")).append(l_pSQLiteDb->GetLastError());
		}
		::MessageBox(this->GetSafeHwnd(), tsResult.c_str(), TEXT("提示"), MB_ICONERROR);
		
	}	
}


void CCustomDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	
	ReleaseControl();

	//CDialogEx::OnCancel();
	EndDialog(NULL);
}
