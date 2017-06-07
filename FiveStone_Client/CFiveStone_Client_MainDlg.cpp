
// CFiveStone_Client_MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiveStone_Client.h"
#include "CFiveStone_Client_MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CImage img;

// CFiveStone_Client_MainDlg �Ի���

CFiveStone_Client_MainDlg::CFiveStone_Client_MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFiveStone_Client_MainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFiveStone_Client_MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFiveStone_Client_MainDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CFiveStone_Client_MainDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_PVE, &CFiveStone_Client_MainDlg::OnBnClickedButtonPVE)
	ON_BN_CLICKED(IDC_BUTTON_RULE, &CFiveStone_Client_MainDlg::OnBnClickedButtonRule)
END_MESSAGE_MAP()


// CFiveStone_Client_MainDlg ��Ϣ�������

BOOL CFiveStone_Client_MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFiveStone_Client_MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		if (!img.IsNull()) 
			img.Destroy();
		img.Load("res//background.png");
		if (!img.IsNull())
		{
			//CRect rect;
			img.Draw(dc.m_hDC, 0, 0, 500, 312);
		}
		else
		{
			//�������ش���
			AfxMessageBox("����ͼƬ���ش���");
		}

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFiveStone_Client_MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFiveStone_Client_MainDlg::OnBnClickedButtonExit()
{
	OnCancel();
}

void CFiveStone_Client_MainDlg::OnClose()
{
	OnCancel();
}


void CFiveStone_Client_MainDlg::OnBnClickedButtonPVE()
{
	BattleType = PVE;

	BattleDlg dlg;
	dlg.DoModal();
}


void CFiveStone_Client_MainDlg::OnBnClickedButtonRule()
{
	RuleDlg dlg;
	dlg.DoModal();
}

