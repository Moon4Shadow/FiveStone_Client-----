#pragma once
#include "afxwin.h"


// RuleDlg 对话框

class RuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RuleDlg)

public:
	RuleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RuleDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
private:
	CEdit Edit_Rule_Ctrl;
};
