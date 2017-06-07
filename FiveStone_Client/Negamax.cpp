#include "stdafx.h"
#include "Negamax.h"
#include <deque>

#define DEBUG_NEGAMAX 3

extern int g_bm_xPos;
extern int g_bm_yPos;
extern int PosValue[GRID_NUM][GRID_NUM];
extern std::deque<StonePos> MovesDeque;


void Negamax::SetSearchDepth(int depth)
{
	m_nSearchDepth = depth;
}

void Negamax::SearchAGoodMove(StoneType position[][GRID_NUM], int role)
{
	int Score;
	memcpy(CurPosition, position, sizeof(CurPosition));
	m_nMaxDepth = m_nSearchDepth;

	Node_Count = 0;
	ABcut_Count = 0;

	switch (DEBUG_NEGAMAX)
	{
	case 1:
		Score = AB_Negamax(m_nMaxDepth, -MATE, MATE, role);
		break;
	case 2:
		Score = NAB_Negamax(m_nMaxDepth, role);
		break;
	case 3:
		Score = MyAB_Negamax(m_nMaxDepth, -MATE, MATE, role);
		break;
	default:
		break;
	}

	g_bm_xPos = m_cmBestMove.StonePos.xPos;
	g_bm_yPos = m_cmBestMove.StonePos.yPos;

	memcpy(position, CurPosition, sizeof(CurPosition));
}

int Negamax::AB_Negamax(int depth, int alpha, int beta, int role)
{
	int score;
	
	if (depth > 0)                                        //���ж��Ƿ�ʤ���ѷ�
	{
		score = Eval.Evaluate(CurPosition, role);
		if (score == SCORE_FIVE || score == -SCORE_FIVE)
			return score;
	}
	
	if (depth == 0)                                       //Ҷ��㣬��������ֵ
	{
		score = Eval.Evaluate(CurPosition, role);
		return score;
	}

	//���ɿ��ܵ������б��������������ݴ����ǣ�
	int count = CreatPossibleMove(CurPosition, depth);
	//int count = CreatPossibleMove_Limited(CurPosition, depth);
	

	for ( int i = 0; i < count; i++)  //���������б�
	{
		//��������
		MakeMove(&MoveList[depth][i], role);
		//ȡ�����Ӻ���������
		//score = AB_Negamax(depth - 1, -beta, -alpha, -role);

		if (depth == 1)
			score = AB_Negamax(depth - 1, -beta, -alpha, -role);
		else
			score = -AB_Negamax(depth - 1, -beta, -alpha, -role);

		ScoreArray[depth][i] = score;

		//if (score > alpha && score < beta && i > 0)
		//{
		//	//���ڵ�һ����Ľڵ�,������������failhigh
		//	alpha = -AB_Negamax(depth - 1, -beta, -score, -role);//re-search
		//	if (depth == m_nMaxDepth)
		//		m_cmBestMove = MoveList[depth][i];
		//}

		//��������
		UnMakeMove(&MoveList[depth][i]);

		if (score > alpha)
		{
			alpha = score;
		//	if (depth == m_nMaxDepth)
				m_cmBestMove = MoveList[depth][i];  //������ӱ�Ϊ��ǰ���ӵ�
		}
		if (score >= beta)
		{
			return alpha;  //��֦
		}
		
	}
	return alpha;
}

int Negamax::MyAB_Negamax(int depth, int alpha, int beta, int role)
{
	int score;

	if (depth > 0)                                        //���ж��Ƿ�ʤ���ѷ�
	{
		score = Eval.Evaluate_Test(CurPosition, role);
		if (score == SCORE_FIVE || score == -SCORE_FIVE)
			return score;
	}

	if (depth == 0)                                       //Ҷ��㣬��������ֵ
	{
		score = Eval.Evaluate_Test(CurPosition, role);
		return score;
	}

	//���ɿ��ܵ������б��������������ݴ����ǣ�
	int count = CreatPossibleMove_Limited(CurPosition, depth);


	for (int i = 0; i < count; i++)  //���������б�
	{
		//��������
		MakeMove(&MoveList[depth][i], role);
		//ȡ�����Ӻ���������
		score = -MyAB_Negamax(depth - 1, -beta, -alpha, -role);

		ScoreArray[depth][i] = score;
		Node_Count++;

		//if (score > alpha && score < beta && i > 0)
		//{
		//	//���ڵ�һ����Ľڵ�,������������failhigh
		//	alpha = MyAB_Negamax(depth - 1, -beta, -score, -role);//re-search
		//	if (depth == m_nMaxDepth)
		//		m_cmBestMove = MoveList[depth][i];
		//}

		//��������
		UnMakeMove(&MoveList[depth][i]);

		if (score > alpha)
		{
			alpha = score;
			if (depth == m_nMaxDepth)
				m_cmBestMove = MoveList[depth][i];  //������ӱ�Ϊ��ǰ���ӵ�
		}
		if (alpha >= beta)
		{
			ABcut_Count++;
			return alpha;
		}
		//beta = alpha + 1;
	}
	return alpha;
}

