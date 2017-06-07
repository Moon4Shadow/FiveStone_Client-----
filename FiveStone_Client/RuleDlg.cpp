// RuleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiveStone_Client.h"
#include "RuleDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>

// RuleDlg �Ի���

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


// RuleDlg ��Ϣ�������

BOOL RuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ȡrule�ı��ļ�
	CFile file("res//rule.txt", CFile::modeRead);
	char *pBuf;
	int iLen = file.GetLength();
	pBuf = (char *)malloc(sizeof(char) * (iLen + 1));
	file.Read(pBuf, iLen);
	pBuf[iLen] = 0;
	file.Close();

	//�����ֽ��ַ��ı�תΪ���ֽ��ı�
	//wchar_t *wpBuf;
	//wpBuf = (wchar_t *)malloc(sizeof(wchar_t) * (iLen+1) );
	//MultiByteToWideChar(CP_ACP, 0, pBuf, iLen + 1, wpBuf, iLen+1);

	//size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)pBuf, iLen+1, NULL, 0);
	//wchar_t *wpBuf;
	//wpBuf = (wchar_t *)malloc(sizeof(wchar_t) * (wlen + 1));
	//MultiByteToWideChar(CP_ACP, 0, pBuf, iLen + 1, wpBuf, iLen + 1);

	//����
	Edit_Rule_Ctrl.SetWindowText(pBuf);

	//����ʹ����ϣ��ͷ��ڴ棬�����ڴ�й¶
	free(pBuf);
	//free(wpBuf);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void RuleDlg::OnBnClickedButtonClose()
{
	OnCancel();
}
