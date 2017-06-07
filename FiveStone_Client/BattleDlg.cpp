// BattleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveStone_Client.h"
#include "BattleDlg.h"
#include "afxdialogex.h"

#include <queue>

bool BattleType;                            //0为PVP，1为PVE
bool IsInvited;
bool IsMyStoneDown = false;
bool IsAIFirst = true;

BattleStatus battleStatus = MyTurn;        //当前对战状态
RePaintType rePaintType = NewGame;
StoneType stonecolor = Black;

CImage img_black, img_black_bs;
CImage img_white, img_white_bs;
CImage img_board;

std::deque<StonePos> MovesDeque;           //记录每步棋子的落子点
StoneType ChessBoard[19][19];              //记录棋盘的落子状况

CRect Rect;              //待重绘区域
CRect RegretRect[2];     //悔棋，待重绘区域

Negamax g_Negamax;

HANDLE hThread;

CMenu m_Menu;

int g_bm_xPos, g_bm_yPos;


// BattleDlg 对话框

IMPLEMENT_DYNAMIC(BattleDlg, CDialogEx)

BattleDlg::BattleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(BattleDlg::IDD, pParent)
	, Bat_Enemy_ID(_T(""))
	, Bat_Self_ID(_T(""))
	, Enemy_Totol_Time(0)
	, Enemy_SinStep_Time(0)
	, Self_Totol_Time(0)
	, Self_SinStep_Time(0)
{

}

BattleDlg::~BattleDlg()
{
}

void BattleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_ENEMY_ID, Bat_Enemy_ID);
	DDX_Text(pDX, IDC_STATIC_SELF_ID, Bat_Self_ID);
	DDX_Text(pDX, IDC_EDIT_ENEMY_TOTOL_TIME, Enemy_Totol_Time);
	DDX_Text(pDX, IDC_EDIT_ENEMY_ONESTEP_TIME, Enemy_SinStep_Time);
	DDX_Text(pDX, IDC_EDIT_SELF_TOTOL_TIME, Self_Totol_Time);
	DDX_Text(pDX, IDC_EDIT_SELF_ONESTEP_TIME, Self_SinStep_Time);
}


BEGIN_MESSAGE_MAP(BattleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EXIT_PVE, &BattleDlg::OnExitPVE)
	ON_COMMAND(ID_NEWGAME, &BattleDlg::OnNewGame)
	ON_COMMAND(ID_REGRET, &BattleDlg::OnRegret)
	ON_COMMAND(ID_PC_BLACK, &BattleDlg::OnPcBlack)
	ON_COMMAND(ID_PC_WHITE, &BattleDlg::OnPcWhite)
END_MESSAGE_MAP()


// BattleDlg 消息处理程序

