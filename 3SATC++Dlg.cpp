
// 3SATC++Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "3SATC++.h"
#include "3SATC++Dlg.h"
#include "afxdialogex.h"
#include "PSSP.h"
#include "GraphViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy3SATCDlg dialog




CMy3SATCDlg::CMy3SATCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy3SATCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3SATCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, m_sucbox);
	//DDX_Control(pDX, IDC_EDIT2, m_timeout);
	DDX_Control(pDX, IDC_BEGIN, m_begin);
	DDX_Control(pDX, IDC_END, m_end);
	DDX_Control(pDX, IDC_STEP, m_step);
}

BEGIN_MESSAGE_MAP(CMy3SATCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON1, &CMy3SATCDlg::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, &CMy3SATCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMy3SATCDlg::DrawBenchmarkBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON1, &CMy3SATCDlg::DrawRandomBnClickedButton)
//	ON_BN_CLICKED(IDC_BUTTON2, &CMy3SATCDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMy3SATCDlg message handlers

BOOL CMy3SATCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy3SATCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy3SATCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy3SATCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void alloc()
{
    register int i, j;
	int lit, r;
	int clauseSize;
	int tatom;
	char c;

    allocateMemory();
	int *numOccurrenceT = (int*) malloc(sizeof(int) * (numLiterals + 1));

	for(i=0;i<=numLiterals;i++)
	{
		numOccurrence[i]=0;
	    numOccurrenceT[i] = 0;
	}

	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			numOccurrenceT[lit + numVars]++;
			j++;
		}
	}

    for (i = 0; i < numLiterals + 1; i++) {
		occurrence[i] = (int*) malloc(sizeof(int) * (numOccurrenceT[i] + 1));
		occurrence[i][numOccurrenceT[i]]=0;
		if (numOccurrenceT[i] > maxNumOccurences)
			maxNumOccurences = numOccurrenceT[i];
	}
	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			occurrence[lit + numVars][numOccurrence[lit + numVars]++] = i;
			j++;
		}
		///////////////////// It was a bug in adrian's main code! ///////////////////////////
		//occurrence[lit + numVars][numOccurrence[lit + numVars]] = 0; //sentinel at the end!
	}
	probs = (double*) malloc(sizeof(double) * (numVars + 1));
	breaks = (int*) malloc(sizeof(int) * (numVars + 1));
	free(numOccurrenceT);
}
void allocCD()
{
    register int i, j;
	int lit, r;
	int clauseSize;
	int tatom;
	char c;

    allocateMemoryCD();
	int *numOccurrenceT = (int*) malloc(sizeof(int) * (numLiterals + 1));

	for(i=0;i<=numLiterals;i++)
	{
		numOccurrence[i]=0;
	    numOccurrenceT[i] = 0;
	}

	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			numOccurrenceT[lit + numVars]++;
			j++;
		}
	}

    for (i = 0; i < numLiterals + 1; i++) {
		occurrence[i] = (int*) malloc(sizeof(int) * (numOccurrenceT[i] + 1));
		occurrence[i][numOccurrenceT[i]]=0;
		if (numOccurrenceT[i] > maxNumOccurences)
			maxNumOccurences = numOccurrenceT[i];
	}
	for (i = 1; i <= numClauses; i++) {
		j = 0;
		while ((lit = clause[i][j])) {
			occurrence[lit + numVars][numOccurrence[lit + numVars]++] = i;
			j++;
		}
		///////////////////// It was a bug in adrian's main code! ///////////////////////////
		//occurrence[lit + numVars][numOccurrence[lit + numVars]] = 0; //sentinel at the end!
	}
	probs = (double*) malloc(sizeof(double) * (numVars + 1));
	breaks = (int*) malloc(sizeof(int) * (numVars + 1));
	free(numOccurrenceT);
}
void BreakOnlyFree()
{
	free(breaks);
	free(probs);
	for(int i=1;i<=numVars;i++)
	{
		free(occurrence[i+numVars]);
		free(occurrence[-i+numVars]);
	}
	free(occurrence);
	free(atom);
	free(numOccurrence);
	free(critVar);
	free(falseClause);
	free(whereFalse);
	free(numTrueLit);
}
void CDFree()
{
	free(breaks);
	free(probs);
	free(C);
	for(int i=1;i<=numVars;i++)
	{
		free(occurrence[i+numVars]);
		free(occurrence[-i+numVars]);
	}
	free(occurrence);
	free(atom);
	free(numOccurrence);
	free(critVar);
	free(falseClause);
	free(whereFalse);
	free(numTrueLit);
}
int CallPolyModified()
{
	maxClauseSize=3;
	minClauseSize=3;
	caching_spec=false;
	cb_spec=false;
	fct_spec=true;
	maxFlips=3*numVars;
	alloc();
	return BreakOnlyModified();
}

