// CFiveStone_Client_MainDlg.h : ͷ�ļ�
//
#pragma once

#include "BattleDlg.h"
#include "RuleDlg.h"
// CFiveStone_Client_MainDlg �Ի���
class CFiveStone_Client_MainDlg : public CDialogEx
{
// ����
public:
	CFiveStone_Client_MainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FIVESTONE_CLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonPVE();
	afx_msg void OnBnClickedButtonRule();
};
