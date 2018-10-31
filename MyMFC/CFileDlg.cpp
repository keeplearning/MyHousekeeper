// CFileDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CFileDlg.h"
#include "afxdialogex.h"


// CFileDlg 对话框

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_ComBox);
}

BOOL  CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//窗口初始化处理函数
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("文件名"), 0, 150);
	m_ListCtrl.InsertColumn(1, _T("创建时间"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("修改时间"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("文件大小"), 0, 100);
	
	//获取电脑上的所有盘符 并且显示在combox中
	TCHAR szName[MAX_PATH] = { 0 };

	GetLogicalDriveStrings(MAX_PATH, szName);

	for (int i = 0; szName[i] != '\0' && i < MAX_PATH; ++i)
	{
		TCHAR szTmp[12] = { 0 };
		wcscpy_s(szTmp, &szName[i]);
		CString str;
		str.Format(_T("%s"), szTmp);
		m_ComBox.AddString(str);
		i += wcslen(&szName[i]);
	}
	m_ComBox.SetCurSel(0);
	CString tmp;
	m_ComBox.GetLBText(0, tmp);
	ShowFile(tmp);

	return TRUE;
}

void CFileDlg::ShowFile(CString str)
{
	//根据传进来的盘符遍历文件
	WIN32_FIND_DATA fData;
	HANDLE  hFile = FindFirstFile(str + L"*", &fData);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			m_ListCtrl.InsertItem(0, fData.cFileName);
			//文件创建时间
			CTime tm(fData.ftCreationTime);
			CString str = tm.Format("%Y-%m-%d");
			m_ListCtrl.SetItemText(0, 1, str);
			//文件修改时间
			CTime p = (fData.ftLastWriteTime);
			str = p.Format("%Y-%m-%d");
			m_ListCtrl.SetItemText(0, 2, str);
			//文件大小
			LONGLONG s = (fData.nFileSizeHigh << 32) + fData.nFileSizeLow;
			s = s / 1024;
			str.Format(L"%lld KB", s);
			m_ListCtrl.SetItemText(0, 3, str);
		} while (FindNextFile(hFile, &fData));
	}
}

BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFileDlg::OnSelchangeCombo1)
END_MESSAGE_MAP()


// CFileDlg 消息处理程序


void CFileDlg::OnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListCtrl.DeleteAllItems();
	int nNum = m_ComBox.GetCurSel();
	CString tmp;
	m_ComBox.GetLBText(nNum, tmp);
	ShowFile(tmp);
}
