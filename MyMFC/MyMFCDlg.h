
// MyMFCDlg.h: 头文件
//

#pragma once
#include <vector>
#include "CClientSocket.h"

#define WM_ONMYSOCKET		WM_USER+1

// CMyMFCDlg 对话框
class CMyMFCDlg : public CDialogEx
{
// 构造
public:
	CMyMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CString text;
	CDialogEx *m_pDia[7];
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	void GetFileString(char *pFileName);
	afx_msg void OnClickedButton1();
	void FindMD5VirusAndDelete(LPCWSTR szFoldPath, DWORD dwTag);
	std::vector<CString> m_vStr;
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	CClientSocket m_client;
protected:
	afx_msg LRESULT OnMySocket(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClickedButton5();
	CString m_strPath;
	BOOL m_IsWindowHide;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};
