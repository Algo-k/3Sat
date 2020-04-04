// GraphViewer.cpp : implementation file
//

#include "stdafx.h"
#include "3SATC++.h"
#include "GraphViewer.h"
#include "afxdialogex.h"
#include "math.h"

// CGraphViewer dialog

IMPLEMENT_DYNAMIC(CGraphViewer, CDialogEx)

CGraphViewer::CGraphViewer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphViewer::IDD, pParent)
{

}

CGraphViewer::~CGraphViewer()
{
}

void CGraphViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_PICTURE, m_picture);
}



BEGIN_MESSAGE_MAP(CGraphViewer, CDialogEx)
//	ON_WM_PAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphViewer message handlers

void CGraphViewer::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect r(0,0,1000,700);
	CBrush b(0xffffff);
	dc.SelectObject(this->GetFont());
	dc.FillRect(r,&b);
	dc.MoveTo(XMargin,700-YMargin);
	dc.LineTo(1000-XMargin,700-YMargin);
	dc.MoveTo(XMargin,700-YMargin);
	dc.LineTo(XMargin,YMargin);
	int YSize=700-YMargin*2;
	int XSize=1000-XMargin*2;
	int j=0;
	if(LOG)
	{
		for(int i=1;i<=YMax;i*=10)
		{
			int num=i;
			if(num==1)
				num=0;
			dc.TextOutW(10,700-YMargin-(j*YSize)/YNum-4,(LPCTSTR)int2str(num));
			j++;
		}
	}
	else
	{
		for(int i=0;i<=YMax;i+=YMax/10)
		{
			dc.TextOutW(10,700-YMargin-(i*YSize)/YMax-4,(LPCTSTR)int2str(i));
		}
	}
	int NumSize=(EndSize-BeginSize)/StepSize;
	j=0;
	for(int i=BeginSize;i<=EndSize;i+=StepSize)
	{
		dc.TextOutW(XMargin+(j*XSize)/NumSize-4,700-YMargin+10,(LPCTSTR)int2str(i));
		j++;
	}
	CPen blue(0,2,RGB(0,0,255));
	CPen red(0,2,RGB(255,0,0));
	CPen green(0,2,RGB(0,255,0));
	dc.SelectObject(blue);
	for(int A=0;A<AlgoNum;A++)
	{
		if(A==2)dc.SelectObject(red);
		if(A==4)dc.SelectObject(green);
		int I=BeginSize;
		int PX=-1,PY=-1;
		for(int i=0;i<=NumSize;i++)
		{
			int x=XMargin+(i*XSize)/NumSize;
			double y=AlgoData[A][i];
			double Y;
			Y=y;
			if(LOG)
			{
				if(y<=1) Y=0;
				else Y=log10((double)y);
				y=700-YMargin-(Y*YSize)/YNum;
			}
			else
			{
				y=700-YMargin-(Y*YSize)/YMax;
			}
			if(PX!=-1)
			{
				int SS=9;
				if(A/2==0)
				{
					SS++;
				    dc.Ellipse(x-SS,y-SS,x+SS,y+SS);
					SS--;
				}
				else if(A/2==1)
				{
					dc.MoveTo(x-SS,y-SS);
					dc.LineTo(x-SS,y+SS);
					dc.LineTo(x+SS,y+SS);
					dc.LineTo(x+SS,y-SS);
				    dc.LineTo(x-SS,y-SS);
				}
				else if(A/2==2)
				{
					SS--;
					dc.MoveTo(x,y+SS);
					dc.LineTo(x-SS,y);
					dc.LineTo(x,y-SS);
					dc.LineTo(x+SS,y);
					dc.LineTo(x,y+SS);
					SS++;
				}
				dc.MoveTo(PX,PY);
				dc.LineTo(x,y);
			}
			PX=x;
			PY=y;
			I+=StepSize;
		}
	}
}
