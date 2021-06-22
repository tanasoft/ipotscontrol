#include "stdafx.h"
#include "PotCTRLServer.h"
#include "DlgUnit.h"
#include "DlgExpList.h"
#include "DlgExpListSelect.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgUnit, CDialogEx)

CDlgUnit::CDlgUnit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgUnit::IDD, pParent)
{
	selUnitNumber = 0;
	unitID = -1;
}

CDlgUnit::~CDlgUnit()
{
}

void CDlgUnit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgUnit, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_UNIT_DLG_BTN_SET_CONDITION, &CDlgUnit::OnBnClickedUnitDlgBtnSetCondition)
	ON_BN_CLICKED(IDC_UNIT_DLG_BTN_OPENEXP, &CDlgUnit::OnBnClickedUnitDlgBtnOpenexp)
	ON_BN_CLICKED(IDC_UNIT_DLG_BTN_UNSET_EXP, &CDlgUnit::OnBnClickedUnitDlgBtnUnsetExp)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_UNIT_DLG_BTN_CT_1, IDC_UNIT_DLG_BTN_CT_4, OnClickedCTBTN)
	ON_BN_CLICKED(IDC_UNIT_DLG_BTN_NEWEXP, &CDlgUnit::OnBnClickedUnitDlgBtnNewexp)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_EXP_CONDITION_CHECK_WATER_1, IDC_EXP_CONDITION_CHECK_WATER_6, OnClickedCheckWater)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_EXP_CONDITION_CHECK_WATER_OPEN_1, IDC_EXP_CONDITION_CHECK_WATER_OPEN_6, OnClickedCheckWaterOpen)
	ON_CBN_SELCHANGE(IDC_EXP_CONDITION_COMBO_PERSON, OnSelchangeExpConditionComboPerson)
END_MESSAGE_MAP()

BOOL CDlgUnit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CFont  *curFont;
	LOGFONT newfont;
	CEdit *edit;
	CButton *btn;
	CString tmpTXT;

	edit = (CEdit*)GetDlgItem(IDC_UNIT_DLG_EDIT_UNIT);
	curFont = edit->GetFont();
	curFont->GetLogFont(&newfont);
	newfont.lfHeight = 40;
	newfont.lfWidth = 20;
	newfont.lfWeight = FW_HEAVY;
	dlgFontLarge = new CFont;
	dlgFontLarge->CreateFontIndirect(&newfont);
	edit->SetFont(dlgFontLarge);
	tmpTXT.Format(_T("Unit %d"), unitID);
	edit->SetWindowText(tmpTXT);
	btn = (CButton*)GetDlgItem(IDC_UNIT_DLG_BTN_CT_1);
	curFont = btn->GetFont();
	curFont->GetLogFont(&newfont);
	newfont.lfHeight = 12;
	newfont.lfWidth = 10;
	newfont.lfWeight = FW_BOLD;
	dlgFontMiddle = new CFont;
	dlgFontMiddle->CreateFontIndirect(&newfont);
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
	edit->SetFont(dlgFontLarge);
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
	edit->SetFont(dlgFontLarge);
	pApp->dbGetPersonList();
	CComboBox *combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
	for (int i = 0; i < pApp->nPersonTXT; i++){
		combo->AddString(pApp->personTXT[i]);
	}
	if (pApp->dbGetExpCondition(unitID, &expCondition) < 1){
		expCondition.Clear();
		setDLG(0);
	}
	else{
		setDLG(1);
	}
	updateDLG();

	return TRUE;
}