int Negamax::NAB_Negamax(int depth, int role)
{
	int score, best = -MATE;

	if (depth > 0)                                        //���ж��Ƿ�ʤ���ѷ�
	{
		score = Eval.Evaluate(CurPosition, role);
		if (score == SCORE_FIVE || score == -SCORE_FIVE)
			return score;
	}

	if (depth == 0)                                       //Ҷ��㣬��������ֵ
	{
		score = Eval.Evaluate(CurPosition, role);
		return score;
	}

	//���ɿ��ܵ������б��������������ݴ����ǣ�
	int count = CreatPossibleMove_Limited(CurPosition, depth);


	for (int i = 0; i < count; i++)  //���������б�
	{
		//��������
		MakeMove(&MoveList[depth][i], role);
		//ȡ�����Ӻ���������
		score = NAB_Negamax(depth - 1, -role);
	
		ScoreArray[depth][i] = score;
		Node_Count++;

		//��������
		UnMakeMove(&MoveList[depth][i]);

		if (score > best)
		{
			best = score;
			if (depth == m_nMaxDepth)
				m_cmBestMove = MoveList[depth][i];  //������ӱ�Ϊ��ǰ���ӵ�
		}
	}
	return -best;
}

int Negamax::CreatPossibleMove(StoneType position[][GRID_NUM], int nPly)
{
	int x, y;
	int nMoveCount = 0;

	for (x = 0; x < GRID_NUM; x++)
		for (y = 0; y < GRID_NUM; y++)
		{
			if (position[x][y] == Empty)
			{
				MoveList[nPly][nMoveCount].StonePos.xPos = x;
				MoveList[nPly][nMoveCount].StonePos.yPos = y;
				MoveList[nPly][nMoveCount].Score = PosValue[x][y];

				nMoveCount++;
			}
		}

	//ʹ����ʷ�������еľ�̬�鲢���������߷����н�������
	//����Ϊ����߼�֦Ч��
	MergeSort(MoveList[nPly], nMoveCount, 0);

	return nMoveCount;
}

int Negamax::CreatPossibleMove_Limited(StoneType position[][GRID_NUM], int nPly)

{
	int l, r, t, b;
	int nMoveCount = 0, test = 0;
	int x, y;
	int theta = 7;

	//ÿ�ο������������Ϊ����2 * theta + 1)^2
	if (MovesDeque.back().xPos - theta < 0)
		l = 0;
	else
		l = MovesDeque.back().xPos - theta;

	if (MovesDeque.back().xPos + theta > GRID_NUM - 1)
		r = GRID_NUM - 1;
	else
		r = MovesDeque.back().xPos + theta;

	if (MovesDeque.back().yPos - theta < 0)
		t = 0;
	else
		t = MovesDeque.back().yPos - theta;

	if (MovesDeque.back().yPos + theta > GRID_NUM - 1)
		b = GRID_NUM - 1;
	else
		b = MovesDeque.back().yPos + theta;

	for ( x = l; x <= r; x++)
		for (y = t; y <= b; y++)
		{
			if (position[x][y] == Empty)
			{
				MoveList[nPly][nMoveCount].StonePos.xPos = x;
				MoveList[nPly][nMoveCount].StonePos.yPos = y;
				MoveList[nPly][nMoveCount].Score = PosValue[x][y];

				nMoveCount++;
			}
			else
				test++;
		}

	//ʹ����ʷ�������еľ�̬�鲢���������߷����н�������
	//����Ϊ����߼�֦Ч��
	MergeSort(MoveList[nPly], nMoveCount, 0);

	return nMoveCount;
}

void Negamax::MakeMove(StoneMove *pMove, int role)
{
	if (role == 1)
		CurPosition[pMove->StonePos.xPos][pMove->StonePos.yPos] = White;
	if (role == -1)
		CurPosition[pMove->StonePos.xPos][pMove->StonePos.yPos] = Black;
}

void Negamax::UnMakeMove(StoneMove *pMove)
{
	CurPosition[pMove->StonePos.xPos][pMove->StonePos.yPos] = Empty;
}

//���߷����д�С��������
//STONEMOVE* sourceԭʼ����
//STONEMOVE* targetĿ�����
//�ϲ�source[l��m]�� source[m +1��r]��target[l��r]
void Negamax::Merge(StoneMove* source, StoneMove* target, int l, int m, int r)
{
	//��С��������
	int i = l;
	int j = m + 1;
	int k = l;

	while (i <= m && j <= r)
		if (source[i].Score <= source[j].Score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];

	if (i > m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)
			target[k++] = source[q];
}

void Negamax::Merge_A(StoneMove* source, StoneMove* target, int l, int m, int r)
{
	//�Ӵ�С����
	int i = l;
	int j = m + 1;
	int k = l;

	while (i <= m && j <= r)
		if (source[i].Score >= source[j].Score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];

	if (i > m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)
			target[k++] = source[q];
}

//�ϲ���СΪ S ������������
//direction �Ǳ�־,ָ���ǴӴ�С���Ǵ�С��������
void Negamax::MergePass(StoneMove* source, StoneMove* target, const int s, const int n, const bool direction)
{
	int i = 0;

	while (i <= n - 2 * s)
	{
		//�ϲ���СΪ s�����ڶ���������
		if (direction)
			Merge(source, target, i, i + s - 1, i + 2 * s - 1);
		else
			Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);

		i = i + 2 * s;
	}

	if (i + s < n)//ʣ���Ԫ�ظ���С��2s
	{
		if (direction)
			Merge(source, target, i, i + s - 1, n - 1);
		else
			Merge_A(source, target, i, i + s - 1, n - 1);
	}
	else
		for (int j = i; j <= n - 1; j++)
			target[j] = source[j];
}

void Negamax::MergeSort(StoneMove* source, int n, bool direction)
{
	int s = 1;

	while (s<n)
	{
		MergePass(source, TargetBuff, s, n, direction);
		s += s;

		MergePass(TargetBuff, source, s, n, direction);
		s += s;
	}
}