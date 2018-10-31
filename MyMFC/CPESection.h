#pragma once


// CPESection 对话框

class CPESection : public CDialogEx
{
	DECLARE_DYNAMIC(CPESection)

public:
	CPESection(IMAGE_NT_HEADERS *pNt, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPESection();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECTION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	IMAGE_NT_HEADERS * m_pNt;
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
};
