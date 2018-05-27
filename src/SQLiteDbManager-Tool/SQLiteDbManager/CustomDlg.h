#pragma once

#define STATIC_ID			20000	//静态框起始ID
#define EDIT_ID				21000	//编辑框起始ID
#define BUTTON_ID			22000	//按钮起始ID
#define STATIC_WIDTH		160		//静态库高度
#define STATIC_HEIGHT		20		//静态框高度
#define EDIT_WIDTH			160		//编辑框宽度
#define EDIT_HEIGHT			60		//编辑框高度
#define BUTTON_WIDTH		100		//按钮宽度
#define BUTTON_HEIGHT		30		//按钮高度
#define SPACE_WIDTH			2		//左右控件间距
#define SPACE_HEIGHT		2		//上下空间间距
#define BORDER_SPACE		6		//边界间距
#define DEFAULT_LINENUMBER	6		//默认一行的控件个数

typedef std::map<DWORD, CWnd*> WndMap;
typedef std::map<DWORD, CWnd*>::iterator WndMapIt;
typedef std::pair<DWORD, CWnd*> MAKE_PAIR;

// CCustomDlg dialog

class CCustomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomDlg)

public:
	CCustomDlg(CWnd* pParent = NULL);   // standard constructor
	CCustomDlg(tstring tsTableName, StringVector * psvDesc, StringVector *psvData, CWnd* pParent = NULL, BOOL bCreate = FALSE, INT nLineNumber = DEFAULT_LINENUMBER);   // extend constructor
	virtual ~CCustomDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;

private:
	WndMap m_wmStatic;
	WndMap m_wmEdit;
	StringVector *m_psvDesc;
	StringVector *m_psvData;
	StringVector m_svData;

	INT m_nLineNumber;
	BOOL m_bCreate;
	CWnd m_ButtonConfirm;
	CWnd m_ButtonCancel;
	tstring m_strTableName;
	CSQLiteDbManagerDlg *m_pSQLiteDbManagerDlg;

private:
	void ReleaseControl();
	size_t DynamicCreateGroupControl(size_t stStartIndex, 
		size_t stControlNumber, size_t stLineNumber, size_t &stLineIndex);

};
