
// SQLiteDbManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SQLiteDbManager.h"
#include "SQLiteDbManagerDlg.h"
#include "afxdialogex.h"
#include "sqlitedb.h"
#include "CustomDlg.h"
#include "CommonWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSQLiteDbManagerDlg dialog




CSQLiteDbManagerDlg::CSQLiteDbManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSQLiteDbManagerDlg::IDD, pParent),
	m_pComboBoxFinderFilter(NULL),
	m_pButtonFinder(NULL),
	m_pButtonFilter(NULL),
	m_pButtonCreate(NULL),
	m_pButtonQuit(NULL),
	m_pListCtrl(NULL),
	m_pStaticStatusBar(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDbFileName = _T("");
	m_strTableName = _T("");
}

CSQLiteDbManagerDlg::CSQLiteDbManagerDlg(tstring strDbFileName, tstring strTableName, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSQLiteDbManagerDlg::IDD, pParent),
	m_pComboBoxFinderFilter(NULL),
	m_pButtonFinder(NULL),
	m_pButtonFilter(NULL),
	m_pButtonCreate(NULL),
	m_pButtonQuit(NULL),
	m_pListCtrl(NULL),
	m_pStaticStatusBar(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDbFileName = strDbFileName;
	m_strTableName = strTableName;
}
CSQLiteDbManagerDlg::~CSQLiteDbManagerDlg()
{

}

void CSQLiteDbManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSQLiteDbManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TABLEVIEW, &CSQLiteDbManagerDlg::OnNMRClickListTableview)
	ON_COMMAND(IDM_TRACK_UPDATE, OnTrackUpdate)
	ON_COMMAND(IDM_TRACK_DELETE, OnTrackDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TABLEVIEW, &CSQLiteDbManagerDlg::OnNMDblclkListTableview)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_TABLEVIEW, &CSQLiteDbManagerDlg::OnLvnColumnClickListTableview)
	ON_BN_CLICKED(IDC_BUTTON_FINDER, &CSQLiteDbManagerDlg::OnBnClickedButtonFinder)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, &CSQLiteDbManagerDlg::OnBnClickedButtonFilter)
	ON_CBN_EDITUPDATE(IDC_COMBO_FINDERFILTER, &CSQLiteDbManagerDlg::OnCBNEditUpdateComboFinderFilter)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


//新建列
BOOL InsertColumnData(CListCtrl * pListCtrl, StringVector * psv, BOOL bErase = TRUE)
{
	BOOL bRet = FALSE;
	if (pListCtrl)
	{
		if (bErase)
		{
			//删除所有行
			pListCtrl->DeleteAllItems();

			//删除所有列
			while (pListCtrl->DeleteColumn(0));
		}

		if (psv)
		{			
			LVCOLUMN lvc = {0};
			size_t stSize = psv->size();
			
			lvc.mask = LVCF_WIDTH | LVCF_TEXT;
			lvc.cx = 100;
			lvc.cxMin = lvc.cx;
			for (size_t stColIdx = 0; stColIdx < stSize; stColIdx++)
			{
				lvc.pszText = (LPTSTR)psv->at(stColIdx).c_str();
				pListCtrl->InsertColumn(stColIdx, &lvc);
			}
			bRet = TRUE;
		}
	}
	return bRet;
}

//新增数据
BOOL InsertItemData(CListCtrl * pListCtrl, StringVectorVector * psvv, BOOL bErase = FALSE)
{
	BOOL bRet = FALSE;
	if (pListCtrl && psvv)
	{
		LVITEM lvi = {0};
		size_t stSize = psvv->size();		
		size_t stNumX = 0;
		size_t stNumY = 0;
		size_t stNum = 0;
		CHeaderCtrl * pHeader = pListCtrl->GetHeaderCtrl();
		
		if (bErase)
		{
			//删除所有行
			pListCtrl->DeleteAllItems();
		}

		if (pHeader)
		{
			stNumX = pHeader->GetItemCount();
		}

		lvi.mask = LVIF_TEXT;
		for (size_t stRowIdx = 0; stRowIdx < stSize; stRowIdx++)
		{
			size_t stColIdx = 0;
			lvi.iItem = stRowIdx;
			lvi.iSubItem = stColIdx;
			lvi.pszText = (LPTSTR)psvv->at(stRowIdx).at(stColIdx).c_str();
			pListCtrl->InsertItem(&lvi);
			stNumY = psvv->at(stRowIdx).size();
			stNum = min_val(stNumX, stNumY);
			for (stColIdx = 1; stColIdx < stNum; stColIdx++)
			{
				lvi.iSubItem = stColIdx;
				lvi.pszText = (LPTSTR)psvv->at(stRowIdx).at(stColIdx).c_str();
				pListCtrl->SetItem(&lvi);
			}
			pListCtrl->SetItemData(lvi.iItem, lvi.iItem);
		}

		bRet = TRUE;
	}

	return bRet;	
}

