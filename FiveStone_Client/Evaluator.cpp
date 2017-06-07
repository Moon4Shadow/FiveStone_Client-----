#include "stdafx.h"
#include "Evaluator.h"

int PosValue[GRID_NUM][GRID_NUM] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};


int Evaluator::AnalysisLine(StoneType *position, int LineLen, int StonePos)
{
	StoneType stonetype, inversetype;  //棋子颜色类型
	StoneType AnalyLine[GRID_NUM];
	int nAnalyPos;

	int LeftEdge, RightEdge;         //与被分析棋子同色且直接相连所成线段的 左边缘 和 右边缘
	int LeftRange, RightRange;       //与被分析棋子同色且直接或间接相连所成线段的 左边界 和 右边界
	StoneType L[5];        //分别存储在LeftEdge往左第1, 2, 3, 4个位置的棋子类型，若超出棋盘边界，则存储为与被分析棋子相反的类型 , 从L[1]开始存储
	StoneType R[5];        //分别存储在RightEdge往右第1, 2, 3, 4个位置的棋子类型，若超出棋盘边界，则存储为与被分析棋子相反的类型，从R[1]开始存储

	if (LineLen < 5)
	{
		//待分析数组小于5，不可能“成五”，没必要分析
		memset(AnalyResRecord, NOTYPE, sizeof(AnalyResRecord));
		return 0;
	}

	nAnalyPos = StonePos;

	stonetype = position[StonePos];
	if (stonetype == Black)
		inversetype = White;
	if (stonetype == White)
		inversetype = Black;

	memcpy((void*)AnalyLine, (void*)position, sizeof(AnalyLine));           //????
	memset(AnalyResRecord, TOBEANALSIS, sizeof(AnalyResRecord));

	LeftEdge = nAnalyPos;
	RightEdge = nAnalyPos;

	//算连续棋子左,右边界
	while (LeftEdge >= 0)
	{
		if (AnalyLine[LeftEdge - 1] != stonetype)
			break;
		LeftEdge--;
	}
	while (RightEdge < LineLen)
	{
		if (AnalyLine[LeftEdge - 1] != stonetype)
			break;
		RightEdge++;
	}

	//算出棋子可下的范围
	LeftRange = LeftEdge;
	RightRange = RightEdge;
	while (LeftRange > 0)
	{
		if (AnalyLine[LeftRange - 1] == inversetype)
			break;
		LeftRange--;
	}
	while (RightRange < LineLen)
	{
		if (AnalyLine[RightRange + 1] == inversetype)
			break;
		RightRange++;
	}

	//算出L1, L2, L3, L4， R1, R2, R3, R4 的类型
	for (int i = 1; i < 5; i++)
	{
		if (LeftEdge - i >= 0)
			L[i] = AnalyLine[LeftEdge - i];
		else
			L[i] = inversetype;

		if (RightEdge + i < LineLen)
			R[i] = AnalyLine[RightEdge + i];
		else
			R[i] = inversetype;
	}

	//如果此范围小于4，则没有分析的必要
	if (RightRange - LeftRange < 4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			AnalyResRecord[k] = NOTYPE;
		return 0;
	}

	//"成五"
	if (RightEdge - LeftEdge + 1 > 4)
		SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, FIVE);

	switch (RightEdge - LeftEdge + 1)
	{
	case 4:		                                                                               //四连
		if (L[1] == Empty && R[1] == Empty)
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, FOUR);
		else
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, SFOUR);
		break;


	case 3:		                                                                               //三连
		if (L[1] == Empty && L[2] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 2, RightEdge, nAnalyPos, LineLen, SFOUR);
			break;
		}
		if (R[1] == Empty && R[2] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 2, nAnalyPos, LineLen, SFOUR);
			break;
		}
		//******************
		if (L[2] == inversetype && R[2] == inversetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, STHREE);
			break;
		}
		//******************
		if (L[1] == inversetype || R[1] == inversetype)
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, STHREE);
		else
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, THREE);
		break;	


	case 2:	                                                                                  //二连
		if (L[1] == Empty && L[2] == stonetype && L[3] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 3, RightEdge, nAnalyPos, LineLen, SFOUR);
			break;
		}
		if (R[1] == Empty && R[2] == stonetype && R[3] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 3, nAnalyPos, LineLen, SFOUR);
			break;
		}
		//******************
		if (L[1] == Empty && L[2] == stonetype)
			if (L[3] == Empty && R[1] == Empty)
			{
				SetAnalyResRecord(AnalyLine, LeftEdge + 3, RightEdge, nAnalyPos, LineLen, THREE);
				break;
			}
			else
			{
				SetAnalyResRecord(AnalyLine, LeftEdge + 3, RightEdge, nAnalyPos, LineLen, STHREE);
				break;
			}
		if (R[1] == Empty && R[2] == stonetype)
			if (R[3] == Empty && L[1] == Empty)
			{
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 3, nAnalyPos, LineLen, THREE);
				break;
			}
			else
			{
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 3, nAnalyPos, LineLen, STHREE);
				break;
			}
		//******************
		if (L[1] == Empty && L[2] == Empty && L[3] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 3, RightEdge, nAnalyPos, LineLen, STHREE);
			break;
		}
		if (R[1] == Empty && R[2] == Empty && R[3] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 3, nAnalyPos, LineLen, STHREE);
			break;
		}
		//******************
		if (L[1] == Empty && R[1] == Empty)
		{
			if ((L[2] == Empty && L[3] == Empty)||
				(R[2] == Empty && R[3] == Empty))
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, TWO);
		}
		else
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, STWO);
		}
		break;


	case 1:	                                                                                     //一连
		if (L[1] == Empty && L[2] == stonetype && L[3] == stonetype && L[4] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, SFOUR);
			break;
		}
		if (R[1] == Empty && R[2] == stonetype && R[3] == stonetype && R[4] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, SFOUR);
			break;
		}
		//******************
		if (L[1] == Empty && L[2] == stonetype && L[3] == stonetype)
			if (L[4] == Empty && R[1] == Empty)
			{
				SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, THREE);
				break;
			}
			else
			{
				SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, STHREE);
				break;
			}
		if (R[1] == Empty && R[2] == stonetype && R[3] == stonetype)
			if (R[4] == Empty && L[1] == Empty)
			{
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, THREE);
				break;
			}
			else
			{
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, STHREE);
				break;
			}
		//******************
		if ((L[1] == Empty && L[2] == stonetype && L[3] == Empty && L[4] == stonetype) ||
			(L[1] == Empty && L[2] == Empty && L[3] == stonetype && L[4] == stonetype))
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, STHREE);
			break;
		}
		if ((R[1] == Empty && R[2] == stonetype && R[3] == Empty && R[4] == stonetype) ||
			(R[1] == Empty && R[2] == Empty && R[3] == stonetype && R[4] == stonetype))
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, STHREE);
			break;
		}
		if (L[1] == Empty && L[2] == stonetype && R[1] == Empty && R[2] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 2, RightEdge + 2, nAnalyPos, LineLen, STHREE);
			break;
		}
		//******************
		if ((L[1] == Empty && L[2] == stonetype && L[3] == Empty) ||
			(L[1] == Empty && L[2] == Empty && L[3] == stonetype))
			if (L[4] == Empty && R[1] == Empty)
			{
				SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, TWO);
				break;
			}
			else
			{
				SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, STWO);
				break;
			}
		if ((R[1] == Empty && R[2] == stonetype && R[3] == Empty) ||
			(R[1] == Empty && R[2] == Empty && R[3] == stonetype))
			if (R[4] == Empty && L[1] == Empty)
			{
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, TWO);
				break;
			}
			else
			{
				SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, STWO);
				break;
			}
		//******************
		if (L[1] == Empty && L[2] == Empty && L[3] == Empty && L[4] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge + 4, RightEdge, nAnalyPos, LineLen, STWO);
			break;
		}
		if (R[1] == Empty && R[2] == Empty && R[3] == Empty && R[4] == stonetype)
		{
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge + 4, nAnalyPos, LineLen, STWO);
			break;
		}
		break;

	default:
		break;
	}

	return 0;
}

