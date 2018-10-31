// CDIALOG1.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CDIALOG1.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include "CMenuProcess.h"


// CDIALOG1 对话框

IMPLEMENT_DYNAMIC(CDIALOG1, CDialogEx)

CDIALOG1::CDIALOG1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDIALOG1::~CDIALOG1()
{
}

void CDIALOG1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDIALOG1, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDIALOG1::OnRclickList1)
	ON_COMMAND_RANGE(ID_32771, ID_32774, &CDIALOG1::OnRClickMenu)
END_MESSAGE_MAP()


// CDIALOG1 消息处理程序

BOOL  CDIALOG1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("子窗口"));
	//窗口初始化处理函数
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("名字"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("PID"), 0, 50);
	m_ListCtrl.InsertColumn(2, _T("路径"), 0, 300);

	ShowProcess();
	
	return TRUE;
}

//遍历显示所有进程
void CDIALOG1::ShowProcess()
{
	//获取快照句柄
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
	//获取第一个进程
	BOOL bSucess = Process32First(hSnap, &pe32);
	USES_CONVERSION;
	if (bSucess)
	{
		do 
		{
			m_ListCtrl.InsertItem(0, pe32.szExeFile);
			CString str;
			str.Format(_T("%d"), pe32.th32ProcessID);
			m_ListCtrl.SetItemText(0, 1, str);
			//打开进程获取exe路径
			HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, 
				FALSE, pe32.th32ProcessID);
			TCHAR szPath[MAX_PATH] = { 0 };
			GetModuleFileNameEx(hPro, 0, szPath, MAX_PATH);
			m_ListCtrl.SetItemText(0, 2, szPath);
		} while (Process32Next(hSnap, &pe32));
	}
	//关闭快照
	CloseHandle(hSnap);
}

void CDIALOG1::OnRClickMenu(UINT id)
{
	//获取进程ID
	CString str;
	str = m_ListCtrl.GetItemText(m_RowNum, 1);
	//进程ID
	DWORD pid = 0;
	swscanf_s(str, L"%d", &pid);
	//关闭进程
	if(id == 32774)
	{
		if (id == GetCurrentProcessId())
		{
			//关闭自己
			ExitProcess(0);
		}
		else
		{
			//关闭别人
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			BOOL bSuccess  = TerminateProcess(hProcess, 0);
			if (bSuccess)
			{
				m_ListCtrl.DeleteItem(m_RowNum);
				MessageBox(L"关闭进程成功");
			}
			
		}
		return;
	}
	//为非模态窗口申请内存
	CMenuProcess* pDlg = new CMenuProcess(id);
 	pDlg->Create(IDD_DIALOG3);
 	pDlg->ShowWindow(SW_SHOW);
	
	//根据不同id显示不同信息
	switch (id)
	{
	//遍历模块
	case 32771:
	{
		pDlg->ShowModule(pid);
	}
	break;
	//遍历堆
	case 32772:
	{
		pDlg->ShowHeap(pid);
	}
	break;
	//遍历线程
	case 32773:
	{
		pDlg->ShowThread(pid);
	}
	break;
	default:
		break;
	}
}

void CDIALOG1::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CPoint pos;
	GetCursorPos(&pos);

	m_ListCtrl.ScreenToClient(&pos);

	LVHITTESTINFO info;

	//获取鼠标点击当前行的行数
	info.pt = pos;
	INT nItem = m_ListCtrl.HitTest(&info);
	//如果不为-1  载入菜单
	if (nItem != -1)
	{
		m_RowNum = nItem;
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CPoint point;
		GetCursorPos(&point);
		
		CMenu *pSubMebu = menu.GetSubMenu(0);
		pSubMebu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}

}