//初始化对战窗口
BOOL BattleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//加载图片资源
	CImage *pImg;

	if (!img_board.IsNull())
		img_board.Destroy();
	img_board.Load(_T("res//ChessBoard.png"));

	if (!img_black.IsNull())
		img_black.Destroy();
	img_black.Load(_T("res//black.png"));
	pImg = &img_black;
	PreMultipliedAlpha(&pImg);

	if (!img_white.IsNull())
		img_white.Destroy();
	img_white.Load("res//white.png");
	pImg = &img_white;
	PreMultipliedAlpha(&pImg);

	if (!img_black_bs.IsNull())
		img_black_bs.Destroy();
	img_black_bs.Load("res//black_bs.png");
	pImg = &img_black_bs;
	PreMultipliedAlpha(&pImg);

	if (!img_white_bs.IsNull())
		img_white_bs.Destroy();
	img_white_bs.Load("res//white_bs.png");
	pImg = &img_white_bs;
	PreMultipliedAlpha(&pImg);


	//根据对战类型作相应设置
	bool flag = true;
	CString cstr_title;


	switch (BattleType)
	{
	case PVE:
		SetWindowText("人机对战");
		Bat_Enemy_ID = "电脑AI";
		Bat_Self_ID = "自己";
		UpdateData(false);
		
		m_Menu.LoadMenu(IDR_MENU1);
		SetMenu(&m_Menu);
		DrawMenuBar();
		break;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//窗口重绘消息响应，**********************注意响应的是所有重绘消息，包括移动窗口，窗口遮挡等引起的重绘
void BattleDlg::OnPaint()
{
	CPaintDC dc(this);
	
	switch (rePaintType)
	{
	case NewGame:
		InitGame(&dc);
		break;

	case StoneDown:		
		//仅当且接收到落子消息时，即当IsMyStoneDown = true时，调用DrawStone();
		//当 rePaintType = StoneDown 且因类似移动窗口引起的重绘时，则不调用DrawStone();
		if (IsMyStoneDown)                                      
		{
			DrawStone(&dc, Rect, stonecolor);
			IsMyStoneDown = false;
			if (IsGameOver(MovesDeque.back()))
			{
				if (battleStatus == GameOver)
				{
					if (stonecolor == Black)
						AfxMessageBox("白棋获胜！");
					else
						AfxMessageBox("黑棋获胜！");
				}		
			}	

			if (battleStatus == EnemyTurn)
				battleStatus = MyTurn;                                               //轮到自己落子
			else if (battleStatus == MyTurn)
				battleStatus = EnemyTurn;

			if (BattleType == PVE && battleStatus == EnemyTurn)
			{
				_beginthreadex(NULL, 0, &AIProc, (void*)(this), 0, NULL);
			}
		}
		break;

	case Regret:
		RegretProc(&dc);
		rePaintType = StoneDown;
		break;
	default:
		break;
	}

	CDialogEx::OnPaint();
}

//初始化游戏
void BattleDlg::InitGame(CPaintDC *pDC)
{
	img_board.Draw(pDC->m_hDC, 0, 0, 566, 566);

	MovesDeque.clear();

	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			ChessBoard[i][j] = Empty;

	rePaintType = StoneDown;

	//根据设置，修改各变量的初始值
	if (BattleType == PVE)
	{
		g_Negamax.SetSearchDepth( 4 );
		if (IsAIFirst)
			battleStatus = EnemyTurn;
		else
			battleStatus = MyTurn;

		stonecolor = Black;
		img_white_bs.Draw(pDC->m_hDC, 640, 60, 40, 40);
		img_black_bs.Draw(pDC->m_hDC, 640, 330, 40, 40);

		if (IsAIFirst)
		{
			IsMyStoneDown = true;
			StonePos pos;

			pos.xPos = 9;
			pos.yPos = 9;

			MovesDeque.push_back(pos);
			ChessBoard[pos.xPos][pos.yPos] = stonecolor;

			Rect.left = 29 * pos.xPos;
			Rect.top = 29 * pos.yPos;
			Rect.right = 29 * pos.xPos + StoneSize;
			Rect.bottom = 29 * pos.yPos + StoneSize;
			InvalidateRect(&Rect);
			UpdateWindow();
		}
	}
}

//窗口关闭消息响应
void BattleDlg::OnClose()
{

	if (!img_board.IsNull())
		img_board.Destroy();
	if (!img_black.IsNull())
		img_black.Destroy();
	if (!img_white.IsNull())
		img_white.Destroy();

	rePaintType = NewGame;

	OnCancel();
}

//鼠标左键点击消息响应
void BattleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (battleStatus == MyTurn)
	{
		StonePos stone;

		//落子成功，则将落子点添加到落子队列中,发送重绘消息
		if (CheckStoneDown(stone, point.x, point.y))
		{
			IsMyStoneDown = true;

			MovesDeque.push_back(stone);
			ChessBoard[stone.xPos][stone.yPos] = stonecolor;

			Rect.left = 29 * stone.xPos;
			Rect.top = 29 * stone.yPos;
			Rect.right = 29 * stone.xPos + StoneSize;
			Rect.bottom = 29 * stone.yPos + StoneSize;
			InvalidateRect(&Rect);	
			UpdateWindow();
		}		
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

//退出PVE
void BattleDlg::OnExitPVE()
{
	if (!img_board.IsNull())
		img_board.Destroy();
	if (!img_black.IsNull())
		img_black.Destroy();
	if (!img_white.IsNull())
		img_white.Destroy();

	rePaintType = NewGame;

	OnCancel();
}

//点击“新游戏”
void BattleDlg::OnNewGame()
{
	rePaintType = NewGame;
	Invalidate();
	UpdateWindow();
}

//点击“悔棋”
void BattleDlg::OnRegret()
{
	//仅当自己回合，且盘面非空时，悔棋有效
	if (battleStatus == MyTurn && !MovesDeque.empty())
	{

		int xpos = MovesDeque.back().xPos;
		int ypos = MovesDeque.back().yPos;
		ChessBoard[xpos][ypos] = Empty;
		RegretRect[0].left = 29 * xpos;
		RegretRect[0].top = 29 * ypos;
		RegretRect[0].right = 29 * xpos + StoneSize;
		RegretRect[0].bottom = 29 * ypos + StoneSize;
		MovesDeque.pop_back();

		xpos = MovesDeque.back().xPos;
		ypos = MovesDeque.back().yPos;
		ChessBoard[xpos][ypos] = Empty;
		RegretRect[1].left = 29 * xpos;
		RegretRect[1].top = 29 * ypos;
		RegretRect[1].right = 29 * xpos + StoneSize;
		RegretRect[1].bottom = 29 * ypos + StoneSize;
		MovesDeque.pop_back();

		rePaintType = Regret;
		InvalidateRect(&RegretRect[0]);
		InvalidateRect(&RegretRect[1]);
		UpdateWindow();
	}
}

//对PNG图片进行Alpha预乘,还原其透明显示
void PreMultipliedAlpha(CImage **ppImg)
{
	for (int i = 0; i < (**ppImg).GetWidth(); i++)               
	{
		for (int j = 0; j < (**ppImg).GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>((**ppImg).GetPixelAddress(i, j));
			pucColor[0] = (pucColor[0] * pucColor[3] + 127) / 255;
			pucColor[1] = (pucColor[1] * pucColor[3] + 127) / 255;
			pucColor[2] = (pucColor[2] * pucColor[3] + 127) / 255;
		}
	}
}

//验证是否为有效落子
bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos)
{
	if (xpos <= ChessBoardSize && ypos <= ChessBoardSize)
	{
		//计算大致的落子点
		stone.xPos = xpos / StoneSize;
		stone.yPos = ypos / StoneSize;
		if (ChessBoard[stone.xPos][stone.yPos] != Empty)
			return false;

		int xUp = 20 + 29 * stone.xPos;
		int xLow = 10 + 29 * stone.xPos;
		int yUp = 20 + 29 * stone.yPos;
		int yLow = 10 + 29 * stone.yPos;

		//确实落点是否位于可落子点范围内
		if (xpos >= xLow && xpos <= xUp &&
			ypos >= yLow && ypos <= yUp)
			return true;
		else
			return false;
	}
	else
		return false;
}

//落子重绘
void DrawStone(CPaintDC *pDC, const CRect rect, StoneType &color)
{
	//先重绘棋盘在Rect区域部分
	img_board.Draw(pDC->m_hDC, Rect, Rect);

	//再再该区域绘制棋子
	if (color == Black)
	{
		img_black.Draw(pDC->m_hDC, rect);
		color = White;
	}		
	else if (color == White)
	{
		img_white.Draw(pDC->m_hDC, rect);
		color = Black;
	}
		
	//最后在棋子上绘制当前下棋步骤
	CString str;
	int steps = MovesDeque.size();
	str.Format(_T("%d"), steps);

	CFont font;
	CFont *pOldFont;                      // 保存设备上下文最初使用的字体对象
	CRect stepRect;
	font.CreatePointFont(100, "隶书");
	pOldFont = pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);          // 更改当前画笔

	//在stepRect内，落子步骤
	stepRect.left = Rect.left;
	stepRect.top = Rect.top + 7;
	stepRect.right = Rect.right;
	stepRect.bottom = Rect.bottom;

	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->DrawText(str, &stepRect, DT_CENTER);
	pDC->SelectObject(pOldFont);          //还原画笔
}

