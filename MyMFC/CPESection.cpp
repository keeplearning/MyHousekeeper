// CPESection.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CPESection.h"
#include "afxdialogex.h"


// CPESection 对话框

IMPLEMENT_DYNAMIC(CPESection, CDialogEx)

CPESection::CPESection(IMAGE_NT_HEADERS *pNt, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SECTION_DLG, pParent)
	, m_pNt(pNt)
{

}

CPESection::~CPESection()
{
}

void CPESection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CPESection, CDialogEx)
END_MESSAGE_MAP()


// CPESection 消息处理程序


BOOL CPESection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("名字"), 0, 80);
	m_ListCtrl.InsertColumn(1, _T("区段大小"), 0, 80);
	m_ListCtrl.InsertColumn(2, _T("RVA"), 0, 80);
	m_ListCtrl.InsertColumn(3, _T("文件大小"), 0, 80);
	m_ListCtrl.InsertColumn(4, _T("文件偏移"), 0, 80);
	m_ListCtrl.InsertColumn(5, _T("属性"), 0, 80);

	//获取区段表
	IMAGE_SECTION_HEADER *pSection = IMAGE_FIRST_SECTION(m_pNt);
	//获取区段个数
	DWORD dwSectionNum = m_pNt->FileHeader.NumberOfSections;
	//遍历区段头表
	for (DWORD i = 0; i < dwSectionNum; i++, pSection++)
	{
		CString str;
		for (DWORD j = 0; j < IMAGE_SIZEOF_SHORT_NAME; j++)
		{
			CString strTmp;
			strTmp.Format(_T("%c"), pSection->Name[j]);
			str += strTmp;
		}
		m_ListCtrl.InsertItem(i, str);
		str.Format(_T("0x%x"), pSection->Misc.VirtualSize);
		m_ListCtrl.SetItemText(i, 1, str);
		str.Format(_T("0x%x"), pSection->VirtualAddress);
		m_ListCtrl.SetItemText(i, 2, str);
		str.Format(_T("0x%x"), pSection->SizeOfRawData);
		m_ListCtrl.SetItemText(i, 3, str);
		str.Format(_T("0x%x"), pSection->PointerToRawData);
		m_ListCtrl.SetItemText(i, 4, str);
		str.Format(_T("0x%x"), pSection->Characteristics);
		m_ListCtrl.SetItemText(i, 5, str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
