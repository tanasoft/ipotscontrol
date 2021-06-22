#pragma once

class CDlgExpList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExpList)

public:
	CDlgExpList(CWnd* pParent = NULL);
	virtual ~CDlgExpList();

	enum { IDD = IDD_DLG_EXPLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	int selCursor;
public:
	int selSID;
	int *sidList;
	afx_msg void OnBnClickedExplistBtnNew();
	afx_msg void OnBnClickedExplistBtnModify();
	void updateDLG();
};
