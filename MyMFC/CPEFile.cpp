// CPEFile.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CPEFile.h"
#include "afxdialogex.h"
#include "CAnalysisPE.h"


// CPEFile 对话框

IMPLEMENT_DYNAMIC(CPEFile, CDialogEx)

CPEFile::CPEFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
	, m_strPath(_T(""))
{

}

CPEFile::~CPEFile()
{
}

void CPEFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPath);
}


BEGIN_MESSAGE_MAP(CPEFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPEFile::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPEFile::OnClickedButton2)
END_MESSAGE_MAP()


// CPEFile 消息处理程序


void CPEFile::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建模态窗口，选择文件
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CPEFile::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//创建模态窗口，解析PE文件
	CAnalysisPE aly(m_strPath);
	INT IS = aly.DoModal();
}
