// CDIALOG2.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CDIALOG2.h"
#include "afxdialogex.h"
#include <Winuser.h>

//窗口回调函数，非成员函数
BOOL CALLBACK  EnumWinProc(HWND hWnd, LPARAM lParam)
{
	TCHAR buff[200] = { 0 };
	GetWindowText(hWnd, buff, 200);

	CListCtrl *pList = (CListCtrl*)lParam;
	
	if (IsWindowVisible(hWnd) && wcslen(buff) != 0)
	{
		pList->InsertItem(0, buff);
	}
	return TRUE;
}
// CDIALOG2 对话框

IMPLEMENT_DYNAMIC(CDIALOG2, CDialogEx)

CDIALOG2::CDIALOG2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CDIALOG2::~CDIALOG2()
{
}

void CDIALOG2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BOOL  CDIALOG2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("子窗口"));
	//窗口初始化处理函数
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT  | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("窗口名"), 0, 500);

	//遍历窗口信息
	EnumWindows(&EnumWinProc, (LPARAM)&m_ListCtrl);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CDIALOG2, CDialogEx)
END_MESSAGE_MAP()


// CDIALOG2 消息处理程序

