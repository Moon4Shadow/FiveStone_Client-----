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
	StoneType stonetype, inversetype;  //������ɫ����
	StoneType AnalyLine[GRID_NUM];
	int nAnalyPos;

	int LeftEdge, RightEdge;         //�뱻��������ͬɫ��ֱ�����������߶ε� ���Ե �� �ұ�Ե
	int LeftRange, RightRange;       //�뱻��������ͬɫ��ֱ�ӻ������������߶ε� ��߽� �� �ұ߽�
	StoneType L[5];        //�ֱ�洢��LeftEdge�����1, 2, 3, 4��λ�õ��������ͣ����������̱߽磬��洢Ϊ�뱻���������෴������ , ��L[1]��ʼ�洢
	StoneType R[5];        //�ֱ�洢��RightEdge���ҵ�1, 2, 3, 4��λ�õ��������ͣ����������̱߽磬��洢Ϊ�뱻���������෴�����ͣ���R[1]��ʼ�洢

	if (LineLen < 5)
	{
		//����������С��5�������ܡ����塱��û��Ҫ����
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

	//������������,�ұ߽�
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

	//������ӿ��µķ�Χ
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

	//���L1, L2, L3, L4�� R1, R2, R3, R4 ������
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

	//����˷�ΧС��4����û�з����ı�Ҫ
	if (RightRange - LeftRange < 4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			AnalyResRecord[k] = NOTYPE;
		return 0;
	}

	//"����"
	if (RightEdge - LeftEdge + 1 > 4)
		SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, FIVE);

	switch (RightEdge - LeftEdge + 1)
	{
	case 4:		                                                                               //����
		if (L[1] == Empty && R[1] == Empty)
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, FOUR);
		else
			SetAnalyResRecord(AnalyLine, LeftEdge, RightEdge, nAnalyPos, LineLen, SFOUR);
		break;


	case 3:		                                                                               //����
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


	case 2:	                                                                                  //����
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


	case 1:	                                                                                     //һ��
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

//��������������Ϊ type��
//ͬʱ������Χ�ڵ�ͬɫ������Ϊ ANALSISED, �����ظ�����
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

	//��ˮƽ�����ϵ�����ת��һά����
	for (int k = 0; k < GRID_NUM; k++)
		tempArray[k] = position[k][y];

	//����ֱ�߷�����������
	AnalysisLine(tempArray, GRID_NUM, x);

	//ʰȡ�������
	for (int s = 0; s < GRID_NUM; s++)
		if (AnalyResRecord[s] != TOBEANALSIS)
			TypeRecord[s][y][0] = AnalyResRecord[s];

	return TypeRecord[x][y][0];
}

/*  |  */
int Evaluator::AnalysisVertical(StoneType position[][GRID_NUM], int x, int y)
{
	StoneType tempArray[GRID_NUM];

	//����ֱ�����ϵ�����ת��һά����
	for (int k = 0; k < GRID_NUM; k++)
		tempArray[k] = position[x][k];

	//����ֱ�߷�����������
	AnalysisLine(tempArray, GRID_NUM, y);

	//ʰȡ�������
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

	//����б�����ϵ����Ӱ����ϵ��µ�˳��ת��һά����
	for (k = 0; k < GRID_NUM; k++)
	{
		if (_x + k > GRID_NUM - 1 || _y + k > GRID_NUM - 1)
			break;
		tempArray[k] = position[_x + k][_y + k];
	}

	//����ֱ�߷�����������
	AnalysisLine(tempArray, k, realnum);

	//ʰȡ�������
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

	//����б�����ϵ����Ӱ����µ��ϵ�˳��ת��һά����
	for (k = 0; k < GRID_NUM; k++)
	{
		if (_x + k > GRID_NUM - 1 || _y - k < 0)
			break;
		tempArray[k] = position[_x + k][_y - k];
	}

	//����ֱ�߷�����������
	AnalysisLine(tempArray, k, realnum);

	//ʰȡ�������
	for (int s = 0; s < k; s++)
		if (AnalyResRecord[s] != TOBEANALSIS)
			TypeRecord[_x + s][_y - s][3] = AnalyResRecord[s];

	return TypeRecord[x][y][3];
}