//打印一维字符串向量集合信息
void PrintStringVector(StringVector * psv)
{
	if (psv)
	{
		for (size_t stIdx = 0; stIdx < psv->size(); stIdx++)
		{
			_tprintf(_T("%s\n"), psv->at(stIdx).c_str());				
		}
		_tprintf(_T("===================================================\n"));	
	}
}
//打印二维字符串向量集合信息
void PrintStringVectorVector(StringVectorVector * psvv, StringVector * psv = NULL)
{
	if (psvv)
	{
		size_t stSize = psvv->size();
		size_t stNum = 0;
		size_t stNumX = 0;
		size_t stNumY = 0;
		if (psv)
		{
			stNumX = psv->size();
		}
		for (size_t stIdx = 0; stIdx < stSize; stIdx++)
		{
			stNumY = psvv->at(stIdx).size();
			stNum = min_val(stNumX, stNumY);
			for (size_t stIdy = 0; stIdy < stNum; stIdy++)
			{
				tstring ts(_T(""));
				if (stNum != stNumX)
				{
					ts = psv->at(stIdy);
				}
				_tprintf(_T("%s:%s\n"), ts.c_str(), psvv->at(stIdx).at(stIdy).c_str());
			}				
			_tprintf(_T("===================================================\n"));				
		}
	}

}

INT InitImageListColumnHeader(CListCtrl * pListCtrl, CImageList *pImageList, 
	UINT nImageId, int nWidth = 16, int nGrow = 1, COLORREF clrRGB = RGB(255, 255, 255))
{
	INT nImageCount = 0;
	if (pListCtrl && pImageList && pListCtrl->GetHeaderCtrl())
	{
		if(pImageList->Create(nImageId, nWidth, nGrow, clrRGB) && 
			pListCtrl->GetHeaderCtrl()->SetImageList(pImageList))
		{
			nImageCount = pImageList->GetImageCount();
		}
	}	
	return nImageCount;
}

void SetColumnHeaderImage(CListCtrl * pListCtrl, IntegerVector * pivImageIdx)
{
	if (pListCtrl && pListCtrl->GetHeaderCtrl() && pivImageIdx)
	{
		// Change the item to ownder drawn
		HD_ITEM hditem = {0};		
		
		for (size_t stIdx = 0; stIdx < pivImageIdx->size(); stIdx++)
		{
			hditem.mask = HDI_FORMAT | HDI_IMAGE;
			pListCtrl->GetHeaderCtrl()->GetItem(stIdx, &hditem);
			hditem.fmt |= HDF_OWNERDRAW;
			hditem.iImage = pivImageIdx->at(stIdx);
			pListCtrl->GetHeaderCtrl()->SetItem(stIdx, &hditem);
		}
		pListCtrl->Invalidate();
		pListCtrl->UpdateWindow();
	}
}

void CSQLiteDbManagerDlg::UpdateStatusBarInfo()
{
	tstring ts = _T("");
	_TCHAR tCount[128] = {0};
	INT nItemCount = m_pListCtrl->GetItemCount();
	_sntprintf_s(tCount, sizeof(tCount), _T("%d"), nItemCount ? nItemCount : 0, sizeof(tCount));
	ts.append(_T("当前共有")).append(tCount).append(_T("条记录"));
	m_pStaticStatusBar->SetWindowText(ts.c_str());
}

size_t FindRecord(StringVectorVector &svv, StringVector sv, size_t stMatchIdx = 0)
{
	size_t stIdx = 0;
	if (stMatchIdx >= 0 && stMatchIdx < sv.size())
	{
		for (; stIdx < svv.size(); stIdx++)
		{
			if (0 == svv[stIdx].at(stMatchIdx).compare(sv.at(stMatchIdx)))
			{
				stIdx = stIdx + 1;
				break;
			}
		}
	}
	
	return stIdx >= svv.size() ? 0 : stIdx;
}

void CSQLiteDbManagerDlg::InsertRecord(StringVector sv)
{
	size_t stMatchIdx = FindRecord(m_svvDataItem, sv);
	if (stMatchIdx <= 0)
	{
		m_svvDataItem.push_back(sv);
	}
}
void CSQLiteDbManagerDlg::DeleteRecord(StringVector sv)
{
	size_t stMatchIdx = FindRecord(m_svvDataItem, sv);
	if (stMatchIdx > 0)
	{
		m_svvDataItem.erase(m_svvDataItem.begin() + stMatchIdx - 1);
	}
}
void CSQLiteDbManagerDlg::UpdateRecord(StringVector sv)
{
	size_t stMatchIdx = FindRecord(m_svvDataItem, sv);
	if (stMatchIdx > 0)
	{
		for (size_t stIdx = 0; stIdx < sv.size(); stIdx++)
		{
			m_svvDataItem[stMatchIdx - 1][stIdx] = sv.at(stIdx);
		}
	}
}

void SetStatusBarText(CStatic *pStatic, tstring strText)
{
	CString cs(_T(""));
	pStatic->GetWindowText(cs);
	pStatic->SetWindowText((LPCTSTR)(cs));
}