//悔棋处理
void RegretProc(CPaintDC *pDC)
{
	//先重绘棋盘在Rect区域部分
	img_board.Draw(pDC->m_hDC, RegretRect[0], RegretRect[0]);
	img_board.Draw(pDC->m_hDC, RegretRect[1], RegretRect[1]);

	//if (stonecolor == Black)
	//	stonecolor = White;
	//else
	//	stonecolor = Black;
}

//计算在某方向上与落子相连且颜色相同的棋子数
int  CountOfStones(StonePos &pos, Direction dir)
{
	int i = 1, count = 1;
	bool block[2];
	block[0] = false;
	block[1] = false;

	while (1)
	{ 
		if (pos.xPos - i < 0 || pos.yPos - i < 0)
			block[0] = true;
		if (pos.xPos + i > GRID_NUM - 1 || pos.yPos + i > GRID_NUM - 1)
			block[1] = true;

		switch (dir)
		{
		case L2R:
			if (!block[0] || !block[1])
			{
				if (!block[0] && ChessBoard[pos.xPos - i][pos.yPos] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[0] = true;

				if (!block[1] && ChessBoard[pos.xPos + i][pos.yPos] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[1] = true;
			}
			break;
		case U2D:
			if (!block[0] || !block[1])
			{
				if (!block[0] && ChessBoard[pos.xPos][pos.yPos - i] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[0] = true;

				if (!block[1] && ChessBoard[pos.xPos][pos.yPos + i] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[1] = true;
			}
			break;
		case LU2RD:
			if (!block[0] || !block[1])
			{
				if (!block[0] && ChessBoard[pos.xPos - i][pos.yPos - i] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[0] = true;

				if (!block[1] && ChessBoard[pos.xPos + i][pos.yPos + i] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[1] = true;
			}
			break;
		case LD2RU:
			if (!block[0] || !block[1])
			{
				if (!block[0] && ChessBoard[pos.xPos - i][pos.yPos + i] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[0] = true;

				if (!block[1] && ChessBoard[pos.xPos + i][pos.yPos - i] == ChessBoard[pos.xPos][pos.yPos])
					count++;
				else
					block[1] = true;
			}
			break;
		default:
			break;
		}
		if (block[0] && block[1])
			break;
		else
			i++;
	}

	return count;
}

//判断游戏是否结束，结束返回true，否则返回false
bool IsGameOver(StonePos pos)
{
	//左右方向、上下方向、左上右下方向、左下右上方向
	if (CountOfStones(pos, L2R) >= 5 ||
		CountOfStones(pos, U2D) >= 5 ||
		CountOfStones(pos, LU2RD) >= 5 ||
		CountOfStones(pos, LD2RU) >= 5)
	{
		battleStatus = GameOver;
		return true;
	}
	return false;
}

void InvertChessBoard()
{
	int x, y;
	for (x = 0; x< GRID_NUM; x++)
	{
		for (y = 0; y < GRID_NUM; y++)
		{
			switch (ChessBoard[x][y])
			{
			case White:
				ChessBoard[x][y] = Black;
				break;
			case Black:
				ChessBoard[x][y] = White;
				break;
			default:
				break;
			}
		}
	}
}

//
//void BattleDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	int m_len = sizeof(FS_MSG);
//	FS_MSG *msg = new FS_MSG;
//	memset(msg, 0, m_len);
//
//	int pos_len = sizeof(Position);
//	Position * pos_ptr = new Position;
//	memset(pos_ptr, 0, pos_len);
//
//	int ret;
//	CString status, id;
//	memset((void*)msg, 0, m_len);
//	ret = recv(g_cli_socket, (char*)msg, m_len, 0);
//
//	if (ret > 0)
//	{
//		if (msg->cmd == FS_BATTLE)
//		{
//
//		}
//	}
//	else if (ret == SOCKET_ERROR)//socket退出
//	{
//		CString cstr;
//		cstr.Format("socket退出，用户登录信息接收错误，错误号为：%d", WSAGetLastError());
//		AfxMessageBox(cstr);
//	}
//	else//出现网络错误，接收失败
//	{
//		CString cstr;
//		cstr.Format("网络错误，用户登录信息接收错误，错误号为：%d", WSAGetLastError());
//		AfxMessageBox(cstr);
//	}
//
//	//收尾
//	delete msg;
//	delete pos_ptr;
//
//	CDialogEx::OnTimer(nIDEvent);
//}

unsigned int __stdcall AIProc(void* pParam)
{
	IsMyStoneDown = true;
	StonePos pos;
	BattleDlg *dlg = (BattleDlg*)pParam;

	if (!IsAIFirst)
		InvertChessBoard();
	g_Negamax.SearchAGoodMove(ChessBoard, -1);
	if (!IsAIFirst)
		InvertChessBoard();

	pos.xPos = g_bm_xPos;
	pos.yPos = g_bm_yPos;


	MovesDeque.push_back(pos);
	ChessBoard[pos.xPos][pos.yPos] = stonecolor;

	Rect.left = 29 * pos.xPos;
	Rect.top = 29 * pos.yPos;
	Rect.right = 29 * pos.xPos + StoneSize;
	Rect.bottom = 29 * pos.yPos + StoneSize;
	dlg->InvalidateRect(&Rect);
	dlg->UpdateWindow();

	return 0;
}

void BattleDlg::OnPcBlack()
{
	CMenu *pMenu = m_Menu.GetSubMenu(1)->GetSubMenu(0);
	if (pMenu->GetMenuState(1, MF_BYPOSITION) == MF_CHECKED)
	{
		pMenu->CheckMenuItem(0, MF_CHECKED | MF_BYPOSITION);
		pMenu->CheckMenuItem(1, MF_UNCHECKED | MF_BYPOSITION);
		IsAIFirst = true;
	}
}


void BattleDlg::OnPcWhite()
{
	CMenu *pMenu = m_Menu.GetSubMenu(1)->GetSubMenu(0);

	if (pMenu->GetMenuState(0, MF_BYPOSITION) == MF_CHECKED)
	{
		pMenu->CheckMenuItem(1, MF_CHECKED | MF_BYPOSITION);
		pMenu->CheckMenuItem(0, MF_UNCHECKED | MF_BYPOSITION);
		IsAIFirst = false;
	}
}
