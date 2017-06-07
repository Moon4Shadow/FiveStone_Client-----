// BattleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiveStone_Client.h"
#include "BattleDlg.h"
#include "afxdialogex.h"

#include <queue>

bool BattleType;                            //0ΪPVP��1ΪPVE
bool IsInvited;
bool IsMyStoneDown = false;
bool IsAIFirst = true;

BattleStatus battleStatus = MyTurn;        //��ǰ��ս״̬
RePaintType rePaintType = NewGame;
StoneType stonecolor = Black;

CImage img_black, img_black_bs;
CImage img_white, img_white_bs;
CImage img_board;

std::deque<StonePos> MovesDeque;           //��¼ÿ�����ӵ����ӵ�
StoneType ChessBoard[19][19];              //��¼���̵�����״��

CRect Rect;              //���ػ�����
CRect RegretRect[2];     //���壬���ػ�����

Negamax g_Negamax;

HANDLE hThread;

CMenu m_Menu;

int g_bm_xPos, g_bm_yPos;


// BattleDlg �Ի���

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


// BattleDlg ��Ϣ�������

//��ʼ����ս����
BOOL BattleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//����ͼƬ��Դ
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


	//���ݶ�ս��������Ӧ����
	bool flag = true;
	CString cstr_title;


	switch (BattleType)
	{
	case PVE:
		SetWindowText("�˻���ս");
		Bat_Enemy_ID = "����AI";
		Bat_Self_ID = "�Լ�";
		UpdateData(false);
		
		m_Menu.LoadMenu(IDR_MENU1);
		SetMenu(&m_Menu);
		DrawMenuBar();
		break;
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//�����ػ���Ϣ��Ӧ��**********************ע����Ӧ���������ػ���Ϣ�������ƶ����ڣ������ڵ���������ػ�
void BattleDlg::OnPaint()
{
	CPaintDC dc(this);
	
	switch (rePaintType)
	{
	case NewGame:
		InitGame(&dc);
		break;

	case StoneDown:		
		//�����ҽ��յ�������Ϣʱ������IsMyStoneDown = trueʱ������DrawStone();
		//�� rePaintType = StoneDown ���������ƶ�����������ػ�ʱ���򲻵���DrawStone();
		if (IsMyStoneDown)                                      
		{
			DrawStone(&dc, Rect, stonecolor);
			IsMyStoneDown = false;
			if (IsGameOver(MovesDeque.back()))
			{
				if (battleStatus == GameOver)
				{
					if (stonecolor == Black)
						AfxMessageBox("�����ʤ��");
					else
						AfxMessageBox("�����ʤ��");
				}		
			}	

			if (battleStatus == EnemyTurn)
				battleStatus = MyTurn;                                               //�ֵ��Լ�����
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

//��ʼ����Ϸ
void BattleDlg::InitGame(CPaintDC *pDC)
{
	img_board.Draw(pDC->m_hDC, 0, 0, 566, 566);

	MovesDeque.clear();

	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			ChessBoard[i][j] = Empty;

	rePaintType = StoneDown;

	//�������ã��޸ĸ������ĳ�ʼֵ
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

//���ڹر���Ϣ��Ӧ
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

//�����������Ϣ��Ӧ
void BattleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (battleStatus == MyTurn)
	{
		StonePos stone;

		//���ӳɹ��������ӵ���ӵ����Ӷ�����,�����ػ���Ϣ
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

//�˳�PVE
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

//���������Ϸ��
void BattleDlg::OnNewGame()
{
	rePaintType = NewGame;
	Invalidate();
	UpdateWindow();
}

//��������塱
void BattleDlg::OnRegret()
{
	//�����Լ��غϣ�������ǿ�ʱ��������Ч
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

//��PNGͼƬ����AlphaԤ��,��ԭ��͸����ʾ
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

//��֤�Ƿ�Ϊ��Ч����
bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos)
{
	if (xpos <= ChessBoardSize && ypos <= ChessBoardSize)
	{
		//������µ����ӵ�
		stone.xPos = xpos / StoneSize;
		stone.yPos = ypos / StoneSize;
		if (ChessBoard[stone.xPos][stone.yPos] != Empty)
			return false;

		int xUp = 20 + 29 * stone.xPos;
		int xLow = 10 + 29 * stone.xPos;
		int yUp = 20 + 29 * stone.yPos;
		int yLow = 10 + 29 * stone.yPos;

		//ȷʵ����Ƿ�λ�ڿ����ӵ㷶Χ��
		if (xpos >= xLow && xpos <= xUp &&
			ypos >= yLow && ypos <= yUp)
			return true;
		else
			return false;
	}
	else
		return false;
}

//�����ػ�
void DrawStone(CPaintDC *pDC, const CRect rect, StoneType &color)
{
	//���ػ�������Rect���򲿷�
	img_board.Draw(pDC->m_hDC, Rect, Rect);

	//���ٸ������������
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
		
	//����������ϻ��Ƶ�ǰ���岽��
	CString str;
	int steps = MovesDeque.size();
	str.Format(_T("%d"), steps);

	CFont font;
	CFont *pOldFont;                      // �����豸���������ʹ�õ��������
	CRect stepRect;
	font.CreatePointFont(100, "����");
	pOldFont = pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);          // ���ĵ�ǰ����

	//��stepRect�ڣ����Ӳ���
	stepRect.left = Rect.left;
	stepRect.top = Rect.top + 7;
	stepRect.right = Rect.right;
	stepRect.bottom = Rect.bottom;

	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->DrawText(str, &stepRect, DT_CENTER);
	pDC->SelectObject(pOldFont);          //��ԭ����
}

//���崦��
void RegretProc(CPaintDC *pDC)
{
	//���ػ�������Rect���򲿷�
	img_board.Draw(pDC->m_hDC, RegretRect[0], RegretRect[0]);
	img_board.Draw(pDC->m_hDC, RegretRect[1], RegretRect[1]);

	//if (stonecolor == Black)
	//	stonecolor = White;
	//else
	//	stonecolor = Black;
}

//������ĳ��������������������ɫ��ͬ��������
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

//�ж���Ϸ�Ƿ��������������true�����򷵻�false
bool IsGameOver(StonePos pos)
{
	//���ҷ������·����������·����������Ϸ���
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
//	else if (ret == SOCKET_ERROR)//socket�˳�
//	{
//		CString cstr;
//		cstr.Format("socket�˳����û���¼��Ϣ���մ��󣬴����Ϊ��%d", WSAGetLastError());
//		AfxMessageBox(cstr);
//	}
//	else//����������󣬽���ʧ��
//	{
//		CString cstr;
//		cstr.Format("��������û���¼��Ϣ���մ��󣬴����Ϊ��%d", WSAGetLastError());
//		AfxMessageBox(cstr);
//	}
//
//	//��β
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
