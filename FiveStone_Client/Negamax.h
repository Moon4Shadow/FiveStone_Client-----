#pragma once
#include "Evaluator.h"
#include <deque>

class Negamax{
public:
	Negamax(){};
	virtual ~Negamax(){};

	void SetSearchDepth(int depth);
	void SearchAGoodMove(StoneType position[][GRID_NUM], int role);

protected:
	int Node_Count;
	int ABcut_Count;

	Evaluator Eval;
	int m_nMaxDepth;
	int m_nSearchDepth;
	StoneType CurPosition[GRID_NUM][GRID_NUM];
	StoneMove m_cmBestMove;
	StoneMove MoveList[10][GRID_COUNT];
	StoneMove TargetBuff[GRID_COUNT];

	int ScoreArray[10][GRID_COUNT];

	int NAB_Negamax(int depth, int role);
	int MyAB_Negamax(int depth, int alpha, int beta, int role);
	int AB_Negamax(int depth, int alpha, int beta, int role);
	int CreatPossibleMove(StoneType position[][GRID_NUM], int nPly);
	void MakeMove(StoneMove *pMove, int role);
	void UnMakeMove(StoneMove *pMove);

	int CreatPossibleMove_Limited(StoneType position[][GRID_NUM], int nPly);

	void Merge(StoneMove* source, StoneMove* target, int l, int m, int r);
	void Merge_A(StoneMove* source, StoneMove* target, int l, int m, int r);
	void MergePass(StoneMove* source, StoneMove* target, const int s, const int n, const bool direction);
	void MergeSort(StoneMove* source, int n, bool direction);
};