int CallCD()
{
	maxClauseSize=3;
	minClauseSize=3;
	caching_spec=false;
	cb_spec=false;
	fct_spec=true;
	maxFlips=3*numVars;
	allocCD();
	return BreakOnlyModified();
}

int CallPoly()
{
	maxClauseSize=3;
	minClauseSize=3;
	caching_spec=false;
	cb_spec=false;
	fct_spec=true;
	fct=0;
	maxFlips=3*numVars;
	alloc();
	return BreakOnly();
}
int CallEXP()
{
	maxClauseSize=3;
	minClauseSize=3;
	caching_spec=false;
	cb_spec=false;
	fct_spec=true;
	fct=1;
	maxFlips=3*numVars;
	alloc();
	return BreakOnly();
}


void CMy3SATCDlg::DrawRandomBnClickedButton()
{
	CGraphViewer v;

	int NumTest=2;
    v.AlgoNum=6;
	v.LOG=false;
	
	CString str_begin;
	CString str_end;
	CString str_step;
	m_begin.GetWindowTextW(str_begin);
	m_end.GetWindowTextW(str_end);
	m_step.GetWindowTextW(str_step);
	v.BeginSize= StrToIntW(str_begin);
	v.EndSize= StrToIntW(str_end);
	v.StepSize= StrToIntW(str_step);

	int DataPtr=0;
	double AVG1,AVG2,AVG3;
	double MAX1,MAX2,MAX3;
	double time;
	for(int size=v.BeginSize;size<=v.EndSize;size+=v.StepSize)
	{
		AVG1=0;
		AVG2=0;
		AVG3=0;
		MAX1=0;
		MAX2=0;
		MAX3=0;
		for(int t=0;t<NumTest;t++)
		{
			Produce(size,0.1);

			BeginTime();
    		CallPoly();
			time=EndTime();
			AVG1+=time;
			if(time>MAX1)MAX1=time;
			BreakOnlyFree();
			
			BeginTime();
			CallPolyModified();
			time=EndTime();
			AVG2+=time;
			if(time>MAX2)MAX2=time;
			BreakOnlyFree();


			BeginTime();
			CallCD();
			time=EndTime();
			AVG3+=time;
			if(time>MAX3)MAX3=time;
			CDFree();

			SATFree();
		}
		AVG1/=NumTest;
		AVG2/=NumTest;
		AVG3/=NumTest;
		v.AlgoData[0][DataPtr]=AVG1;
		v.AlgoData[1][DataPtr]=MAX1;
		v.AlgoData[2][DataPtr]=AVG2;
		v.AlgoData[3][DataPtr]=MAX2;
		v.AlgoData[4][DataPtr]=AVG3;
		v.AlgoData[5][DataPtr]=MAX3;
		DataPtr++;
	}
    v.XMargin=32+16;
    v.YMargin=32;
    v.YMax=500;
    v.YNum=3;
	v.DoModal();
}

