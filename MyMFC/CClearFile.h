#pragma once


// CClearFile 对话框

class CClearFile : public CDialogEx
{
	DECLARE_DYNAMIC(CClearFile)

public:
	CClearFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClearFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void OnClickBtn(UINT id);
	void EnumFoldAndDeleteFile(LPCWSTR szFoldPath);
	CString m_editStr;
};