BOOL GetCommandRecordSets(StringVector * psvHeader, StringVectorVector * psvvItemData, CSQLiteDatabase * pSQLiteDb, tstring tsSQLCommand)
{
	BOOL bRet = FALSE;
	if (psvHeader && psvvItemData && pSQLiteDb && tsSQLCommand.length())
	{
		psvHeader->clear();
		psvvItemData->clear();
		if(pSQLiteDb->ExcuteQuery(tsSQLCommand))
		{
			pSQLiteDb->ExcuteQuery(tsSQLCommand);
			for (int nIdx = 0; nIdx < pSQLiteDb->GetFieldNum(); nIdx++)
			{
				tstring ts = _T("");
				pSQLiteDb->GetFieldName(nIdx, ts);
				psvHeader->push_back(ts);
			}
			while (pSQLiteDb->MoreRecord())
			{				
				StringVector sv;
				size_t stSize = psvHeader->size();
				for (size_t stIdx = 0; stIdx < stSize; stIdx++)
				{
					tstring ts = _T("");

					long lType = pSQLiteDb->GetFieldType(stIdx);
					switch(lType)
					{
					case SQLITE_INTEGER:
						{
							int nVal = 0;
							_TCHAR tVal[32] = { 0 };
							pSQLiteDb->GetFieldValue(stIdx, nVal);
							_sntprintf_s(tVal, sizeof(tVal), _T("%d"), nVal, sizeof(tVal));
							ts = tVal;
						}
						break;
					case SQLITE_FLOAT:
						{
							double dVal = 0.0f;
							_TCHAR tVal[128] = { 0 };
							pSQLiteDb->GetFieldValue(stIdx, dVal);
							_sntprintf_s(tVal, sizeof(tVal), _T("%.f"), dVal, sizeof(tVal));
							ts = tVal;
						}
						break;
					case SQLITE_TEXT:
						{
							pSQLiteDb->GetFieldValue(stIdx, ts);
						}
						break;
					case SQLITE_BLOB:
					case SQLITE_NULL:
					default:
						{
							//no op
						}
						break;
					}

					sv.push_back(ts);
				}
				psvvItemData->push_back(sv);
				pSQLiteDb->NextRecord();
			}
		}
		if(psvvItemData->size() > 0)
		{
			bRet = TRUE;
		}
	}
	return bRet;
}
BOOL GetTableRecordSets(StringVector * psvHeader, StringVectorVector * psvvItemData, CSQLiteDatabase * pSQLiteDb, tstring tsTableName)
{
	BOOL bRet = FALSE;
	tstring tsSql = _T("SELECT * FROM ");
	if (psvHeader && psvvItemData && pSQLiteDb && tsTableName.length())
	{
		tsSql.append(tsTableName.c_str());
		psvHeader->clear();
		psvvItemData->clear();
		if(pSQLiteDb->ExcuteQuery(tsSql))
		{
			for (int nIdx = 0; nIdx < pSQLiteDb->GetFieldNum(); nIdx++)
			{
				tstring ts = _T("");
				pSQLiteDb->GetFieldName(nIdx, ts);
				psvHeader->push_back(ts);
			}
			while (pSQLiteDb->MoreRecord())
			{				
				StringVector sv;
				size_t stSize = psvHeader->size();
				for (size_t stIdx = 0; stIdx < stSize; stIdx++)
				{
					tstring ts = _T("");

					long lType = pSQLiteDb->GetFieldType(stIdx);
					switch(lType)
					{
					case SQLITE_INTEGER:
						{
							__int64 nVal = 0;
							_TCHAR tVal[128] = { 0 };
							
							pSQLiteDb->GetFieldValue(stIdx, nVal);
							_sntprintf_s(tVal, sizeof(tVal), _T("%lld"), nVal, sizeof(tVal));
							ts = tVal;
						}
						break;
					case SQLITE_FLOAT:
						{
							double dVal = 0.0f;
							_TCHAR tVal[128] = { 0 };
							pSQLiteDb->GetFieldValue(stIdx, dVal);
							_sntprintf_s(tVal, sizeof(tVal), _T("%.f"), dVal, sizeof(tVal));
							ts = tVal;
						}
						break;
					case SQLITE_TEXT:
						{
							pSQLiteDb->GetFieldValue(stIdx, ts);
						}
						break;
					case SQLITE_BLOB:
					case SQLITE_NULL:
					default:
						{
							//no op
						}
						break;
					}

					sv.push_back(ts);
				}
				psvvItemData->push_back(sv);
				pSQLiteDb->NextRecord();
			}
		}
		
		bRet = TRUE;
	}
	return bRet;
}
// CSQLiteDbManagerDlg message handlers
//////////////////////////////////////////////////////////////////////////
//数据库表浏览窗口
StringVector * g_pTableNameList = NULL;
TCHAR g_tTableName[MAX_CW_STR] = _T("");
#define IDCW_LISTCTRL	1

//屏幕居中显示控件
void CenterDisplay(HWND hWnd, int nWidth, int nHeight)
{
	RECT rcWorkArea = { 0 };
	RECT rcWndRect = { 0, 0, nWidth, nHeight };

	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	rcWndRect.left = (rcWorkArea.right - rcWorkArea.left - nWidth) / 2;
	rcWndRect.top = (rcWorkArea.bottom - rcWorkArea.top - nHeight) / 2;
	::MoveWindow(hWnd, rcWndRect.left, rcWndRect.top, nWidth, nHeight, TRUE);
}
//屏幕居中显示控件
void CenterDisplayInParent(HWND hParentWnd, HWND hWnd, int nWidth, int nHeight)
{
	RECT rcParent = { 0 };
	RECT rcWndRect = { 0, 0, nWidth, nHeight };

	GetWindowRect(hParentWnd, &rcParent);
	rcWndRect.left = rcParent.left + (rcParent.right - rcParent.left - nWidth) / 2;
	rcWndRect.top = rcParent.top + (rcParent.bottom - rcParent.top - nHeight) / 2;
	::MoveWindow(hWnd, rcWndRect.left, rcWndRect.top, nWidth, nHeight, TRUE);
}

