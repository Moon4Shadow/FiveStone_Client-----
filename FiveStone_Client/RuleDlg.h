#pragma once
#include "afxwin.h"


// RuleDlg �Ի���

class RuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RuleDlg)

public:
	RuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RuleDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
private:
	CEdit Edit_Rule_Ctrl;
};