//������������֣�������Ե�ǰ���ӷ���
//Ĭ��PCִ���壬role = 1ʱ��ΪPC���ӣ�����Ժ������֣�role = -1ʱ��ΪHUM���ӣ�����԰�������
int Evaluator::Evaluate(StoneType position[][GRID_NUM], int role)
{
	int i, j, k;
	StoneType nStoneType;

	//������ͷ������
	memset(TypeRecord, TOBEANALSIS, sizeof(TypeRecord));
	memset(TypeCount, 0, sizeof(TypeCount));

	StoneType st[GRID_NUM][GRID_NUM];
	memcpy(st, position, sizeof(st));

	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] != Empty)
			{
				//���ˮƽ������û�з�����
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//�����ֱ������û�з�����
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//�����б������û�з�����
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);

				//�����б������û�з�����
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}

	//�Է����������ͳ��,�õ�ÿ�����͵�����
	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
			for (k = 0; k < 4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != Empty)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE://����
						TypeCount[nStoneType][FIVE]++;
						break;

					case FOUR://����
						TypeCount[nStoneType][FOUR]++;
						break;

					case SFOUR://����
						TypeCount[nStoneType][SFOUR]++;
						break;

					case THREE://����
						TypeCount[nStoneType][THREE]++;
						break;

					case STHREE://����
						TypeCount[nStoneType][STHREE]++;
						break;

					case TWO://���
						TypeCount[nStoneType][TWO]++;
						break;

					case STWO://�߶�
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

	//�������Ե�ʤ����ֱ�ӷ���һ����Լ�ֵ
	//���㼺�����Ӻ�ľ������֣�˳�򲻿ɱ䣬������Լ���
	if (TypeCount[_ememy][FIVE])                                            //�Է��ѡ����塱
		return -1 * SCORE_FIVE;

	if (TypeCount[_self][FIVE])                                             //�Է�δ�����塱�������ѡ����塱
		return SCORE_FIVE;

	if (TypeCount[_ememy][FOUR] ||
		TypeCount[_ememy][SFOUR])                                           //�Է��ѡ�������
		return -1 * SCORE_FOUR;

	if (TypeCount[_self][FOUR] ||
		TypeCount[_self][SFOUR] > 1 ||
		(TypeCount[_self][SFOUR] == 1 && TypeCount[_self][THREE] > 0))      //�����ѡ����ġ� ���� ���������ϡ����ġ� ���� ��һ�������ġ���һ�����ϵġ�������
		return SCORE_FOUR;

	if (TypeCount[_ememy][THREE] && !TypeCount[_self][SFOUR])               //�Է���һ�����ϡ������� �� �����ޡ����ġ�
		return -1 * SCORE_FOUR;

	//if (TypeCount[_self][SFOUR] == 1 && !TypeCount[_self][THREE])
	//	return SCORE_SFOUR;

	if (TypeCount[_self][THREE] > 1 && !TypeCount[_ememy][THREE])           //�������������ϡ������� �� �Է��ޡ�������
		return SCORE_FOUR;

	//���ڲ����Ե�ʤ������ ˫�����ֵ��ۼ�ֵ�Ĳ�ֵ������-�з��� ��Ϊ���վ�������

	//�ȷֱ��ۼ�˫�������͵�����,ע����ͬ���ͣ��з�������ֵӦ�ñȼ����ĸ�
	//���������е����ͣ����ģ�һ������������һ���Ҳ�����Ĳ��棩��������������߶�
	//�з������е����ͣ�������һ������������������߶�
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
		_EValue += SCORE_THREE + SCORE_SFOUR;                            //�з��С��������Ҽ����С����ġ�ʱ���з��ġ����������ֱȼ����ġ����ġ�Ҫ��
	if (TypeCount[_ememy][STHREE])
		_EValue += (SCORE_STHREE + 90) * TypeCount[_ememy][STHREE];
	if (TypeCount[_ememy][TWO])
		_EValue += (SCORE_TWO + 45)* TypeCount[_ememy][TWO];
	if (TypeCount[_ememy][STWO])
		_EValue += (SCORE_STWO + 10) * TypeCount[_ememy][STWO];

	//�ٷֱ��ۼ�˫������������������λ�õ�����
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

	//������ͷ������
	memset(TypeRecord, TOBEANALSIS, sizeof(TypeRecord));
	memset(TypeCount, 0, sizeof(TypeCount));

	StoneType st[GRID_NUM][GRID_NUM];
	memcpy(st, position, sizeof(st));

	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] != Empty)
			{
				//���ˮƽ������û�з�����
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//�����ֱ������û�з�����
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//�����б������û�з�����
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);

				//�����б������û�з�����
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}

	//�Է����������ͳ��,�õ�ÿ�����͵�����
	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
			for (k = 0; k < 4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != Empty)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE://����
						TypeCount[nStoneType][FIVE]++;
						break;

					case FOUR://����
						TypeCount[nStoneType][FOUR]++;
						break;

					case SFOUR://����
						TypeCount[nStoneType][SFOUR]++;
						break;

					case THREE://����
						TypeCount[nStoneType][THREE]++;
						break;

					case STHREE://����
						TypeCount[nStoneType][STHREE]++;
						break;

					case TWO://���
						TypeCount[nStoneType][TWO]++;
						break;

					case STWO://�߶�
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

	//�������Ե�ʤ����ֱ�ӷ���һ����Լ�ֵ
	//���㼺�����Ӻ�ľ������֣�˳�򲻿ɱ䣬������Լ���
	if (TypeCount[_self][FIVE])                                             //�����ѡ����塱
		return SCORE_FIVE;

	if (TypeCount[_ememy][FIVE])                                            //�Է��ѡ����塱
		return -1 * SCORE_FIVE;

	if (TypeCount[_self][SFOUR] > 1)
		TypeCount[_self][FOUR]++;
	if (TypeCount[_self][FOUR])                                             //�����С����ġ�
		return SCORE_FOUR;
	if (TypeCount[_self][SFOUR])                                            //�����С����ġ�
		return SCORE_FOUR;

	if (TypeCount[_ememy][SFOUR] > 1)
		TypeCount[_ememy][FOUR]++;
	if (TypeCount[_ememy][FOUR])                                            //�Է��ѡ����ġ�
		return -1 * SCORE_FOUR;

	if (TypeCount[_ememy][SFOUR] && TypeCount[_ememy][THREE])               //�Է��С����ġ����С�������
		return -1 * (SCORE_FOUR - 500);

	//if (TypeCount[_ememy][SFOUR])                                         //�Է��С����ġ��ޡ�������
	//	return -1 * SCORE_SFOUR;

	if (TypeCount[_self][THREE] && !TypeCount[_ememy][SFOUR])              //������һ�����ϡ������� �� �Է��ޡ����ġ�
		return SCORE_FOUR - 1000;

	if (TypeCount[_ememy][THREE])                                           //�Է���һ�����ϡ�������
		return -1 * SCORE_THREE + 1000;


	//���ڲ����Ե�ʤ������ ˫�����ֵ��ۼ�ֵ�Ĳ�ֵ������-�з��� ��Ϊ���վ�������

	//�ȷֱ��ۼ�˫�������͵�����,ע����ͬ���ͣ��з�������ֵӦ�ñȼ����ĸ�
	//���������е����ͣ�������������������߶�
	//�з������е����ͣ����ģ�������������߶�
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

	//�ٷֱ��ۼ�˫������������������λ�õ�����
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