//将被分析棋子设为 type；
//同时分析范围内的同色棋子设为 ANALSISED, 以免重复分析
void Evaluator::SetAnalyResRecord(StoneType *analyline, int left, int right, int pos, int linelen, int type)
{
	for (;left <= right; left++)
	{
		if (left < 0 || left > linelen - 1)
			continue;

		if (left != pos && AnalyResRecord[left] == TOBEANALSIS && analyline[left] == analyline[pos])
			AnalyResRecord[left] = ANALSISED;
		else if (left == pos)
			AnalyResRecord[left] = type;
	}
}

/*  -  */
int Evaluator::AnalysisHorizon(StoneType position[][GRID_NUM], int x, int y)
{
	StoneType tempArray[GRID_NUM];

	//将水平方向上的棋子转入一维数组
	for (int k = 0; k < GRID_NUM; k++)
		tempArray[k] = position[k][y];

	//调用直线分析函数分析
	AnalysisLine(tempArray, GRID_NUM, x);

	//拾取分析结果
	for (int s = 0; s < GRID_NUM; s++)
		if (AnalyResRecord[s] != TOBEANALSIS)
			TypeRecord[s][y][0] = AnalyResRecord[s];

	return TypeRecord[x][y][0];
}

/*  |  */
int Evaluator::AnalysisVertical(StoneType position[][GRID_NUM], int x, int y)
{
	StoneType tempArray[GRID_NUM];

	//将垂直方向上的棋子转入一维数组
	for (int k = 0; k < GRID_NUM; k++)
		tempArray[k] = position[x][k];

	//调用直线分析函数分析
	AnalysisLine(tempArray, GRID_NUM, y);

	//拾取分析结果
	for (int s = 0; s < GRID_NUM; s++)
		if (AnalyResRecord[s] != TOBEANALSIS)
			TypeRecord[x][s][1] = AnalyResRecord[s];

	return TypeRecord[x][y][1];
}

