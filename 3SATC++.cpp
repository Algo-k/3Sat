
// 3SATC++.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "3SATC++.h"
#include "3SATC++Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy3SATCApp

BEGIN_MESSAGE_MAP(CMy3SATCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMy3SATCApp construction

CMy3SATCApp::CMy3SATCApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMy3SATCApp object

CMy3SATCApp theApp;


// CMy3SATCApp initialization

BOOL CMy3SATCApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CMy3SATCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

char* int2str(int i)
{
	static char str[100];
	int p=99;
	str[p--]=0;
	str[p--]=0;
	if(i==0)
	{
	    str[p--]=0;
		str[p--]='0';
	    return &str[p+1];
	}
	else while(i!=0)
	{
	    str[p--]=0;
		str[p--]='0'+i%10;
		i/=10;
	}
	return &str[p+1];
}

char* word2char(wchar_t* in)
{
	int n,i;
	for(n=0;in[n]!=0;n++);
	char* out=(char*)malloc(sizeof(char*)*n);
	for(i=0;i<n;i++)
	{
		out[i]=(char)in[i];
	}
	out[n]=0;
	return out;
}
