#pragma once
#include "Global.h"

class Evaluator{
public:
	Evaluator(){};
	virtual ~Evaluator(){};

	int Evaluate(StoneType position[][GRID_NUM], int role);
	int Evaluate_Test(StoneType position[][GRID_NUM], int role);
protected:
	int AnalysisLine(StoneType *position, int LineLen, int StonePos);
	int AnalysisRight(StoneType position[][GRID_NUM], int x, int y);
	int AnalysisLeft(StoneType position[][GRID_NUM], int x, int y);
	int AnalysisVertical(StoneType position[][GRID_NUM], int x, int y);
	int AnalysisHorizon(StoneType position[][GRID_NUM], int x, int y);
	void SetAnalyResRecord(StoneType *analyline ,int left, int right, int pos, int linelen, int type);

	int AnalyResRecord[GRID_NUM];          //记录待分析数组中每个棋子所形成的棋型
	int TypeRecord[GRID_NUM][GRID_NUM][4]; //记录每个棋子在四个方向上各形成的棋型
	int TypeCount[2][20];                  //记录棋盘上两种棋子，每种棋型的个数
};