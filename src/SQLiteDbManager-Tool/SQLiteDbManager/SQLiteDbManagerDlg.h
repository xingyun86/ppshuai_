
// SQLiteDbManagerDlg.h : header file
//

#pragma once


// CSQLiteDbManagerDlg dialog
class CSQLiteDbManagerDlg : public CDialogEx
{
// Construction
public:
	CSQLiteDbManagerDlg(tstring strDbFileName, tstring strTableName, CWnd* pParent = NULL);	// standard constructor
	CSQLiteDbManagerDlg(CWnd* pParent = NULL);	// standard constructor
	~CSQLiteDbManagerDlg();
	
// Dialog Data
	enum { IDD = IDD_SQLITEDBMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CImageList m_imgListHeader;
	CComboBox *m_pComboBoxFinderFilter;
	CButton * m_pButtonFinder;
	CButton * m_pButtonFilter;
	CButton * m_pButtonCreate;
	CButton * m_pButtonQuit;
	CListCtrl *m_pListCtrl;
	CStatic *m_pStaticStatusBar;

	CSQLiteDatabase m_SQLiteDb;
	tstring m_strDbFileName;
	tstring m_strTableName;
	IntegerVector m_ivFinder;
	StringVector m_svCombox;
	StringVector m_svHeader;
	StringVectorVector m_svvDataItem;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTrackUpdate();
	afx_msg void OnTrackDelete();
	afx_msg void OnNMRClickListTableview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTableview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnClickListTableview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFinder();
	afx_msg void OnBnClickedButtonFilter();
	afx_msg void OnCBNEditUpdateComboFinderFilter();
	DECLARE_MESSAGE_MAP()

	void ResizeWindow();
	
public:
	void InsertRecord(StringVector sv);
	void DeleteRecord(StringVector sv);
	void UpdateRecord(StringVector sv);

	void UpdateStatusBarInfo();
	void *GetSQLiteDatabase();
	
};
