// RuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveStone_Client.h"
#include "RuleDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>

// RuleDlg 对话框

IMPLEMENT_DYNAMIC(RuleDlg, CDialogEx)

RuleDlg::RuleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(RuleDlg::IDD, pParent)
{

}

RuleDlg::~RuleDlg()
{
}

void RuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RULE, Edit_Rule_Ctrl);
}


BEGIN_MESSAGE_MAP(RuleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &RuleDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// RuleDlg 消息处理程序

BOOL RuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//读取rule文本文件
	CFile file("res//rule.txt", CFile::modeRead);
	char *pBuf;
	int iLen = file.GetLength();
	pBuf = (char *)malloc(sizeof(char) * (iLen + 1));
	file.Read(pBuf, iLen);
	pBuf[iLen] = 0;
	file.Close();

	//将多字节字符文本转为宽字节文本
	//wchar_t *wpBuf;
	//wpBuf = (wchar_t *)malloc(sizeof(wchar_t) * (iLen+1) );
	//MultiByteToWideChar(CP_ACP, 0, pBuf, iLen + 1, wpBuf, iLen+1);

	//size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)pBuf, iLen+1, NULL, 0);
	//wchar_t *wpBuf;
	//wpBuf = (wchar_t *)malloc(sizeof(wchar_t) * (wlen + 1));
	//MultiByteToWideChar(CP_ACP, 0, pBuf, iLen + 1, wpBuf, iLen + 1);

	//更新
	Edit_Rule_Ctrl.SetWindowText(pBuf);

	//数据使用完毕，释放内存，避免内存泄露
	free(pBuf);
	//free(wpBuf);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void RuleDlg::OnBnClickedButtonClose()
{
	OnCancel();
}
