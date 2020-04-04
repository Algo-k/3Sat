
// 3SATC++.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMy3SATCApp:
// See 3SATC++.cpp for the implementation of this class
//

class CMy3SATCApp : public CWinApp
{
public:
	CMy3SATCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMy3SATCApp theApp;

char* int2str(int i);
char* word2char(wchar_t* in);
