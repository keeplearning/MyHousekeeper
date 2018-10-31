#pragma once


// CPEFile 对话框

class CPEFile : public CDialogEx
{
	DECLARE_DYNAMIC(CPEFile)

public:
	CPEFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPEFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	CString m_strPath;
	afx_msg void OnClickedButton2();
};
