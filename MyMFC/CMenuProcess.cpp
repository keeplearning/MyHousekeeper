// CMenuProcess.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CMenuProcess.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CMenuProcess 对话框

IMPLEMENT_DYNAMIC(CMenuProcess, CDialogEx)

CMenuProcess::CMenuProcess(DWORD id, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent),
	m_dwMenuId(id)
{

}

CMenuProcess::~CMenuProcess()
{
}

BOOL  CMenuProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("子窗口"));
	//窗口初始化处理函数
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//根据菜单选项的不同 初始化表格的列也不同
	switch (m_dwMenuId)
	{
	case 32771:
	{
		m_ListCtrl.InsertColumn(0, _T("模块名字"), 0, 150);
		m_ListCtrl.InsertColumn(1, _T("模块ID"), 0, 50);
		m_ListCtrl.InsertColumn(2, _T("模块所属进程ID"), 0, 50);
		m_ListCtrl.InsertColumn(3, _T("模块路径"), 0, 200);
	}
	break;
	case 32772:
	{
		m_ListCtrl.InsertColumn(0, _T("堆ID"), 0, 150);
		m_ListCtrl.InsertColumn(1, _T("进程ID"), 0, 50);
		m_ListCtrl.InsertColumn(2, _T("堆大小"), 0, 50);
		m_ListCtrl.InsertColumn(3, _T("堆地址"), 0, 200);
	}
	break;
	case 32773:
	{
		m_ListCtrl.InsertColumn(0, _T("线程ID"), 0, 100);
		m_ListCtrl.InsertColumn(1, _T("所属进程ID"), 0, 100);
	}
	break;
	default:
		break;
	}
	return TRUE;
}

void CMenuProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

void CMenuProcess::ShowHeap(DWORD id)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, id);

	HEAPLIST32 peList = { sizeof(HEAPLIST32) };

	BOOL bSuccess = Heap32ListFirst(hSnap, &peList);

	if (bSuccess)
	{
		HEAPENTRY32 pe = { sizeof(HEAPENTRY32) };
		do 
		{
			
			Heap32First(&pe, peList.th32ProcessID, peList.th32HeapID);
			do 
			{
				CString str;
				str.Format(_T("%d"), pe.th32HeapID);
				m_ListCtrl.InsertItem(0, str);
				str.Format(_T("%d"), pe.th32ProcessID);
				m_ListCtrl.SetItemText(0, 1, str);
				str.Format(_T("%d"), pe.dwBlockSize);
				m_ListCtrl.SetItemText(0, 2, str);
				str.Format(_T("0x%x"), pe.dwAddress);
				m_ListCtrl.SetItemText(0, 3, str);


			} while (FALSE);
			
		} while (Heap32ListNext(hSnap, &peList));
	}
	CloseHandle(hSnap);
}

void CMenuProcess::ShowThread(DWORD id)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, id);

	THREADENTRY32 pe = { sizeof(THREADENTRY32) };

	BOOL bSuccess = Thread32First(hSnap, &pe);

	if (bSuccess)
	{
		do 
		{
			CString str;
			str.Format(_T("%d"), pe.th32ThreadID);
			m_ListCtrl.InsertItem(0, str);
			str.Format(_T("%d"), pe.th32OwnerProcessID);
			m_ListCtrl.SetItemText(0, 1, str);

		} while (Thread32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
}

void CMenuProcess::ShowModule(DWORD id)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	MODULEENTRY32 pe = { sizeof(MODULEENTRY32) };

	BOOL bSuccess = Module32First(hSnap, &pe);

	if (bSuccess)
	{
		do 
		{
			CString str;
			str.Format(_T("%d"), pe.th32ModuleID);
			m_ListCtrl.InsertItem(0, pe.szModule);
			m_ListCtrl.SetItemText(0, 1, str);
			str.Format(_T("%d"), pe.th32ProcessID);
			m_ListCtrl.SetItemText(0, 2, str);
			m_ListCtrl.SetItemText(0, 3, pe.szExePath);
		} while (Module32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
}

BEGIN_MESSAGE_MAP(CMenuProcess, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMenuProcess::OnRclickList1)
	ON_COMMAND_RANGE(ID_SUBMENU, ID_SUBMENU2, &CMenuProcess::OnRClickMenu)
END_MESSAGE_MAP()


// CMenuProcess 消息处理程序



void CMenuProcess::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//如果不是线程界面 不用 弹出菜单
	if (m_dwMenuId != 32773)
	{
		return;
	}
	CPoint pt;
	GetCursorPos(&pt);
	m_ListCtrl.ScreenToClient(&pt);
	LVHITTESTINFO info;
	info.pt = pt;
	//获取鼠标点击当前行的行数
	m_nItem = m_ListCtrl.HitTest(&info);
	if (m_nItem != -1)
	{
		CMenu menu;
		CPoint point;
		GetCursorPos(&point);
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_SUBMENU, L"挂起线程");
		menu.AppendMenu(MF_STRING, ID_SUBMENU1, L"恢复线程");
		menu.AppendMenu(MF_STRING, ID_SUBMENU2, L"结束线程");
		menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

void CMenuProcess::OnRClickMenu(UINT id)
{
	//获取线程ID
	CString str;
	str = m_ListCtrl.GetItemText(m_nItem, 0);
	//线程ID
	DWORD pid = 0;
	swscanf_s(str, L"%d", &pid);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pid);

	if (hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//响应菜单事件
	switch (id)
	{
	case 200:
	{
		DWORD err = SuspendThread(hThread);
		if (err != 0)
		{
			MessageBox(L"挂起线程成功");
		}
	}
	break;
	case 201:
	{
		DWORD err = ResumeThread(hThread);
		if (err != 0)
		{
			MessageBox(L"恢复线程成功");
		}
	}
	break;
	case 202:
	{
		BOOL bSuccess = TerminateThread(hThread, 0);
		if (bSuccess)
		{
			m_ListCtrl.DeleteItem(m_nItem);
			MessageBox(L"删除线程成功");
		}
	}
	break;
	default:
		break;
	}
}