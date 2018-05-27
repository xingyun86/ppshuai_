#pragma once

#define STATIC_ID			20000	//��̬����ʼID
#define EDIT_ID				21000	//�༭����ʼID
#define BUTTON_ID			22000	//��ť��ʼID
#define STATIC_WIDTH		160		//��̬��߶�
#define STATIC_HEIGHT		20		//��̬��߶�
#define EDIT_WIDTH			160		//�༭����
#define EDIT_HEIGHT			60		//�༭��߶�
#define BUTTON_WIDTH		100		//��ť���
#define BUTTON_HEIGHT		30		//��ť�߶�
#define SPACE_WIDTH			2		//���ҿؼ����
#define SPACE_HEIGHT		2		//���¿ռ���
#define BORDER_SPACE		6		//�߽���
#define DEFAULT_LINENUMBER	6		//Ĭ��һ�еĿؼ�����

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