INT_PTR CALLBACK DbTable_DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			SetWindowText(hDlg, tstring(tstring(_T("[")) + (StrRStrI(G_DB_PATH, NULL, _T("\\")) + 1) + tstring(_T("].数据库表"))).c_str());
			HWND hListWnd = ::CreateWindowEx(LVS_EX_GRIDLINES, WC_LISTVIEW, _T(""), 
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT, 0, 0, 400, 300, hDlg, (HMENU)IDCW_LISTCTRL, NULL, NULL);
			if (hListWnd)
			{
				::MoveWindow(hListWnd, 0, 0, 400, 300, TRUE);
				::ShowWindow(hListWnd, SW_SHOW);
			}
			CenterDisplayInParent(GetParent(hDlg), hDlg, 400, 300);
			::ShowWindow(::GetDlgItem(hDlg, IDCW_EDIT), SW_HIDE);
			::ShowWindow(::GetDlgItem(hDlg, IDCW_HELP), SW_HIDE);
			::ShowWindow(::GetDlgItem(hDlg, IDOK), SW_HIDE);

				//////////////////////////////////////////////////////////////////////////
			//获取窗口指针
			CListCtrl * pListCtrl = (CListCtrl *)CWnd::FromHandle(hListWnd);
			
			//////////////////////////////////////////////////////////////////////////
			//设置ListCtrl风格
			//设置显示属性
			DWORD dwStyle = pListCtrl->GetStyle();
			DWORD dwExStyle = pListCtrl->GetExtendedStyle();
			pListCtrl->ModifyStyle(NULL, dwStyle | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
			pListCtrl->SetExtendedStyle(dwExStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERINALLVIEWS | LVS_EX_SUBITEMIMAGES);		

			//////////////////////////////////////////////////////////////////////////
			//初始化列表数据
			LVCOLUMN lvc = {0};
			
			lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
			lvc.fmt = LVCFMT_CENTER;
			lvc.pszText = _T("表名称");
			lvc.cx = 400;
			pListCtrl->InsertColumn(0, &lvc);
			
			pListCtrl->DeleteAllItems();
			if (g_pTableNameList)
			{
				for (size_t stIdx = 0; stIdx < g_pTableNameList->size(); stIdx++)
				{
					LVITEM lvi = {0};
					lvi.mask = LVIF_TEXT;
					lvi.pszText = (LPTSTR)g_pTableNameList->at(stIdx).c_str();
					lvi.iSubItem = 0;
					lvi.iItem = stIdx;
					pListCtrl->InsertItem(&lvi);
				}
			}
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//case IDCW_EDIT:
		//	SetDlgItemText(hDlg, IDCW_EDIT, _T("请输入您的字符串"));
		//	return TRUE;
		//case IDCW_HELP:
		//	MessageBox(NULL, _T("请输入您的字符串"), _T("输入数据"), MB_OK | MB_SYSTEMMODAL | MB_NOFOCUS);
		//	return TRUE;
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	case WM_NOTIFY:
		{
			NMHDR * pNMHDR = (NMHDR*)lParam;
			switch(pNMHDR->code)
			{
			case NM_DBLCLK:
				{
					switch(pNMHDR->idFrom)
					{
					case IDCW_LISTCTRL:
						{
							CListCtrl * pListCtrl = (CListCtrl *)CWnd::FromHandle(pNMHDR->hwndFrom);
							if (pListCtrl)
							{
								NM_LISTVIEW *pNMListView=(NM_LISTVIEW *)pNMHDR;
								int nItem = pNMListView->iItem;
								int nItemCount = pListCtrl->GetItemCount();
								if (nItem != (-1) && nItem < nItemCount)
								{
									LVITEM lvi = {0};
									
									lvi.mask = LVIF_TEXT;
									lvi.iItem = nItem;
									lvi.iSubItem = 0;
									lvi.pszText = g_tTableName;
									lvi.cchTextMax = sizeof(g_tTableName);
									pListCtrl->GetItem(&lvi);
									EndDialog(hDlg, IDOK);
								}
								else
								{
									
								}
							}
						}
						break;
					default:
						{

						}
						break;
					}
				}
				break;
			default:
				{

				}
				break;
			}			
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////

BOOL CSQLiteDbManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//添加菜单项					
	HMENU hMenu = ::GetSystemMenu(m_hWnd, FALSE);
	::AppendMenu(hMenu, MF_SEPARATOR, (UINT)0, NULL);
	::AppendMenu(hMenu, MF_STRING, (UINT)IDM_SELECT_DBFILE, _T("打开数据库"));
	::AppendMenu(hMenu, MF_STRING, (UINT)IDM_SELECT_DBTABLE, _T("选择数据表"));
	::SetMenu(m_hWnd, hMenu);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
		
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	SetWindowText(SQLITE3_WINDOW_TITLE);

	//////////////////////////////////////////////////////////////////////////
	// 初始化ListCtrl
	m_pComboBoxFinderFilter = (CComboBox *)GetDlgItem(IDC_COMBO_FINDERFILTER);
	m_pButtonFinder = (CButton *)GetDlgItem(IDC_BUTTON_FINDER);
	m_pButtonFilter = (CButton *)GetDlgItem(IDC_BUTTON_FILTER);
	m_pButtonCreate = (CButton *)GetDlgItem(IDOK);
	m_pButtonQuit = (CButton *)GetDlgItem(IDCANCEL);
	m_pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_TABLEVIEW);
	m_pStaticStatusBar = (CStatic *)GetDlgItem(IDC_STATIC_STATUS);

	SetDefID(IDC_BUTTON_FINDER);

	{
		//设置显示属性
		DWORD dwStyle = m_pListCtrl->GetStyle();
		DWORD dwExStyle = m_pListCtrl->GetExtendedStyle();
		m_pListCtrl->ModifyStyle(NULL, dwStyle | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
		m_pListCtrl->SetExtendedStyle(dwExStyle | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERINALLVIEWS | LVS_EX_SUBITEMIMAGES);		
		
		InitImageListColumnHeader(m_pListCtrl, &m_imgListHeader, IDB_BITMAP_HEADER);

		SetProp(m_pListCtrl->GetSafeHwnd(), _T("SORT_COLUMN"), (HANDLE)FALSE);
		SetProp(m_pListCtrl->GetSafeHwnd(), _T("SORT_DIRECTION"), (HANDLE)FALSE);

		ResizeWindow();
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CSQLiteDbManagerDlg::ShowTablesSelectDlg()
{
	tstring tsResult(_T(""));
	if (GetCommandRecordSets(&m_svHeader, &m_svvDataItem, &m_SQLiteDb, _T("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name")))
	{
		bool bFind = false;
		//测试对话框
		{
			if (!g_pTableNameList)
			{
				g_pTableNameList = new StringVector();
			}
			g_pTableNameList->clear();
			for (size_t stIdx = 0; stIdx < m_svvDataItem.size(); stIdx++)
			{
				g_pTableNameList->push_back(m_svvDataItem.at(stIdx).at(0));
			}

			CCommonWindow modalDlg(NULL, this->GetSafeHwnd(), &DbTable_DialogProc);
			INT_PTR nRet = modalDlg.DoModal();
			if (g_pTableNameList)
			{
				delete g_pTableNameList;
				g_pTableNameList = NULL;
			}
			if (nRet != IDOK)
			{
				m_SQLiteDb.Close();
				return;
			}
			if (!*g_tTableName)
			{
				EndDialog(NULL);
			}

			m_strTableName = g_tTableName;
		}
		for (size_t stIdx = 0; stIdx < m_svvDataItem.size(); stIdx++)
		{
			if (!m_strTableName.compare(m_svvDataItem.at(stIdx).at(0)))
			{
				bFind = true;
				break;
			}
		}
		if (!bFind)
		{
			tsResult = _T("");
			tsResult.append(m_strTableName.c_str()).append(_T("数据表不存在, 请检查打开的数据库."));
			::MessageBox(this->GetSafeHwnd(), tsResult.c_str(), _T("错误提示"), MB_ICONERROR);
			return;
		}
		else
		{
			tsResult = _T("");
			tsResult.append(_T("[")).append((StrRStrI(m_strDbFileName.c_str(), NULL, _T("\\")) + 1)).append(_T("].[")).append(m_strTableName.c_str()).append(_T("]表-")).append(m_strDbFileName.c_str());
			this->SetWindowText(tsResult.c_str());
		}
	}
	else
	{
		tsResult = _T("");
		tsResult.append(m_strTableName.c_str()).append(_T("数据表不存在, 请检查打开的数据库."));
		::MessageBox(this->GetSafeHwnd(), tsResult.c_str(), _T("错误提示"), MB_ICONERROR);
		return;
	}

	if (GetTableRecordSets(&m_svHeader, &m_svvDataItem, &m_SQLiteDb, m_strTableName))
	{
		//新建列
		InsertColumnData(m_pListCtrl, &m_svHeader);

		//新增数据
		InsertItemData(m_pListCtrl, &m_svvDataItem);

		UpdateStatusBarInfo();
	}
	else
	{
		::MessageBox(this->GetSafeHwnd(), _T("未获取到数据记录！"), _T("错误提示"), MB_ICONERROR);
		return;
	}
}
void CSQLiteDbManagerDlg::OpenOrReOpenDatabase(LPCTSTR lpFileName)
{
	////////////////////////////////////////////////////////////////////////////
	// 重置所有参数

	SetWindowText(SQLITE3_WINDOW_TITLE);

	lstrcpy(G_DB_PATH, _T(""));
	m_strDbFileName = G_DB_PATH;
	lstrcpy(g_tTableName, _T(""));
	m_strTableName = g_tTableName;
	//删除所有行
	((CListCtrl *)GetDlgItem(IDC_LIST_TABLEVIEW))->DeleteAllItems();

	//删除所有列
	while (((CListCtrl *)GetDlgItem(IDC_LIST_TABLEVIEW))->DeleteColumn(0));

	////////////////////////////////////////////////////////////////////////////

	lstrcpy(G_DB_PATH, lpFileName);
	m_strDbFileName = G_DB_PATH;
	//////////////////////////////////////////////////////////////////////////

	int nVal = _taccess(m_strDbFileName.c_str(), 0);
	if (nVal != 0)
	{
		::MessageBox(this->GetSafeHwnd(), _T("数据库文件不存在, 请检查打开的数据库路径."), _T("错误提示"), MB_ICONERROR);
		return;
	}
	if (m_SQLiteDb.IsOpened())
	{
		m_SQLiteDb.Close();
	}
	if (m_SQLiteDb.Open(m_strDbFileName.c_str()))
	{
		ShowTablesSelectDlg();
	}
	else
	{
		::MessageBox(this->GetSafeHwnd(), _T("指定目录文件不是标准的数据库文件, 请检查文件是否损坏."), _T("错误提示"), MB_ICONERROR);
		return;
	}
}
void CSQLiteDbManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == IDM_SELECT_DBTABLE)
	{
		if (m_SQLiteDb.IsOpened())
		{
			ShowTablesSelectDlg();
		}
		else
		{
			::MessageBox(this->GetSafeHwnd(), _T("当前尚未打开数据库,请先打开数据库."), _T("错误提示"), MB_ICONERROR);
			return;
		}
	}
	else if (nID == IDM_SELECT_DBFILE)
	{
		OPENFILENAME ofn;       // common dialog box structure
		_TCHAR szFile[MAX_PATH];       // buffer for file name
									   // Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = _T('\0');
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = _T("All\0*.*\0Sqlite\0*.DB\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 
		if (GetOpenFileName(&ofn) == TRUE)
		{
			if (lstrcmpi(G_DB_PATH, szFile))
			{
				OpenOrReOpenDatabase(szFile);
			}
			else
			{
				ShowTablesSelectDlg();
			}
		}
	}
	else if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSQLiteDbManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSQLiteDbManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSQLiteDbManagerDlg::ResizeWindow()
{
	INT nHeight = 0;
	INT nWidth = 0;
	CRect rc(0,0,0,0);	
	CRect rcWindow(0,0,0,0);
	CRect rcControl(0,0,0,0);
	CRect rcStatusBar(0,0,0,0);

	this->GetClientRect(rcWindow);
	
	//按钮系列的位置
	m_pButtonQuit->GetClientRect(rcControl);
	nWidth += rcControl.Width();
	nHeight = max_val(nHeight, rcControl.Height());
	rc.left = rcWindow.Width() - nWidth;
	rc.top = 0;
	rc.right = rc.left + rcControl.Width();
	rc.bottom = rc.top + rcControl.Height();
	m_pButtonQuit->MoveWindow(rc, FALSE);
	
	m_pButtonCreate->GetClientRect(rcControl);
	nWidth += rcControl.Width();
	nHeight = max_val(nHeight, rcControl.Height());
	rc.left = rcWindow.Width() - nWidth;
	rc.top = 0;
	rc.right = rc.left + rcControl.Width();
	rc.bottom = rc.top + rcControl.Height();
	m_pButtonCreate->MoveWindow(rc, FALSE);
		
	m_pButtonFilter->GetClientRect(rcControl);
	nWidth += rcControl.Width();
	nHeight = max_val(nHeight, rcControl.Height());
	rc.left = rcWindow.Width() - nWidth;
	rc.top = 0;
	rc.right = rc.left + rcControl.Width();
	rc.bottom = rc.top + rcControl.Height();
	m_pButtonFilter->MoveWindow(rc, FALSE);

	m_pButtonFinder->GetClientRect(rcControl);
	nWidth += rcControl.Width();
	nHeight = max_val(nHeight, rcControl.Height());
	rc.left = rcWindow.Width() - nWidth;
	rc.top = 0;
	rc.right = rc.left + rcControl.Width();
	rc.bottom = rc.top + rcControl.Height();
	m_pButtonFinder->MoveWindow(rc, FALSE);

	//ComboxBox动态增长控件位置
	m_pComboBoxFinderFilter->GetClientRect(rcControl);
	nHeight = max_val(nHeight, rcControl.Height());
	rc.left = 0;
	rc.top = 0;
	rc.right = rcWindow.Width() - nWidth;
	rc.bottom = rc.top + rcControl.Height();
	m_pComboBoxFinderFilter->MoveWindow(rc, FALSE);

	//状态栏动态增长控件位置
	m_pStaticStatusBar->GetClientRect(rcStatusBar);
	rc.left = 0;
	rc.top = rcWindow.Height() - rcStatusBar.Height();
	rc.right = rcWindow.Width();
	rc.bottom = rc.top + rcStatusBar.Height();
	m_pStaticStatusBar->MoveWindow(rc, FALSE);

	//列表视图动态增长控件位置
	rc.left = 0;
	rc.top = nHeight;
	rc.right = rcWindow.Width();
	rc.bottom = rcWindow.Height() - rcStatusBar.Height();
	m_pListCtrl->MoveWindow(rc, FALSE);

	RedrawWindow(&rcWindow, NULL, RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN | RDW_INVALIDATE);
}

void CSQLiteDbManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (GetDlgItem(IDOK)->GetSafeHwnd())
	{
		ResizeWindow();
	}
}

void CSQLiteDbManagerDlg::OnNMRClickListTableview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	CListCtrl * pListCtrl = (CListCtrl *)GetDlgItem(pNMItemActivate->hdr.idFrom);
	if (pListCtrl && pNMItemActivate->iItem != (-1) && 
		(pNMItemActivate->hdr.hwndFrom == pListCtrl->GetSafeHwnd()))
	{
		CPoint pt;
		CMenu menu;
		CMenu * pMenu = NULL;

		if(menu.LoadMenu(IDR_MENU_TRACK) && (pMenu = menu.GetSubMenu(0)))
		{
			GetCursorPos(&pt);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, NULL);
		}
	}

	*pResult = 0;
}

void CSQLiteDbManagerDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	StringVector l_svDataItem;
	for (size_t stIdx = 0; stIdx < m_svHeader.size(); stIdx++)
	{
		l_svDataItem.push_back(_T(""));
	}
	CCustomDlg l_CustomDlg(m_strTableName, &m_svHeader, &l_svDataItem, this, TRUE, l_svDataItem.size() / 2 + 1);
	l_CustomDlg.DoModal();

	//CDialogEx::OnOK();
}

void CSQLiteDbManagerDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_SQLiteDb.IsOpened())
	{
		m_SQLiteDb.Close();
	}

	CDialogEx::OnCancel();
}
void CSQLiteDbManagerDlg::OnTrackUpdate()
{
	if (m_pListCtrl->GetHeaderCtrl())
	{
		int nColumnNumber = m_pListCtrl->GetHeaderCtrl()->GetItemCount();
		POSITION pt = m_pListCtrl->GetFirstSelectedItemPosition();
		if (pt)//支持改成while多选
		{
			int nItem = m_pListCtrl->GetNextSelectedItem(pt);
			if (nItem != -1)
			{
				StringVector l_svDataItem;
				for (int iIdx = 0; iIdx < nColumnNumber; iIdx++)
				{
					CString cs = m_pListCtrl->GetItemText(nItem, iIdx);
					l_svDataItem.push_back(tstring((LPCTSTR)cs));
				}
				CCustomDlg l_CustomDlg(m_strTableName, &m_svHeader, &l_svDataItem, this, FALSE, l_svDataItem.size() / 2 + 1);
				l_CustomDlg.DoModal();
			}
		}		
	}
}

