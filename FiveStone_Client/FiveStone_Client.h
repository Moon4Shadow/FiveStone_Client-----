// FiveStone_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once


#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

//���ͷ�ļ��б�
#include "CFiveStone_Client_MainDlg.h"



// CFiveStone_ClientApp: 
// �йش����ʵ�֣������ FiveStone_Client.cpp
//

class CFiveStone_ClientApp : public CWinApp
{
public:
	CFiveStone_ClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFiveStone_ClientApp theApp;