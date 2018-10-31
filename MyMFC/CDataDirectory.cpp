// CDataDirectory.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CDataDirectory.h"
#include "afxdialogex.h"

typedef struct TypeOffset
{
	WORD offset : 12; // (1) 大小为12Bit的重定位偏移
	WORD type : 4;// (2) 大小为4Bit的重定位信息类型值
}TypeOffset;


// CDataDirectory 对话框

IMPLEMENT_DYNAMIC(CDataDirectory, CDialogEx)

CDataDirectory::CDataDirectory(LPBYTE pBuff, IMAGE_NT_HEADERS *pNt, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTORY_DLG, pParent)
	, m_pNt(pNt)
	, m_pBuff(pBuff)
{

}

CDataDirectory::~CDataDirectory()
{
}

void CDataDirectory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CDataDirectory, CDialogEx)
END_MESSAGE_MAP()


// CDataDirectory 消息处理程序


BOOL CDataDirectory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//获取数据目录表
	IMAGE_DATA_DIRECTORY *pData = (IMAGE_DATA_DIRECTORY*)(m_pNt->OptionalHeader.DataDirectory);
	HTREEITEM hRoot = m_treeCtrl.InsertItem(L"数据目录表", TVI_ROOT, TVI_LAST);
	WCHAR *szName[16] = { L"导出表", L"导入表" , L"资源表", L"异常程序处理表", L"安全表"
		L"重定位表" , L"调试信息表" , L"版权" , L"全局指针" , L"TLS表", L"配置表",
		L"输入目录表", L"导入地址表" , L"延迟载入表" , L"COM信息" };
	CString str;
	//输出各种表的RVA及大小
	for (DWORD i = 0; i < m_pNt->OptionalHeader.NumberOfRvaAndSizes; i++, pData++)
	{
		//如果为0说明没有这个表
		if (pData->VirtualAddress == 0 && pData->Size == 0)
		{
			continue;
		}
		HTREEITEM hChild = m_treeCtrl.InsertItem(szName[i], hRoot, TVI_LAST);
		HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"RVA", hChild, TVI_LAST);
		str.Format(L"0x%x", pData->VirtualAddress);
		m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);

		HTREEITEM hChild2 = m_treeCtrl.InsertItem(L"Size", hChild, TVI_LAST);
		str.Format(L"0x%x", pData->Size);
		m_treeCtrl.InsertItem(str, hChild2, TVI_LAST);
	}

	//导出表
	HTREEITEM hRoot1 = m_treeCtrl.InsertItem(L"导出表", TVI_ROOT, TVI_LAST);
	//先获得导出表的RVA
	DWORD expRva = m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//RVA转成文件偏移
	DWORD expFoa = RvaToFoa(expRva);
	if (expFoa != -1)
	{
		//获取导出表
		IMAGE_EXPORT_DIRECTORY *pExport = NULL;
		pExport = (IMAGE_EXPORT_DIRECTORY*)(m_pBuff + expFoa);

		HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"函数数量", hRoot1, TVI_LAST);
		str.Format(L"0x%x", pExport->NumberOfFunctions);
		m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);

		HTREEITEM hChild2 = m_treeCtrl.InsertItem(L"函数名称数量", hRoot1, TVI_LAST);
		str.Format(L"0x%x", pExport->NumberOfNames);
		m_treeCtrl.InsertItem(str, hChild2, TVI_LAST);

		HTREEITEM hChild3 = m_treeCtrl.InsertItem(L"函数地址表", hRoot1, TVI_LAST);
		HTREEITEM hChild4 = m_treeCtrl.InsertItem(L"函数名称表", hRoot1, TVI_LAST);
		HTREEITEM hChild5 = m_treeCtrl.InsertItem(L"函数序号表", hRoot1, TVI_LAST);
		DWORD *pEAT = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + m_pBuff);
		DWORD *pENT = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + m_pBuff);
		WORD *pEOT = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + m_pBuff);
		for (DWORD i = 0; i < pExport->NumberOfNames; i++)
		{
			str.Format(L"0x%x", pEAT[i]);
			m_treeCtrl.InsertItem(str, hChild3, TVI_LAST);

			CHAR *szCh= (CHAR*)(RvaToFoa(pENT[i]) + m_pBuff);
			USES_CONVERSION;
			m_treeCtrl.InsertItem(A2W(szCh), hChild4, TVI_LAST);

			str.Format(L"%d", pEOT[i]);
			m_treeCtrl.InsertItem(str, hChild5, TVI_LAST);
		}
	}

	//导入表
	HTREEITEM hRoot2 = m_treeCtrl.InsertItem(L"导入表", TVI_ROOT, TVI_LAST);
	//先获得导入表的RVA
	DWORD impRva = m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//转换成FOA
	DWORD impFoa = RvaToFoa(impRva);
	if (impFoa != -1)
	{
		//获取导入表的首地址
		IMAGE_IMPORT_DESCRIPTOR *pImp = NULL;
		pImp = (IMAGE_IMPORT_DESCRIPTOR*)(m_pBuff + impFoa);

		//以0作为结尾
		while (pImp->Name != 0)
		{
			//Name是一个RVA转为FOA
			CHAR* szName = (CHAR*)(RvaToFoa(pImp->Name) + m_pBuff);
			//将名字加入树控件
			HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"DLL", hRoot2, TVI_LAST);
			USES_CONVERSION;
			m_treeCtrl.InsertItem(A2W(szName), hChild1, TVI_LAST);

			//获取INT表首地址，也是一个RVA需要转FOA
			IMAGE_THUNK_DATA *pInt = (IMAGE_THUNK_DATA*)(RvaToFoa(pImp->OriginalFirstThunk) + m_pBuff);
			while (pInt->u1.Function != 0)
			{
				//先判断导入的方式，以序号还是名称
				if (IMAGE_SNAP_BY_ORDINAL(pInt->u1.Ordinal))
				{
					str.Format(L"0x%04x", pInt->u1.Ordinal & 0xffff);
					m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);
				}
				else
				{
					IMAGE_IMPORT_BY_NAME *pIntName = (IMAGE_IMPORT_BY_NAME*)
						(RvaToFoa(pInt->u1.AddressOfData) + m_pBuff);
					USES_CONVERSION;
					m_treeCtrl.InsertItem(A2W(pIntName->Name), hChild1, TVI_LAST);
				}
				pInt++;
			}

			pImp++;
		}
	}

	//资源表
	hRoot3 = m_treeCtrl.InsertItem(L"资源表", TVI_ROOT, TVI_LAST);
	//先获取资源表RVA
	DWORD resRva = m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//转换FOA
	DWORD resFoa = RvaToFoa(resRva);
	if (resFoa != -1)
	{
		//获取资源表
		IMAGE_RESOURCE_DIRECTORY *pRes = NULL;
		pRes = (IMAGE_RESOURCE_DIRECTORY*)(m_pBuff + resFoa);
		ParseResources((LPBYTE)pRes, pRes);

	}

	//重定位表
	HTREEITEM hRoot4 = m_treeCtrl.InsertItem(L"重定位表", TVI_ROOT, TVI_LAST);
	//获取重定位表RVA
	DWORD reloRva = m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	DWORD reloFoa = RvaToFoa(reloRva);
	if (reloFoa != -1)
	{
		IMAGE_BASE_RELOCATION* pRelo = (IMAGE_BASE_RELOCATION*)
			(m_pBuff + reloFoa);
		while (pRelo->VirtualAddress != 0)
		{
			//获取typeoffset地址和数量
			TypeOffset *pTypeOffset = (TypeOffset *)(pRelo + 1);
			DWORD dwCount = (pRelo->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
			
			HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"页首RVA", hRoot4, TVI_LAST);
			str.Format(L"0x%x", pRelo->VirtualAddress);
			m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);

			for (DWORD i = 0; i < dwCount; ++i)
			{
				m_treeCtrl.InsertItem(L"页内偏移", hChild1, TVI_LAST);
				str.Format(L"0x%x", pTypeOffset[i].offset);
				m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);
			}
			pRelo = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelo + pRelo->SizeOfBlock);
		}
	}

	//延迟加载表
	 m_treeCtrl.InsertItem(L"延迟加载表", TVI_ROOT, TVI_LAST);

	//TLS表
	HTREEITEM hRoot5 = m_treeCtrl.InsertItem(L"TLS表", TVI_ROOT, TVI_LAST);
	//获取TLS表RVA
	DWORD tlsRva = m_pNt->OptionalHeader.DataDirectory[9].VirtualAddress;
	DWORD tlsFoa = RvaToFoa(tlsRva);
	if (tlsFoa != -1)
	{
		IMAGE_TLS_DIRECTORY *pTls = (IMAGE_TLS_DIRECTORY*)
			(m_pBuff + tlsFoa);
		m_treeCtrl.InsertItem(L"源数据起始位置", hRoot5, TVI_LAST);
		str.Format(L"0x%x", pTls->StartAddressOfRawData);
		m_treeCtrl.InsertItem(str, hRoot5, TVI_LAST);

		m_treeCtrl.InsertItem(L"源数据终止位置", hRoot5, TVI_LAST);
		str.Format(L"0x%x", pTls->EndAddressOfRawData);
		m_treeCtrl.InsertItem(str, hRoot5, TVI_LAST);

		m_treeCtrl.InsertItem(L"回调函数地址表位置", hRoot5, TVI_LAST);
		str.Format(L"0x%x", pTls->AddressOfCallBacks);
		m_treeCtrl.InsertItem(str, hRoot5, TVI_LAST);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//相对虚拟地址转文件偏移
DWORD CDataDirectory::RvaToFoa(DWORD dwRva)
{
	IMAGE_SECTION_HEADER *pSection = (IMAGE_SECTION_HEADER*)IMAGE_FIRST_SECTION(m_pNt);
	for (DWORD i = 0; i < m_pNt->FileHeader.NumberOfSections; i++)
	{
		if (dwRva >= pSection[i].VirtualAddress && dwRva <= pSection[i].VirtualAddress
			+ pSection[i].SizeOfRawData)
		{
			return dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return -1;
}

void CDataDirectory::ParseResources(LPBYTE pResRoot, IMAGE_RESOURCE_DIRECTORY* pRes, int nLevel)
{
	//先获取这一层资源的总个数
	DWORD dwEntryCount = pRes->NumberOfIdEntries + pRes->NumberOfNamedEntries;

	//获取到这一层目录入口的首地址
	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry = NULL;
	pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRes + 1);
	CString str;
	if (nLevel == 1)
	{
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			//判断id是不是一个字符串 为1是字符串
			if (pEntry[i].NameIsString)
			{
				IMAGE_RESOURCE_DIR_STRING_U *pStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].OffsetToData + pResRoot);
				hResChild1 = m_treeCtrl.InsertItem(L"资源名称", hRoot3, TVI_LAST);
				m_treeCtrl.InsertItem(pStr->NameString, hResChild1, TVI_LAST);
			}
			else/*id是一个整形*/
			{
				char* pIdName[] =
				{
					"",
					"鼠标指针（Cursor）",
					"位图（Bitmap）",
					"图标（Icon）",
					"菜单（Menu）",
					"对话框（Dialog）",
					"字符串列表（String Table）",
					"字体目录（Font Directory）",
					"字体（Font）",
					"快捷键（Accelerators）",
					"非格式化资源（Unformatted）",
					"消息列表（Message Table）",
					"鼠标指针组（Group Cursor）",
					"",
					"图标组（Group Icon）",
					"",
					"版本信息（Version Information）"
				};
				USES_CONVERSION;
				if (pEntry[i].Id >= _countof(pIdName))
				{
					str.Format(L"%d", pEntry[i].Id);
					hResChild1 = m_treeCtrl.InsertItem(L"资源ID", hRoot3, TVI_LAST);
					m_treeCtrl.InsertItem(str, hResChild1, TVI_LAST);
				}
				else
				{
					hResChild1 = m_treeCtrl.InsertItem(L"资源ID", hRoot3, TVI_LAST);
					m_treeCtrl.InsertItem(A2W(pIdName[pEntry[i].Id]), hResChild1, TVI_LAST);
				}
			}
			//以资源根目录为偏移，找到下一层目录
			IMAGE_RESOURCE_DIRECTORY *pRes2 = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResRoot);
			ParseResources(pResRoot, pRes2, 2);
		}
	}
	else if (nLevel == 2)
	{
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			if (pEntry[i].NameIsString)
			{
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResRoot);
				hResChild2 = m_treeCtrl.InsertItem(L"资源ID", hResChild1, TVI_LAST);
				m_treeCtrl.InsertItem(pIdStr->NameString, hResChild2, TVI_LAST);
			}
			else
			{
				str.Format(L"%d", pEntry[i].Id);
				hResChild2 = m_treeCtrl.InsertItem(L"资源ID", hResChild1, TVI_LAST);
				m_treeCtrl.InsertItem(str, hResChild2, TVI_LAST);
			}
			// 2. 偏移(以资源根目录偏移, 偏移到第三层)
			IMAGE_RESOURCE_DIRECTORY* pRes3 = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResRoot);
			// 找到第3层目录的首地址
			ParseResources(pResRoot, pRes3, 3);
		}
	}
	else if (nLevel == 3)
	{
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// 解析第三层
			// 1. id(没有什么用)
			// 2. 偏移(以资源根目录为偏移, 偏移到数据入口)
			IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)
				(pEntry[i].OffsetToData + pResRoot);
			HTREEITEM hResChild3 = m_treeCtrl.InsertItem(L"资源大小和位置", hResChild2, TVI_LAST);
			str.Format(L"%d", pDataEntry->Size);
			m_treeCtrl.InsertItem(str, hResChild3, TVI_LAST);
			str.Format(L"%d", pDataEntry->OffsetToData);
			m_treeCtrl.InsertItem(str, hResChild3, TVI_LAST);
		}
	}
}
