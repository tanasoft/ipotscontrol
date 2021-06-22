#pragma once

class CDlgExpListSelect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExpListSelect)

public:
	CDlgExpListSelect(CWnd* pParent = NULL);
	virtual ~CDlgExpListSelect();
	enum { IDD = IDD_DLG_EXPLIST_SELECT_EXP };
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
};
