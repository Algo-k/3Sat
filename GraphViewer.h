

#pragma once


// CGraphViewer dialog

class CGraphViewer : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphViewer)

public:
    int AlgoNum;
    double AlgoData[6][1000];
    int BeginSize;
    int EndSize;
    int StepSize;
    int XMargin;
    int YMargin;
    int YMax;
    int YNum;
    int LOG;
	CGraphViewer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphViewer();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
//	CStatic m_picture;
};
