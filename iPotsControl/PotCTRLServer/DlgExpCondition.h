#pragma once

#include "PotCTRLServer.h"

class CDlgExpCondition : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgExpCondition)

public:
	CDlgExpCondition(CWnd* pParent = NULL);
	virtual ~CDlgExpCondition();
	enum { IDD = IDD_DLG_EXP_CONDITION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

public:
	int DLG_MODE;
	C_ExpCondition expCondition;
	afx_msg void OnClickedExpConditionCheckWater(UINT nRes);
	afx_msg void OnSelchangeExpConditionComboPerson();
};
