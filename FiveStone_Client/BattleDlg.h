#pragma once

#include "Global.h"
#include "Negamax.h"
#include "afxwin.h"

#define PVP false
#define PVE true

#define StoneSize 29
#define ChessBoardSize 550

enum Direction
{
	L2R = 1,
	U2D,
	LU2RD,
	LD2RU
};

enum BattleStatus
{
	MyTurn = 1,
	EnemyTurn,
	GameOver,
	QuitOver
};

extern bool BattleType;
extern bool IsInvited;
extern CImage img_black;
extern CImage img_white;
extern CImage img_board;


// BattleDlg 对话框

class BattleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BattleDlg)

public:
	BattleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BattleDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_BATTLE };


	//CSearchEngine *m_pSE;
	//初始化游戏
	void InitGame(CPaintDC *pDC);


protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExitPVE();

	afx_msg void OnNewGame();
	afx_msg void OnRegret();
	CString Bat_Enemy_ID;
	CString Bat_Self_ID;
	double Enemy_Totol_Time;
	double Enemy_SinStep_Time;
	double Self_Totol_Time;
	double Self_SinStep_Time;
	afx_msg void OnPcBlack();
	afx_msg void OnPcWhite();
};





//验证落子有效性
bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos);
//Alpha预乘
void PreMultipliedAlpha(CImage **ppImg);
//在特定的矩形区域内绘制棋子
void DrawStone(CPaintDC *pDC, const CRect rect, StoneType &color);
//悔棋处理
void RegretProc(CPaintDC *pDC);
//计算某位置棋子在不同方向上的最大同色棋子相连数
int  CountOfStones(StonePos &pos, Direction dir);
//判断对弈是否结束
bool IsGameOver(StonePos pos);
//黑白翻转
void InvertChessBoard();


unsigned int __stdcall AIProc(void* pParam);