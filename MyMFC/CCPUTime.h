#pragma once


// CCPUTime 对话框

class CCPUTime : public CDialogEx
{
	DECLARE_DYNAMIC(CCPUTime)

public:
	CCPUTime(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCPUTime();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	__int64 CompareFileTime(FILETIME time1, FILETIME time2);
	CString m_editStr;
protected:
	afx_msg LRESULT OnUpdatedata(WPARAM wParam, LPARAM lParam);
public:
	CString m_editMem;
	afx_msg void OnClickedButton1();
};

DWORD WINAPI ThreadFunc(LPVOID p);