void CSQLiteDbManagerDlg::OnTrackDelete()
{
	if (m_pListCtrl)
	{
		StringVector sv;
		IntegerVector iv;
		BOOL bRet = FALSE;
		tstring tsResult = _T("");
		tstring tsSqlCmd = _T("DELETE FROM ");
		tsSqlCmd.append(m_strTableName.c_str()).append(_T(" WHERE ")).append(m_svHeader.at(0).c_str()).append(_T(" IN ('"));

		POSITION pt = m_pListCtrl->GetFirstSelectedItemPosition();
		if (pt)//支持改成while多选
		{
			int nItem = m_pListCtrl->GetNextSelectedItem(pt);
			if (nItem != -1)
			{
				CString cs = m_pListCtrl->GetItemText(nItem, 0);
				tsSqlCmd.append((LPCTSTR)cs).append(_T("',"));
				iv.push_back(nItem);
				sv.push_back(tstring((LPCTSTR)cs));
			}
		}
		size_t npos = tsSqlCmd.rfind(_T("',"));
		if (npos != tstring::npos)
		{
			tsSqlCmd.replace(npos, 2, _T("')"));
		}
		
		bRet = m_SQLiteDb.ExecuteCommand(tsSqlCmd.c_str());
		if(bRet)
		{
			for (size_t stIdx = 0; stIdx < iv.size(); stIdx++)
			{
				m_pListCtrl->DeleteItem(iv.at(stIdx));
			}
			DeleteRecord(sv);
			UpdateStatusBarInfo();

			::MessageBox(this->GetSafeHwnd(), _T("删除记录成功!"), _T("提示"), MB_ICONINFORMATION);
		}
		else
		{
			tsResult.append(_T("删除记录失败!\r\n错误原因: ")).append(m_SQLiteDb.GetLastError());
			::MessageBox(this->GetSafeHwnd(), tsResult.c_str(), _T("提示"), MB_ICONERROR);
		}
	}
}