void CDlgUnit::updateDLG(void)
{
	CEdit *edit;
	CButton *chk;
	CString tmpTXT;

	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	pApp->unitDevice[selUnitNumber].GET_Data();
	if (expCondition.SID > 0){
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
		tmpTXT.Format(_T("%d"), expCondition.SID);
		edit->SetWindowText(tmpTXT);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		tmpTXT.Format(_T("%s"), expCondition.NAME);
		edit->SetWindowText(tmpTXT);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		tmpTXT.Format(_T("%s"), expCondition.PERSON);
		edit->SetWindowText(tmpTXT);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		tmpTXT.Format(_T("%s"), expCondition.MEMO);
		edit->SetWindowText(tmpTXT);
	}
	else{
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
		tmpTXT = _T("New");
		edit->SetWindowText(tmpTXT);
		tmpTXT = _T("");
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->SetWindowText(tmpTXT);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->SetWindowText(tmpTXT);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->SetWindowText(tmpTXT);
	}
	for (int i = 0; i < NUM_POTS; i++){
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_WATER_1 + i);
		if (expCondition.WATER[i] < -100.0){
			chk = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_OPEN_1 + i);
			chk->SetCheck(1);
			chk = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1 + i);
			chk->SetCheck(0);
			tmpTXT.Format(_T(""));
			edit->SetWindowText(tmpTXT);
			edit->SetReadOnly(1);
		}
		else if( expCondition.WATER[i] < 0 ){
			chk = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_OPEN_1 + i);
			chk->SetCheck(0);
			chk = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1 + i);
			chk->SetCheck(0);
			tmpTXT.Format(_T(""));
			edit->SetWindowText(tmpTXT);
			edit->SetReadOnly(1);
		}
		else{
			chk = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_OPEN_1 + i);
			chk->SetCheck(0);
			chk = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1 + i);
			chk->SetCheck(1);
			tmpTXT.Format(_T("%.1lf"), expCondition.WATER[i]);
			edit->SetWindowText(tmpTXT);
			edit->SetReadOnly(0);
		}
	}
}
void CDlgUnit::setDLG(int _mode)
{
	CButton *btn;
	CEdit *edit;
	CComboBox *combo;

	switch (_mode){
	case 0:
		btn = (CButton*)GetDlgItem(IDC_UNIT_DLG_BTN_UNSET_EXP);
		btn->EnableWindow(FALSE);
		for (int i = IDC_UNIT_DLG_BTN_CT_1; i < IDC_UNIT_DLG_BTN_SET_CONDITION + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(TRUE);
		}
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->EnableWindow(TRUE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->EnableWindow(TRUE);
		combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
		combo->EnableWindow(TRUE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->EnableWindow(TRUE);
		for (int i = IDC_EXP_CONDITION_EDIT_WATER_1; i < IDC_EXP_CONDITION_EDIT_WATER_6 + 1; i++){
			edit = (CEdit*)GetDlgItem(i);
			edit->EnableWindow(TRUE);
		}
		for (int i = IDC_EXP_CONDITION_CHECK_WATER_1; i < IDC_EXP_CONDITION_CHECK_WATER_OPEN_6 + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(TRUE);
		}
		break;
	case 1:
		btn = (CButton*)GetDlgItem(IDC_UNIT_DLG_BTN_UNSET_EXP);
		btn->EnableWindow(TRUE);
		btn = (CButton*)GetDlgItem(IDC_UNIT_DLG_BTN_NEWEXP);
		btn->EnableWindow(FALSE);
		btn = (CButton*)GetDlgItem(IDC_UNIT_DLG_BTN_OPENEXP);
		btn->EnableWindow(FALSE);
		btn = (CButton*)GetDlgItem(IDC_UNIT_DLG_BTN_SET_CONDITION);
		btn->EnableWindow(FALSE);
		for (int i = IDC_UNIT_DLG_BTN_CT_1; i < IDC_UNIT_DLG_BTN_CT_4 + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(TRUE);
		}
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->EnableWindow(FALSE);
		combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
		combo->EnableWindow(FALSE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->EnableWindow(FALSE);
		for (int i = IDC_EXP_CONDITION_EDIT_WATER_1; i < IDC_EXP_CONDITION_EDIT_WATER_6 + 1; i++){
			edit = (CEdit*)GetDlgItem(i);
			edit->EnableWindow(FALSE);
		}
		for (int i = IDC_EXP_CONDITION_CHECK_WATER_1; i < IDC_EXP_CONDITION_CHECK_WATER_OPEN_6 + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(FALSE);
		}
		break;
	case 2:
		for (int i = IDC_UNIT_DLG_BTN_CT_1; i < IDC_UNIT_DLG_BTN_UNSET_EXP + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(FALSE);
		}
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
		edit->EnableWindow(FALSE);
		combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
		combo->EnableWindow(FALSE);
		edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
		edit->EnableWindow(FALSE);
		for (int i = IDC_EXP_CONDITION_EDIT_WATER_1; i < IDC_EXP_CONDITION_EDIT_WATER_6 + 1; i++){
			edit = (CEdit*)GetDlgItem(i);
			edit->EnableWindow(FALSE);
		}
		for (int i = IDC_EXP_CONDITION_CHECK_WATER_1; i < IDC_EXP_CONDITION_CHECK_WATER_OPEN_6 + 1; i++){
			btn = (CButton*)GetDlgItem(i);
			btn->EnableWindow(FALSE);
		}
		break;
	}
}

void CDlgUnit::OnClickedCheckWater(UINT nRes)
{
	CButton *chk = (CButton*)GetDlgItem(nRes);
	CButton *chkopen = (CButton*)GetDlgItem(nRes - IDC_EXP_CONDITION_CHECK_WATER_1 + IDC_EXP_CONDITION_CHECK_WATER_OPEN_1);
	CEdit *edit = (CEdit*)GetDlgItem(nRes - IDC_EXP_CONDITION_CHECK_WATER_1 + IDC_EXP_CONDITION_EDIT_WATER_1);
	CString tmpTXT;

	if (chk->GetCheck()){
		tmpTXT = _T("200.0");
		edit->SetWindowText(tmpTXT);
		edit->SetReadOnly(0);
 		chkopen->SetCheck(0);
		chkopen->EnableWindow(0);
	}
	else{
		tmpTXT = _T("");
		edit->SetWindowText(tmpTXT);
		edit->SetReadOnly(1);
		chkopen->SetCheck(0);
		chkopen->EnableWindow(1);
	}
}
void CDlgUnit::OnClickedCheckWaterOpen(UINT nRes)
{
	CButton *chk = (CButton*)GetDlgItem(nRes);
	CButton *chkWT = (CButton*)GetDlgItem(nRes - IDC_EXP_CONDITION_CHECK_WATER_OPEN_1 + IDC_EXP_CONDITION_CHECK_WATER_1);
	CEdit *edit = (CEdit*)GetDlgItem(nRes - IDC_EXP_CONDITION_CHECK_WATER_OPEN_1 + IDC_EXP_CONDITION_EDIT_WATER_1);
	CString tmpTXT;

	if (chk->GetCheck()){
		chkWT->SetCheck(0);
		chkWT->EnableWindow(0);
		tmpTXT = _T("");
		edit->SetWindowText(tmpTXT);
		edit->SetReadOnly(1);
	}
	else{
		chkWT->SetCheck(0);
		chkWT->EnableWindow(1);
		tmpTXT = _T("");
		edit->SetWindowText(tmpTXT);
		edit->SetReadOnly(1);
	}
}

void CDlgUnit::OnOK()
{
	CDialogEx::OnOK();
}

void CDlgUnit::OnDestroy()
{
	CDialogEx::OnDestroy();
	dlgFontLarge->DeleteObject();
	delete dlgFontLarge;
	dlgFontMiddle->DeleteObject();
	delete dlgFontMiddle;
}

void CDlgUnit::OnBnClickedUnitDlgBtnNewexp()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CButton *btn;
	expCondition.Clear();
	updateDLG();
	setDLG(0);
}

void CDlgUnit::OnBnClickedUnitDlgBtnOpenexp()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CDlgExpListSelect dlg;
	CButton *btn;
	if (dlg.DoModal() == IDOK){
		if (pApp->dbGetExpConditionSID(dlg.selSID, &expCondition) < 1){
			expCondition.Clear();
		}
		else{
			setDLG(0);
		}
	}
	updateDLG();
}

void CDlgUnit::OnBnClickedUnitDlgBtnUnsetExp()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CString tmpTXT;
	CButton *btn;
	CEdit *edit;
	C_ExpData data;
	if (MessageBox(_T("Pause system ?"), _T("Confirm"), MB_YESNO | MB_ICONWARNING) == IDYES){
		pApp->unitDevice[selUnitNumber].UNSET_Water();
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 5;
		data.VALUE = _T("0");
		pApp->dbSaveEXPDATA(data);
		MessageBox(_T("Stopped the system."), _T("Information"), MB_OK | MB_ICONINFORMATION);
		tmpTXT.Format(_T("update EXPLIST set UNIT = -1 where SID=%d and enable > 0;"), expCondition.SID);
		pApp->dbExecSQL(tmpTXT);
		setDLG(0);
	}
	updateDLG();
}

