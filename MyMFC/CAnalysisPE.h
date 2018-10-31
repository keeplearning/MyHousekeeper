#pragma once


// CAnalysisPE 对话框

class CAnalysisPE : public CDialogEx
{
	DECLARE_DYNAMIC(CAnalysisPE)

public:
	CAnalysisPE(CString path, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAnalysisPE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALYSISPE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//文件路径
	CString m_strPath;
	virtual BOOL OnInitDialog();
	CString m_strData1;
	CString m_strData2;
	CString m_strData3;
	CString m_strData4;
	CString m_strData5;
	CString m_strData6;
	CString m_strData7;
	CString m_strData8;
	CString m_strData9;
	CString m_strData10;
	CString m_strData11;
	CString m_strData12;
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClose();
	HANDLE m_hFile;
	IMAGE_NT_HEADERS *m_pNt;
	LPBYTE m_pBuff;
};
