#include "stdafx.h"
#include "PotCTRLServer.h"
#include "DlgExpCondition.h"
#include "afxdialogex.h"
#include "PotCTRLServer.h"

IMPLEMENT_DYNAMIC(CDlgExpCondition, CDialogEx)

CDlgExpCondition::CDlgExpCondition(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgExpCondition::IDD, pParent)
{
	DLG_MODE = 0;
}

CDlgExpCondition::~CDlgExpCondition()
{
}

void CDlgExpCondition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgExpCondition, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_EXP_CONDITION_CHECK_WATER_1, IDC_EXP_CONDITION_CHECK_WATER_6, OnClickedExpConditionCheckWater)
	ON_CBN_SELCHANGE(IDC_EXP_CONDITION_COMBO_PERSON, &CDlgExpCondition::OnSelchangeExpConditionComboPerson)
END_MESSAGE_MAP()

void CDlgExpCondition::OnCancel()
{
	CDialogEx::OnCancel();
}

BOOL CDlgExpCondition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();

	CEdit *edit;
	CComboBox *combo;
	CButton *btn;
	CString tmpTXT;
	int i;

	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
	edit->SetWindowText(expCondition.NAME);
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
	edit->SetWindowText(expCondition.PERSON);
	pApp->dbGetPersonList();
	combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
	for (int i = 0; i < pApp->nPersonTXT; i++){
		combo->AddString(pApp->personTXT[i]);
	}
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
	edit->SetWindowText(expCondition.MEMO);
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
	tmpTXT.Format(_T("%d"), expCondition.SID);
	edit->SetWindowText(tmpTXT);
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_DATE);
	edit->SetWindowText(expCondition.DATE);
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_UNIT);
	if (expCondition.UNIT_ID < 0){
		tmpTXT.Format(_T("no assign"));
	}
	else{
		tmpTXT.Format(_T("Unit %d"), expCondition.UNIT_ID);
	}
	edit->SetWindowText(tmpTXT);
	btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_ENABLE);
	if (expCondition.enable > 0)
		btn->SetCheck(1);
	else
		btn->SetCheck(0);
	for (i = 0; i < 6; i++){
		btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1 + i);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_WATER_1 + i);
		if (expCondition.WATER[i] < 0.0){
			btn->SetCheck(0);
			if (expCondition.WATER[i] < -100.0)
				tmpTXT.Format(_T("-999.0"));
			else if (expCondition.WATER[i] < 0.0)
				tmpTXT.Format(_T("-1"));
			else
				tmpTXT.Format(_T("%.1lf"), expCondition.WATER[i]);
			edit->SetWindowText(tmpTXT);
			edit->SetReadOnly(1);
		}
		else{
			btn->SetCheck(1);
			if (expCondition.WATER[i] < -100.0)
				tmpTXT.Format(_T("-999.0"));
			else if (expCondition.WATER[i] < 0.0)
				tmpTXT.Format(_T("-1"));
			else
				tmpTXT.Format(_T("%.1lf"), expCondition.WATER[i]);
			edit->SetWindowText(tmpTXT);
			edit->SetReadOnly(0);
		}
	}
	switch (DLG_MODE)
	{
	case 0:
		for (int i = IDC_EXP_CONDITION_EDIT_NAME; i < IDC_EXP_CONDITION_EDIT_WATER_6 + 1; i++){
			edit = (CEdit*)GetDlgItem(i);
			edit->SetReadOnly(1);
		}
		for (int i = IDC_EXP_CONDITION_CHECK_WATER_1; i < IDC_EXP_CONDITION_CHECK_ENABLE + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(FALSE);
		}
		break;
	case 1:
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->SetReadOnly(FALSE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->SetReadOnly(FALSE);
		combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
		combo->EnableWindow(TRUE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->SetReadOnly(0);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_UNIT);
		edit->SetReadOnly(1);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
		edit->SetReadOnly(1);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_DATE);
		edit->SetReadOnly(1);
		btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_ENABLE);
		btn->EnableWindow(FALSE);
		break;
	case 2:
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->SetReadOnly(FALSE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->SetReadOnly(FALSE);
		combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
		combo->EnableWindow(TRUE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->SetReadOnly(0);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_UNIT);
		edit->SetReadOnly(1);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
		edit->SetReadOnly(1);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_DATE);
		edit->SetReadOnly(1);
		btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_ENABLE);
		if (expCondition.UNIT_ID < 0){
			btn->EnableWindow(TRUE);
		}
		else{
			btn->EnableWindow(FALSE);
		}
		if (expCondition.UNIT_ID < 0){
		}
		else{
			for (int i = IDC_EXP_CONDITION_EDIT_WATER_1; i < IDC_EXP_CONDITION_EDIT_WATER_6 + 1; i++){
				edit = (CEdit*)GetDlgItem(i);
				edit->SetReadOnly(1);
			}
			for (int i = IDC_EXP_CONDITION_CHECK_WATER_1; i < IDC_EXP_CONDITION_EDIT_WATER_6 + 1; i++){
				btn = (CButton*)GetDlgItem(i);
				btn->EnableWindow(FALSE);
			}
		}
		break;
	default:
		break;
	};

	return TRUE;
}


