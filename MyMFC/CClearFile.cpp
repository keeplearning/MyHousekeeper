// CClearFile.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CClearFile.h"
#include "afxdialogex.h"


// CClearFile 对话框

IMPLEMENT_DYNAMIC(CClearFile, CDialogEx)

CClearFile::CClearFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_editStr(_T(""))
{

}

CClearFile::~CClearFile()
{
}

void CClearFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editStr);
	
}


BEGIN_MESSAGE_MAP(CClearFile, CDialogEx)
	ON_COMMAND_RANGE(IDC_BUTTON1, IDC_BUTTON4, &CClearFile::OnClickBtn)
END_MESSAGE_MAP()


// CClearFile 消息处理程序
void CClearFile::OnClickBtn(UINT id)
{
	switch (id)
	{
	case IDC_BUTTON1:
	{
		CFolderPickerDialog fdlg;
		if (fdlg.DoModal() == IDOK)
		{
			m_editStr = fdlg.GetFolderPath();
			UpdateData(FALSE);
		}
	}
		break;
	case IDC_BUTTON2:
	{
		if (m_editStr == "")
		{
			MessageBox(L"路径不能为空");
			return;
		}
		EnumFoldAndDeleteFile(m_editStr);
		MessageBox(L"清理成功");
	}
		break;
	default:
		break;
	}
}

void CClearFile::EnumFoldAndDeleteFile(LPCWSTR szFoldPath)
{
	WIN32_FIND_DATA wfData = {};
	CString str;
	str.Format(L"%s%s", szFoldPath, L"\\*");
	HANDLE hFile = FindFirstFile(str, &wfData);
	do 
	{
		//去掉两个特殊目录
		if (!wcscmp(wfData.cFileName, L".") || !wcscmp(wfData.cFileName, L".."))
		{
			continue;
		}
		WCHAR szSubFoldPath[MAX_PATH];
		wsprintf(szSubFoldPath, L"%s\\%s", szFoldPath, wfData.cFileName);
		//如果是文件，继续删除文件里面的内容，使用递归
		if (wfData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EnumFoldAndDeleteFile(szSubFoldPath);
		}
		else
		{
			//删除垃圾文件
			LPWSTR fileSuffix = PathFindExtension(szSubFoldPath);
			if (!wcscmp(fileSuffix, L".tlog") || 
				!wcscmp(fileSuffix, L".obj") ||
				!wcscmp(fileSuffix, L".log") ||
				!wcscmp(fileSuffix, L".pch") ||
				!wcscmp(fileSuffix, L".ilk") ||
				!wcscmp(fileSuffix, L".pdb") ||
				!wcscmp(fileSuffix, L".tmp") ||
				!wcscmp(fileSuffix, L"._mp") ||
				!wcscmp(fileSuffix, L".gid") ||
				!wcscmp(fileSuffix, L".chk") ||
				!wcscmp(fileSuffix, L".old") ||
				!wcscmp(fileSuffix, L".bak"))
			{
				DeleteFile(szSubFoldPath);
			}
		}
	} while (FindNextFile(hFile, &wfData));
}
