#pragma once
#include "PotCTRLServer.h"

class CDlgUnit : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUnit)

public:
	CDlgUnit(CWnd* pParent = NULL);
	virtual ~CDlgUnit();

	enum { IDD = IDD_DLG_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	int selUnitNumber;
	int unitID;
	C_ExpCondition expCondition;
private:
	CFont *dlgFontLarge;
	CFont *dlgFontMiddle;
	void updateDLG(void);
	void setDLG(int _mode);
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedUnitDlgBtnSetCondition();
	afx_msg void OnBnClickedUnitDlgBtnOpenexp();
	afx_msg void OnClickedCTBTN(UINT nRes);
	afx_msg void OnBnClickedUnitDlgBtnUnsetExp();
	afx_msg void OnBnClickedUnitDlgBtnNewexp();
	afx_msg void OnClickedCheckWater(UINT nRes);
	afx_msg void OnClickedCheckWaterOpen(UINT nRes);
	afx_msg void OnSelchangeExpConditionComboPerson();
	virtual BOOL OnInitDialog();
};
