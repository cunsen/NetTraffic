
// NetTraffic.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNetTrafficApp: 
// �йش����ʵ�֣������ NetTraffic.cpp
//

class CNetTrafficApp : public CWinApp
{
public:
	CNetTrafficApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetTrafficApp theApp;