/*  \  */
int Evaluator::AnalysisLeft(StoneType position[][GRID_NUM], int x, int y)
{
	StoneType tempArray[GRID_NUM];
	int _x, _y, k, realnum;

	if (x < y)
	{
		_x = 0;
		_y = y - x;
		realnum = x;
	}
	else
	{
		_x = x - y;
		_y = 0;
		realnum = y;
	}

	//将左斜方向上的棋子按从上到下的顺序转入一维数组
	for (k = 0; k < GRID_NUM; k++)
	{
		if (_x + k > GRID_NUM - 1 || _y + k > GRID_NUM - 1)
			break;
		tempArray[k] = position[_x + k][_y + k];
	}

	//调用直线分析函数分析
	AnalysisLine(tempArray, k, realnum);

	//拾取分析结果
	for (int s = 0; s < k; s++)
		if (AnalyResRecord[s] != TOBEANALSIS)
			TypeRecord[_x + s][_y + s][2] = AnalyResRecord[s];

	return TypeRecord[x][y][2];
}

/*  /  */
int Evaluator::AnalysisRight(StoneType position[][GRID_NUM], int x, int y)
{
	StoneType tempArray[GRID_NUM];
	int _x, _y, k, realnum;

	if (GRID_NUM - 1 - x < y)
	{
		_x = x + y - GRID_NUM + 1;
		_y = GRID_NUM - 1;
		realnum = GRID_NUM - 1 - y;

	}
	else
	{
		_x = 0;
		_y = x + y;
		realnum = x;
	}

	//将右斜方向上的棋子按从下到上的顺序转入一维数组
	for (k = 0; k < GRID_NUM; k++)
	{
		if (_x + k > GRID_NUM - 1 || _y - k < 0)
			break;
		tempArray[k] = position[_x + k][_y - k];
	}

	//调用直线分析函数分析
	AnalysisLine(tempArray, k, realnum);

	//拾取分析结果
	for (int s = 0; s < k; s++)
		if (AnalyResRecord[s] != TOBEANALSIS)
			TypeRecord[_x + s][_y - s][3] = AnalyResRecord[s];

	return TypeRecord[x][y][3];
}