void CDlgUnit::OnBnClickedUnitDlgBtnSetCondition()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CButton *btn;
	CEdit *edit;
	CString tmpTXT;
	C_ExpData data;
	int _sid;
	float valueWT[6], valueHT[6];
	CString ctrlWT[6], ctrlHT[6];
	for (int i = 0; i < 6; i++){
		btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_OPEN_1+i);
		if (btn->GetCheck()){
			valueWT[i] = 0.0;
			ctrlWT[i] = _T("V");
			expCondition.WATER[i] = -999.0;
		}
		else{
			btn = (CButton*)GetDlgItem(IDC_EXP_CONDITION_CHECK_WATER_1+i);
			if (btn->GetCheck()){
				edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_WATER_1+i);
				edit->GetWindowText(tmpTXT);
				valueWT[i] = _tstof(tmpTXT);
				if (valueWT[i] < 0.0)
					valueWT[i] = 0.0;
				if (valueWT[i] > 200.0)
					valueWT[i] = 200.0;
				ctrlWT[i] = _T("*");
				expCondition.WATER[i] = valueWT[i];
			}
			else{
				valueWT[i] = 0.0;
				ctrlWT[i] = _T(" ");
				expCondition.WATER[i] = -1.0;
			}
		}
	}
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_NAME);
	edit->GetWindowText(tmpTXT);
	if (tmpTXT.GetLength() < 1)
		tmpTXT = _T("no text");
	expCondition.NAME = tmpTXT;
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
	edit->GetWindowText(tmpTXT);
	if (tmpTXT.GetLength() < 1)
		tmpTXT = _T("no text");
	expCondition.PERSON = tmpTXT;
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_MEMO);
	edit->GetWindowText(tmpTXT);
	if (tmpTXT.GetLength() < 1)
		tmpTXT = _T("no text");
	expCondition.MEMO = tmpTXT;
	expCondition.UNIT_ID = unitID;
	edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_SID);
	edit->GetWindowText(tmpTXT);
	if (tmpTXT == _T("New")){
		_sid = pApp->_dbGetMaxSID() + 1;
	}
	else{
		_sid = _tstoi(tmpTXT);
	}
	expCondition.SID = _sid;
	pApp->dbSaveExp(expCondition);
	pApp->unitDevice[selUnitNumber].SET_Water(valueWT, ctrlWT);
	data.SID = expCondition.SID;
	data.UNIT_ID = expCondition.UNIT_ID;
	data.TARGET = 5;
	data.VALUE = _T("");
	for (int i = 0; i < 6; i++){
		if (ctrlWT[i] == _T("V")){
			tmpTXT.Format(_T("V;"));
		}
		else if (ctrlWT[i] == _T("*")){
			tmpTXT.Format(_T("%.1lf;"), valueWT[i]);
		}
		else{
			tmpTXT.Format(_T("-;"));
		}
		data.VALUE += tmpTXT;
	}
	pApp->dbSaveEXPDATA(data);
	pApp->dbGetExpCondition(unitID, &expCondition);
	updateDLG();
	setDLG(1);
	MessageBox(_T("Started the system"), _T("Information"), MB_OK | MB_ICONINFORMATION);
}

