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


// BattleDlg �Ի���

class BattleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BattleDlg)

public:
	BattleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BattleDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_BATTLE };


	//CSearchEngine *m_pSE;
	//��ʼ����Ϸ
	void InitGame(CPaintDC *pDC);


protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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





//��֤������Ч��
bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos);
//AlphaԤ��
void PreMultipliedAlpha(CImage **ppImg);
//���ض��ľ��������ڻ�������
void DrawStone(CPaintDC *pDC, const CRect rect, StoneType &color);
//���崦��
void RegretProc(CPaintDC *pDC);
//����ĳλ�������ڲ�ͬ�����ϵ����ͬɫ����������
int  CountOfStones(StonePos &pos, Direction dir);
//�ж϶����Ƿ����
bool IsGameOver(StonePos pos);
//�ڰ׷�ת
void InvertChessBoard();


unsigned int __stdcall AIProc(void* pParam);