//对盘面进行评分，评分针对当前落子方；
//默认PC执黑棋，role = 1时，为PC落子，则针对黑棋评分；role = -1时，为HUM落子，则针对白棋落子
int Evaluator::Evaluate(StoneType position[][GRID_NUM], int role)
{
	int i, j, k;
	StoneType nStoneType;

	//清空棋型分析结果
	memset(TypeRecord, TOBEANALSIS, sizeof(TypeRecord));
	memset(TypeCount, 0, sizeof(TypeCount));

	StoneType st[GRID_NUM][GRID_NUM];
	memcpy(st, position, sizeof(st));

	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] != Empty)
			{
				//如果水平方向上没有分析过
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//如果垂直方向上没有分析过
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//如果左斜方向上没有分析过
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);

				//如果右斜方向上没有分析过
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}

	//对分析结果进行统计,得到每种棋型的数量
	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
			for (k = 0; k < 4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != Empty)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE://五连
						TypeCount[nStoneType][FIVE]++;
						break;

					case FOUR://活四
						TypeCount[nStoneType][FOUR]++;
						break;

					case SFOUR://冲四
						TypeCount[nStoneType][SFOUR]++;
						break;

					case THREE://活三
						TypeCount[nStoneType][THREE]++;
						break;

					case STHREE://眠三
						TypeCount[nStoneType][STHREE]++;
						break;

					case TWO://活二
						TypeCount[nStoneType][TWO]++;
						break;

					case STWO://眠二
						TypeCount[nStoneType][STWO]++;
						break;

					default:
						break;
					}
				}
			}

	int _ememy, _self;
	int _EValue = 0, _SValue = 0;
	if (role == 1)
	{
		_ememy = White;
		_self = Black;
	}
	else
	{
		_ememy = Black;
		_self = White;
	}

	//对于明显的胜负，直接返回一个相对极值
	//计算己方落子后的局面评分，顺序不可变，评分针对己方
	if (TypeCount[_ememy][FIVE])                                            //对方已“成五”
		return -1 * SCORE_FIVE;

	if (TypeCount[_self][FIVE])                                             //对方未“成五”，己方已“成五”
		return SCORE_FIVE;

	if (TypeCount[_ememy][FOUR] ||
		TypeCount[_ememy][SFOUR])                                           //对方已“四连”
		return -1 * SCORE_FOUR;

	if (TypeCount[_self][FOUR] ||
		TypeCount[_self][SFOUR] > 1 ||
		(TypeCount[_self][SFOUR] == 1 && TypeCount[_self][THREE] > 0))      //己方已“活四” 或者 有两个以上“冲四” 或者 有一个“冲四”和一个以上的“活三”
		return SCORE_FOUR;

	if (TypeCount[_ememy][THREE] && !TypeCount[_self][SFOUR])               //对方有一个以上“活三” 且 己方无“冲四”
		return -1 * SCORE_FOUR;

	//if (TypeCount[_self][SFOUR] == 1 && !TypeCount[_self][THREE])
	//	return SCORE_SFOUR;

	if (TypeCount[_self][THREE] > 1 && !TypeCount[_ememy][THREE])           //己方有两个以上“活三” 且 对方无“活三”
		return SCORE_FOUR;

	//对于不明显的胜负，则将 双方评分的累计值的差值（己方-敌方） 作为最终局面评分

	//先分别累计双方各棋型的评分,注意相同类型，敌方的评分值应该比己方的高
	//己方可能有的棋型：冲四（一个），活三（一个且不与冲四并存），眠三，活二，眠二
	//敌方可能有的棋型：活三（一个），眠三，活二，眠二
	if (TypeCount[_self][SFOUR] == 1)
		_SValue += SCORE_SFOUR;
	if (TypeCount[_self][THREE] == 1)
		_SValue += SCORE_THREE;
	if (TypeCount[_self][STHREE])
		_SValue += SCORE_STHREE * TypeCount[_self][STHREE];
	if (TypeCount[_self][TWO])
		_SValue += SCORE_TWO * TypeCount[_self][TWO];
	if (TypeCount[_self][STWO])
		_SValue += SCORE_STWO * TypeCount[_self][STWO];

	if (TypeCount[_ememy][THREE] == 1)
		_EValue += SCORE_THREE + SCORE_SFOUR;                            //敌方有“活三”且己方有“冲四”时，敌方的“活三”评分比己方的“冲四”要高
	if (TypeCount[_ememy][STHREE])
		_EValue += (SCORE_STHREE + 90) * TypeCount[_ememy][STHREE];
	if (TypeCount[_ememy][TWO])
		_EValue += (SCORE_TWO + 45)* TypeCount[_ememy][TWO];
	if (TypeCount[_ememy][STWO])
		_EValue += (SCORE_STWO + 10) * TypeCount[_ememy][STWO];

	//再分别累计双方有用棋子所在棋盘位置的评分
	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			if ((TypeRecord[i][j][0] == NOTYPE &&
				TypeRecord[i][j][1] == NOTYPE &&
				TypeRecord[i][j][2] == NOTYPE &&
				TypeRecord[i][j][3] == NOTYPE) ||
				position[i][j] == Empty)
				continue;

			if ((role == 1 && position[i][j] == Black) ||
				(role == -1 && position[i][j] == White))
				_SValue += PosValue[i][j];

			if ((role == 1 && position[i][j] == White) ||
				(role == -1 && position[i][j] == Black))
				_EValue += PosValue[i][j];
		}

	//return _EValue - _SValue;
	return _SValue - _EValue;
}