void CDlgUnit::OnClickedCTBTN(UINT nRes)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	C_ExpData data;
	switch (nRes){
	case IDC_UNIT_DLG_BTN_CT_1:
		pApp->unitDevice[selUnitNumber].SET_CTMode(1);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("00");
		pApp->dbSaveEXPDATA(data);
		break;
	case IDC_UNIT_DLG_BTN_CT_2:
		pApp->unitDevice[selUnitNumber].SET_CTMode(2);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("01");
		pApp->dbSaveEXPDATA(data);
		break;
	case IDC_UNIT_DLG_BTN_CT_3:
		pApp->unitDevice[selUnitNumber].SET_CTMode(3);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("02");
		pApp->dbSaveEXPDATA(data);
		break;
	case IDC_UNIT_DLG_BTN_CT_4:
		pApp->unitDevice[selUnitNumber].SET_CTMode(0);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("03");
		pApp->dbSaveEXPDATA(data);
		break;
	}
}

void CDlgUnit::OnSelchangeExpConditionComboPerson()
{
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EXP_CONDITION_EDIT_PERSON);
	CComboBox *combo = (CComboBox*)GetDlgItem(IDC_EXP_CONDITION_COMBO_PERSON);
	CString tmpTXT;
	combo->GetLBText(combo->GetCurSel(), tmpTXT);
	edit->SetWindowText(tmpTXT);
}