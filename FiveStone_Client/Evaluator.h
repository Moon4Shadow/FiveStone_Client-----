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

	int AnalyResRecord[GRID_NUM];          //��¼������������ÿ���������γɵ�����
	int TypeRecord[GRID_NUM][GRID_NUM][4]; //��¼ÿ���������ĸ������ϸ��γɵ�����
	int TypeCount[2][20];                  //��¼�������������ӣ�ÿ�����͵ĸ���
};