void CSQLiteDbManagerDlg::OnNMDblclkListTableview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	OnTrackUpdate();

	*pResult = 0;
}
typedef struct tagSortData{
	INT iIdx;		//列号
	CWnd *pWnd;		//CListCtrl控件指针
	BOOLEAN bSort;	//True为升序，False为降序
}SortData, *PSortData;

//排序时比较表项的回调函数
INT CALLBACK CompareFunction(LPARAM lParamL,LPARAM lParamR,LPARAM lParamD)
{
	INT iResult = 0;
	tstring strL = _T("");
	tstring strR = _T("");
	SortData *psd = (SortData *)lParamD;
	if (psd)
	{
		INT iItemIdxL = 0;
		INT iItemIdxR = 0;
		INT iIdx = psd->iIdx;
		LVFINDINFO lvfi = {0};		
		CListCtrl* pListCtrl = (CListCtrl *)psd->pWnd;
				
		lvfi.flags = LVFI_PARAM;

		if (pListCtrl)
		{
			lvfi.lParam = lParamL;
			iItemIdxL = pListCtrl->FindItem(&lvfi, -1);
			lvfi.lParam = lParamR;
			iItemIdxR = pListCtrl->FindItem(&lvfi, -1);
			if (iItemIdxL != (-1) && iItemIdxR != (-1))
			{
				strL = pListCtrl->GetItemText(iItemIdxL, iIdx);			
				strR = pListCtrl->GetItemText(iItemIdxR, iIdx);

				iResult = strL.compare(strR);
			}
		}		
	}

	return ((psd->bSort) ? iResult : (-iResult));
}

