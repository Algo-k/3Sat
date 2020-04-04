
// 3SATC++Dlg.h : header file
//

#pragma once


// CMy3SATCDlg dialog
class CMy3SATCDlg : public CDialogEx
{
// Construction
public:
	void CMy3SATCDlg::RunFolder();
	CMy3SATCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MY3SATC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void DrawRandomBnClickedButton();
//	afx_msg void OnBnClickedButton2();
	CEdit m_sucbox;
	afx_msg void DrawBenchmarkBnClickedButton();
	CEdit m_timeout;
//	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton2();
	CEdit m_begin;
	CEdit m_end;
	CEdit m_step;
};