int Evaluator::Evaluate_Test(StoneType position[][GRID_NUM], int role)
{
	int i, j, k;
	StoneType nStoneType;

	//清空棋型分析结果
	memset(TypeRecord, TOBEANALSIS, sizeof(TypeRecord));
	memset(TypeCount, 0, sizeof(TypeCount));

	StoneType st[GRID_NUM][GRID_NUM];
	memcpy(st, position, sizeof(st));

	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] != Empty)
			{
				//如果水平方向上没有分析过
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//如果垂直方向上没有分析过
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//如果左斜方向上没有分析过
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);

				//如果右斜方向上没有分析过
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}

	//对分析结果进行统计,得到每种棋型的数量
	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
			for (k = 0; k < 4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != Empty)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE://五连
						TypeCount[nStoneType][FIVE]++;
						break;

					case FOUR://活四
						TypeCount[nStoneType][FOUR]++;
						break;

					case SFOUR://冲四
						TypeCount[nStoneType][SFOUR]++;
						break;

					case THREE://活三
						TypeCount[nStoneType][THREE]++;
						break;

					case STHREE://眠三
						TypeCount[nStoneType][STHREE]++;
						break;

					case TWO://活二
						TypeCount[nStoneType][TWO]++;
						break;

					case STWO://眠二
						TypeCount[nStoneType][STWO]++;
						break;

					default:
						break;
					}
				}
			}

	int _ememy, _self;
	int _EValue = 0, _SValue = 0;
	if (role == -1)
	{
		_self = Black;
		_ememy = White;
	}
	else
	{
		_self = White;
		_ememy = Black;
	}

	//对于明显的胜负，直接返回一个相对极值
	//计算己方落子后的局面评分，顺序不可变，评分针对己方
	if (TypeCount[_self][FIVE])                                             //己方已“成五”
		return SCORE_FIVE;

	if (TypeCount[_ememy][FIVE])                                            //对方已“成五”
		return -1 * SCORE_FIVE;

	if (TypeCount[_self][SFOUR] > 1)
		TypeCount[_self][FOUR]++;
	if (TypeCount[_self][FOUR])                                             //己方有“活四”
		return SCORE_FOUR;
	if (TypeCount[_self][SFOUR])                                            //己方有“冲四”
		return SCORE_FOUR;

	if (TypeCount[_ememy][SFOUR] > 1)
		TypeCount[_ememy][FOUR]++;
	if (TypeCount[_ememy][FOUR])                                            //对方已“活四”
		return -1 * SCORE_FOUR;

	if (TypeCount[_ememy][SFOUR] && TypeCount[_ememy][THREE])               //对方有“冲四”且有“活三”
		return -1 * (SCORE_FOUR - 500);

	//if (TypeCount[_ememy][SFOUR])                                         //对方有“冲四”无“活三”
	//	return -1 * SCORE_SFOUR;

	if (TypeCount[_self][THREE] && !TypeCount[_ememy][SFOUR])              //己方有一个以上“活三” 且 对方无“冲四”
		return SCORE_FOUR - 1000;

	if (TypeCount[_ememy][THREE])                                           //对方有一个以上“活三”
		return -1 * SCORE_THREE + 1000;


	//对于不明显的胜负，则将 双方评分的累计值的差值（己方-敌方） 作为最终局面评分

	//先分别累计双方各棋型的评分,注意相同类型，敌方的评分值应该比己方的高
	//己方可能有的棋型：活三、眠三，活二，眠二
	//敌方可能有的棋型：冲四，眠三，活二，眠二
	if (TypeCount[_self][THREE])
		_SValue += SCORE_THREE * TypeCount[_self][THREE];
	if (TypeCount[_self][STHREE])
		_SValue += SCORE_STHREE * TypeCount[_self][STHREE];
	if (TypeCount[_self][TWO])
		_SValue += SCORE_TWO * TypeCount[_self][TWO];
	if (TypeCount[_self][STWO])
		_SValue += SCORE_STWO * TypeCount[_self][STWO];


	if (TypeCount[_ememy][SFOUR])                                           
		_EValue += SCORE_SFOUR;
	if (TypeCount[_ememy][STHREE])
		_EValue += SCORE_STHREE * TypeCount[_ememy][STHREE];
	if (TypeCount[_ememy][TWO])
		_EValue += SCORE_TWO * TypeCount[_ememy][TWO];
	if (TypeCount[_ememy][STWO])
		_EValue += SCORE_STWO * TypeCount[_ememy][STWO];

	//再分别累计双方有用棋子所在棋盘位置的评分
	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			if ((TypeRecord[i][j][0] == NOTYPE &&
				TypeRecord[i][j][1] == NOTYPE &&
				TypeRecord[i][j][2] == NOTYPE &&
				TypeRecord[i][j][3] == NOTYPE) ||
				position[i][j] == Empty)
				continue;

			if ((role == 1 && position[i][j] == White) ||
				(role == -1 && position[i][j] == Black))
				_SValue += PosValue[i][j];

			if ((role == 1 && position[i][j] == Black) ||
				(role == -1 && position[i][j] == White))
				_EValue += PosValue[i][j];
		}
	return _SValue - _EValue;
}