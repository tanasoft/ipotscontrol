
#pragma once

class CPotCTRLServerDlg : public CDialogEx
{
public:
	CPotCTRLServerDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_POTCTRLSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStartMonitor();
	afx_msg void OnBTN_UNIT(UINT nID);
	afx_msg void OnBTN_CTMODE(UINT nID);
	afx_msg void OnBTN_POTMOVE(UINT nID);

	void updateDiglog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	bool flgMonitorUnitStatus;
	afx_msg void OnBnClickedBtnExplist();

private:
	CFont *dlgFontLarge;
	CFont *dlgFontMiddle;
private:
	CCButton *unitBTN[30];
	CCButton *ctBTN[30];
	CCButton *potBTN[30];
};