int Stage = 0;
int Cls[3];
int ClsI = 0;
int CI = 1;
int CI2 = 1;
void Initialize()
{
    ClsI = 0;
    Stage = 0;
}
void SubmitNum(int num)
{
    if (Stage == 0)
    {
        numVars = num;
        matrix = (int*)malloc(sizeof(int)*num);
    }
    else if (Stage == 1)
    {
        numClauses = num;
        clause = (int**)malloc(sizeof(int*)*num*2);
    }
    else
    {
        Cls[ClsI++] = num;
    }
    Stage++;
}
void SubmitCls(int c1, int c2, int c3)
{
	clause[CI]=(int*)malloc(sizeof(int)*4);
	clause[CI][0]=c1;
	clause[CI][1]=c2;
	clause[CI][2]=c3;
	clause[CI][3]=0;
    CI++;
}
void EndClause()
{
    if (ClsI == 1)
    {
        int R = 0;
        do
        {
            R = rand() % numVars + 1;
        } while (R == abs(Cls[0]));
        int S = 0;
        do
        {
            S = rand() % numVars + 1;
        } while (S == abs(Cls[0]) || S==R);
        SubmitCls(Cls[0], S, R);
        SubmitCls(Cls[0], S, -R);
        SubmitCls(Cls[0], -S, R);
        SubmitCls(Cls[0], -S, -R);
    }
    else if (ClsI == 2)
    {
        int R = 0;
        do
        {
            R = rand() % numVars + 1;
        } while (R == abs(Cls[0]) || R == abs(Cls[1]));
        SubmitCls(Cls[0], Cls[1], R);
        SubmitCls(Cls[0], Cls[1], -R);
    }
    else
    {
        SubmitCls(Cls[0], Cls[1], Cls[2]);
    }
    ClsI = 0;
    CI2++;
}

void OpenFile(wchar_t* str)
{
	FILE* f;
	    f=fopen((const char*)word2char(str),"r");
        int A = 0;
        while (A < 5)
        {
            char c = 10;
            while (c == 10) c = (char)getc(f);
		    //Sleep(1);
            if (A == 0 && c == 'p')
                A++;
            else if (A == 1 && c == ' ')
                A++;
            else if (A == 2 && c == 'c')
                A++;
            else if (A == 3 && c == 'n')
                A++;
            else if (A == 4 && c == 'f')
                A++;
            else
                A = 0;
        }
        int State = 0; // 0 space 1 number 2 End
        int Number = 1;
        numClauses = 10000000;
        Stage = 0;
        CI = 1;
        CI2 = 1;
        ClsI = 0;
        int ccc = 0;
        while (State != 2 && CI2 < numClauses)
        {
            ccc++;
            //if (ccc % 5 == 0)
            //    Sleep(1);
            char c = (char)getc(f);
            if (c == 255)
            {
                State = 2;
            }
            else if (State == 0)
            {
                if (c == '0')
                {
                    EndClause();
                    State = 0;
                }
                else if ('1' <= c && c <= '9')
                {
                    Number = c - '0';
                    State = 1;
                }
                else if (c == '-')
                {
                    c = 10;
                    while (c == 10) c = getc(f);
                    Number = -c + '0';
                    State = 1;
                }
            }
            else if (State == 1)
            {
                if ('0' <= c && c <= '9')
                {
                    if (Number > 0)
                        Number = Number * 10 + (c - '0');
                    else
                        Number = Number * 10 - (c - '0');
                }
                else if (c == '-')
                {
                    SubmitNum(Number);
                    c = 10;
                    while (c == 10) c = getc(f);
                    Number = -c + '0';
                    State = 1;
                }
                else
                {
                    State = 0;
                    SubmitNum(Number);
                }
            }
        }
        fclose(f);
        numClauses = CI-1;
}

void CMy3SATCDlg::RunFolder()
{
    int Suc = 0;
	CFileDialog openFileDialog1(true,0,0, 6UL | OFN_ALLOWMULTISELECT,0,0,0UL,1);
	wchar_t* pszFile = new wchar_t[32767];
    ZeroMemory(pszFile,32767*sizeof(wchar_t));
    openFileDialog1.m_ofn.lpstrFile=pszFile;
    openFileDialog1.m_ofn.nMaxFile=32767;
    openFileDialog1.m_ofn.nFileOffset=0;

	openFileDialog1.DoModal();
	CString name;
	int i;
	POSITION P;
    P=openFileDialog1.GetStartPosition();
	double AVG=0;
	double MAX=0;
    for (i = 0; P; i++)
    {
		name=openFileDialog1.GetNextPathName(P);
		wchar_t* str=name.GetBuffer();
		OpenFile(str);
		BeginTime();
        //if (_PSSP1()==10)
		if(CallPoly()==10)
		{
            Suc++;
			AVG+=numSelectOperation;
			if(numSelectOperation>MAX)MAX=numSelectOperation;
		}
		//_PSSPFree();
		BreakOnlyFree();
    }
	CString Out1;
	AVG/=Suc;
	Out1.Format(_T("Success=%d / %d AVG=%f MAX=%f"),Suc,i,AVG,MAX);
    this->m_sucbox.SetWindowTextW(Out1);
}

