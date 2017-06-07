
// CFiveStone_Client_MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveStone_Client.h"
#include "CFiveStone_Client_MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CImage img;

// CFiveStone_Client_MainDlg 对话框

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


// CFiveStone_Client_MainDlg 消息处理程序

BOOL CFiveStone_Client_MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFiveStone_Client_MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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
			//背景加载错误
			AfxMessageBox("背景图片加载错误！");
		}

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

