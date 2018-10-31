// CService.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CService.h"
#include "afxdialogex.h"


// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVICE_DLG, pParent)
{

}

CService::~CService()
{
}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CService::OnRclickList2)
	ON_COMMAND_RANGE(ID_SUBMENU, ID_SUBMENU1, &CService::OnRClickMenu)
END_MESSAGE_MAP()


// CService 消息处理程序


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("名称"), 0, 150);
	m_ListCtrl.InsertColumn(1, _T("PID"), 0, 80);
	m_ListCtrl.InsertColumn(2, _T("描述"), 0, 150);
	m_ListCtrl.InsertColumn(2, _T("状态"), 0, 150);

	//打开远程计算机服务控制管理器
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;

	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)
		new char[dwSize];

	//第二次枚举
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, (LPBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	CString str;
	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		m_ListCtrl.InsertItem(i, pEnumService[i].lpServiceName);
		str.Format(L"%d", pEnumService[i].ServiceStatusProcess.dwProcessId);
		m_ListCtrl.SetItemText(i, 1, str);
		m_ListCtrl.SetItemText(i, 2, GetStringByCurrentState(pEnumService[i].ServiceStatusProcess.dwCurrentState));
		m_ListCtrl.SetItemText(i, 3, pEnumService[i].lpDisplayName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

LPCTSTR CService::GetStringByCurrentState(DWORD state)
{
	switch (state)
	{
	case SERVICE_CONTINUE_PENDING:
		return L"即将继续";
	case SERVICE_PAUSE_PENDING:
		return L"暂停";
	case SERVICE_PAUSED:
		return L"已暂停";
	case SERVICE_RUNNING:
		return L"正在运行";
	case SERVICE_START_PENDING:
		return L"正在启动";
	case SERVICE_STOP_PENDING:
		return L"正在停止";
	case SERVICE_STOPPED:
		return L"已停止";
	default:
		break;
	}
	return L"";
}


void CService::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

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
		menu.AppendMenu(MF_STRING, ID_SUBMENU, L"启动服务");
		menu.AppendMenu(MF_STRING, ID_SUBMENU1, L"停止服务");
		menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

//单击子菜单事件
void CService::OnRClickMenu(UINT id)
{
	//打开服务获取服务句柄
	CString serviceName = m_ListCtrl.GetItemText(m_nItem, 0);
	SC_HANDLE hService = OpenService(hSCM, serviceName, SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP);
	BOOL bRet = FALSE;
	switch (id)
	{
		//启动服务
	case ID_SUBMENU:
	{
		bRet = StartService(hService, NULL, NULL);
		if (bRet)
		{
			SERVICE_STATUS serviceStatus = { 0 };
			do {
				bRet = QueryServiceStatus(hService, &serviceStatus);
				//休眠(挂起线程),单位毫秒
				Sleep(300);
			} while (bRet && serviceStatus.dwCurrentState != SERVICE_RUNNING);
			m_ListCtrl.SetItemText(m_nItem, 2, GetStringByCurrentState(serviceStatus.dwCurrentState));
		}
	}
	break;
	//停止服务
	case ID_SUBMENU1:
	{
		SERVICE_STATUS serviceStatus = { 0 };
		bRet = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
		if (bRet)
		{
			do {
				bRet = QueryServiceStatus(hService, &serviceStatus);
				//休眠(挂起线程),单位毫秒
				Sleep(300);
			} while (bRet && serviceStatus.dwCurrentState != SERVICE_STOPPED);
			m_ListCtrl.SetItemText(m_nItem, 2, GetStringByCurrentState(serviceStatus.dwCurrentState));
		}
	}
	break;
	default:
		break;
	}

	CloseServiceHandle(hService);
}