//void CMy3SATCDlg::OnBnClickedButton2()
//{
//	CString S;
//	m_timeout.GetWindowTextW(S);
//	___TimeOut=wcstod(S.GetBuffer(),0);
//	RunFolder();
//}

void CMy3SATCDlg::DrawBenchmarkBnClickedButton()
{
	CGraphViewer v;
	v.AlgoNum=6;
    CString str_begin;
	CString str_end;
	CString str_step;
	m_begin.GetWindowTextW(str_begin);
	m_end.GetWindowTextW(str_end);
	m_step.GetWindowTextW(str_step);
	v.BeginSize= StrToIntW(str_begin);
	v.EndSize= StrToIntW(str_end);
	v.StepSize= StrToIntW(str_step);
	v.LOG=true;
	int DataPtr=0;
	double AVG1,AVG2,AVG3;
	double MAX1,MAX2,MAX3;
	double time;
	CFileDialog** openFileDialog;
	openFileDialog=(CFileDialog**)malloc(sizeof(CFileDialog*)*(v.EndSize-v.BeginSize)/v.StepSize);
	int c=0;
	for(int size=v.BeginSize;size<=v.EndSize;size+=v.StepSize)
	{
		CString t;
		t.Format(_T("Selecting Size = %d Instances"),size);
		openFileDialog[c]=(new CFileDialog(true,0,0, 6UL | OFN_ALLOWMULTISELECT,0,0,0UL,1));
	    openFileDialog[c]->m_ofn.lpstrTitle = t;
		wchar_t* pszFile = new wchar_t[32767];
		ZeroMemory(pszFile,32767*sizeof(wchar_t));
		openFileDialog[c]->m_ofn.lpstrFile=pszFile;
		openFileDialog[c]->m_ofn.nMaxFile=32767;
		openFileDialog[c]->m_ofn.nFileOffset=0;
		openFileDialog[c]->DoModal();
		c++;
	}
	c=0;
	for(int size=v.BeginSize;size<=v.EndSize;size+=v.StepSize)
	{
		AVG1=0;
		AVG2=0;
		AVG3=0;
		MAX1=0;
		MAX2=0;
		MAX3=0;

		POSITION P=openFileDialog[c]->GetStartPosition();
		int t=0;
		for(t=0;P;t++)
		{
			CString str=openFileDialog[c]->GetNextPathName(P);
			wchar_t* wstr=str.GetBuffer();
			OpenFile(wstr);

			BeginTime();
			CallPoly();
			time=EndTime();
			AVG1+=time;
			if(time>MAX1)MAX1=time;
			BreakOnlyFree();
			
			BeginTime();
			CallPolyModified();
			time=EndTime();
			AVG2+=time;
			if(time>MAX2)MAX2=time;
			BreakOnlyFree();

			BeginTime();
			CallCD();
			time=EndTime();
			AVG3+=time;
			if(time>MAX3)MAX3=time;
			CDFree();


			SATFree();
		}
		AVG1/=t;
		AVG2/=t;
		AVG3/=t;
		v.AlgoData[0][DataPtr]=AVG1;
		v.AlgoData[1][DataPtr]=MAX1;
		v.AlgoData[2][DataPtr]=AVG2;
		v.AlgoData[3][DataPtr]=MAX2;
		v.AlgoData[4][DataPtr]=AVG3;
		v.AlgoData[5][DataPtr]=MAX3;
		DataPtr++;
		c++;
	}
    v.XMargin=32+16;
    v.YMargin=32;
    v.YMax=10000;
    v.YNum=3;
	v.DoModal();
}