void CDlgExpCondition::OnOK()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CEdit *edit;
	CComboBox *combo;
	CButton *btn;
	CString tmpTXT;

	switch (DLG_MODE){
	case 0:
		break;
	case 1:
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->GetWindowText(tmpTXT);
		expCondition.NAME = tmpTXT;
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->GetWindowText(tmpTXT);
		expCondition.PERSON = tmpTXT;
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->GetWindowText(tmpTXT);
		expCondition.MEMO = tmpTXT;
		expCondition.UNIT_ID = -1;
		expCondition.enable = -1;
		for (int i = 0; i < 6; i++){
			btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1 + i);
			if (btn->GetCheck()){
				edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_WATER_1 + i);
				edit->GetWindowText(tmpTXT);
				expCondition.WATER[i] = _tstof(tmpTXT);
			}
			else{
				expCondition.WATER[i] = -1.0;
			}
		}
		pApp->dbExpListNew(expCondition);
		break;
	case 2:
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->GetWindowText(tmpTXT);
		expCondition.NAME = tmpTXT;
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->GetWindowText(tmpTXT);
		expCondition.PERSON = tmpTXT;
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->GetWindowText(tmpTXT);
		expCondition.MEMO = tmpTXT;
		btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_ENABLE);
		if (btn->GetCheck())
			expCondition.enable = 1;
		else
			expCondition.enable = 0;
		for (int i = 0; i < 6; i++){
			btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1 + i);
			if (btn->GetCheck()){
				edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_WATER_1 + i);
				edit->GetWindowText(tmpTXT);
				expCondition.WATER[i] = _tstof(tmpTXT);
			}
			else{
				expCondition.WATER[i] = -1.0;
			}
		}
		pApp->dbExpListModify(expCondition);
		break;
	}

	CDialogEx::OnOK();
}

void CDlgExpCondition::OnClickedExpConditionCheckWater(UINT nRes)
{
	CButton *chk = (CButton*)GetDlgItem(nRes);
	CEdit *edit = (CEdit*)GetDlgItem(nRes - IDC_EXP_CONDITION_CHECK_WATER_1 + IDC_EXP_CONDITION_EDIT_WATER_1);
	CString tmpTXT;

	if (chk->GetCheck()){
		tmpTXT = _T("200.0");
		edit->SetWindowText(tmpTXT);
		edit->SetReadOnly(0);
	}
	else{
		tmpTXT = _T("-1.0");
		edit->SetWindowText(tmpTXT);
		edit->SetReadOnly(1);
	}
}
void CDlgExpCondition::OnSelchangeExpConditionComboPerson()
{
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
	CComboBox *combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
	CString tmpTXT;
	combo->GetLBText(combo->GetCurSel(), tmpTXT);
	edit->SetWindowText(tmpTXT);
}