void CSQLiteDbManagerDlg::OnLvnColumnClickListTableview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	if(pNMLV->iSubItem != (-1))
	{
		SortData sd = {0};
		sd.iIdx = pNMLV->iSubItem;
		sd.pWnd = GetDlgItem(pNMLV->hdr.idFrom);
		if (sd.pWnd)
		{
			int nSortItem = (int)GetProp(pNMLV->hdr.hwndFrom, _T("SORT_COLUMN"));
			int nSortDesc = (int)GetProp(pNMLV->hdr.hwndFrom, _T("SORT_DIRECTION"));
			if(nSortItem == pNMLV->iSubItem)
			{
				nSortDesc = (nSortDesc + 1) % 2;
				SetProp(pNMLV->hdr.hwndFrom,_T("SORT_DIRECTION"), (HANDLE)nSortDesc);
			}
			SetProp(pNMLV->hdr.hwndFrom, _T("SORT_COLUMN"), (HANDLE)pNMLV->iSubItem);
			sd.bSort = nSortDesc;

			IntegerVector ivImg;
			for (int i = 0; i < ((CListCtrl*)sd.pWnd)->GetHeaderCtrl()->GetItemCount(); i++)
			{
				if (pNMLV->iSubItem != i)
				{
					ivImg.push_back(2);
				}
				else
				{
					ivImg.push_back(!sd.bSort);
				}
			}
			SetColumnHeaderImage(((CListCtrl*)sd.pWnd), &ivImg);

			((CListCtrl*)sd.pWnd)->SortItems((PFNLVCOMPARE)CompareFunction, (DWORD_PTR)&sd);
		}			
	}	

	*pResult = 0;
}


void CSQLiteDbManagerDlg::OnBnClickedButtonFinder()
{
	// TODO: Add your control notification handler code here
	int nIndex = (-1);
	LVFINDINFO lvfi = {0};
	CString strText(_T(""));
	CString strTemp(_T(""));
	tstring tsResult = _T("");

	m_ivFinder.clear();
	if(GetDlgItemText(m_pComboBoxFinderFilter->GetDlgCtrlID(), strText))
	{
		if (strText.GetLength())
		{
			bool bFind = false;
			size_t stSize = m_svCombox.size();
			for (size_t stIdx = 0; stIdx < stSize; stIdx++)
			{
				if (!m_svCombox.at(stIdx).compare(strText))
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				m_svCombox.push_back(tstring((LPCTSTR)strText));
				m_pComboBoxFinderFilter->AddString((LPCTSTR)strText);
			}

			lvfi.flags = LVFI_STRING | LVFI_PARTIAL;
			lvfi.psz = (LPCTSTR)strText;
			while ((nIndex = m_pListCtrl->FindItem(&lvfi, nIndex)) >= 0 
				&& (size_t)nIndex < m_svvDataItem.size())
			{
				m_ivFinder.push_back(nIndex);				
			}
		}
	}
}

void GetListCtrlLineData(StringVector * psv, INT nLineIdx, CListCtrl * pListCtrl)
{
	if (psv && pListCtrl && pListCtrl->GetHeaderCtrl())
	{
		for (INT nIdx = 0; nIdx < pListCtrl->GetHeaderCtrl()->GetItemCount(); nIdx++)
		{
			CString cs(_T(""));
			cs = pListCtrl->GetItemText(nLineIdx, nIdx);
			psv->push_back(tstring((LPCTSTR)cs));
		}	
	}
}

void CSQLiteDbManagerDlg::OnBnClickedButtonFilter()
{
	// TODO: Add your control notification handler code here
	CString strText(_T(""));
	
	OnBnClickedButtonFinder();
	
	if(GetDlgItemText(m_pComboBoxFinderFilter->GetDlgCtrlID(), strText))
	{
		StringVectorVector svv;
		size_t stSize = m_ivFinder.size();
		
		for (size_t stIdx = 0; stIdx < stSize; stIdx++)
		{
			StringVector sv;
			GetListCtrlLineData(&sv, m_ivFinder.at(stIdx), m_pListCtrl);
			svv.push_back(sv);
		}

		InsertItemData(m_pListCtrl, &svv, TRUE);
	}
	else
	{
		InsertItemData(m_pListCtrl, &m_svvDataItem, TRUE);
	}
}

void CSQLiteDbManagerDlg::OnCBNEditUpdateComboFinderFilter()
{
	// TODO: Add your control notification handler code here
	InsertItemData(m_pListCtrl, &m_svvDataItem, TRUE);
	OnBnClickedButtonFilter();
}

void *CSQLiteDbManagerDlg::GetSQLiteDatabase()
{
	return (void *)&m_SQLiteDb;
}


void CSQLiteDbManagerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	_TCHAR szFile[MAX_PATH] = { 0 };
	WIN32_FILE_ATTRIBUTE_DATA wfad = { 0 };
	UINT uNum = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	if (uNum)
	{
		for (UINT i = 0; i<uNum; i++)
		{
			::DragQueryFile(hDropInfo, i, szFile, _countof(szFile));
			if (::GetFileAttributesEx(szFile, GetFileExInfoStandard, &wfad) && 
				((wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY))
			{
				if (lstrcmpi(G_DB_PATH, szFile))
				{
					OpenOrReOpenDatabase(szFile);
				}
				else
				{
					ShowTablesSelectDlg();
				}
				break;
			}
			
		}
	}

	::DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}
