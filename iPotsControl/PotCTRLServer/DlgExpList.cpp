#include "stdafx.h"
#include "PotCTRLServer.h"
#include "DlgExpList.h"
#include "afxdialogex.h"
#include "DlgExpCondition.h"

IMPLEMENT_DYNAMIC(CDlgExpList, CDialogEx)

CDlgExpList::CDlgExpList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgExpList::IDD, pParent)
{
	selCursor = -1;
	selSID = -1;
	sidList = NULL;
}

CDlgExpList::~CDlgExpList()
{
}

void CDlgExpList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgExpList, CDialogEx)
	ON_BN_CLICKED(IDC_EXPLIST_BTN_NEW, &CDlgExpList::OnBnClickedExplistBtnNew)
	ON_BN_CLICKED(IDC_EXPLIST_BTN_MODIFY, &CDlgExpList::OnBnClickedExplistBtnModify)
END_MESSAGE_MAP()

void CDlgExpList::OnCancel()
{
	selSID = -1;
	CDialogEx::OnCancel();
}

BOOL CDlgExpList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CListCtrl *lstCtrl = (CListCtrl*)GetDlgItem(IDC_EXPLIST_LIST);
	lstCtrl->InsertColumn(0, _T("SID"), LVCFMT_LEFT, 50, -1);
	lstCtrl->InsertColumn(1, _T("UNIT"), LVCFMT_LEFT, 50, -1);
	lstCtrl->InsertColumn(2, _T("DATE"), LVCFMT_LEFT, 150, -1);
	lstCtrl->InsertColumn(3, _T("NAME"), LVCFMT_LEFT, 150, -1);
	lstCtrl->InsertColumn(4, _T("PERSON"), LVCFMT_LEFT, 150, -1);
	lstCtrl->InsertColumn(5, _T("MEMO"), LVCFMT_LEFT, 200, -1);
	lstCtrl->InsertColumn(6, _T("enable"), LVCFMT_LEFT, 50, -1);
	lstCtrl->SetExtendedStyle(lstCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	updateDLG();
	return TRUE;
}

void CDlgExpList::OnOK()
{
	CListCtrl *lstCtrl = (CListCtrl*)GetDlgItem(IDC_EXPLIST_LIST);
	int n = lstCtrl->GetNextItem(-1, LVNI_SELECTED);
	if (n > -1){
		selSID = sidList[n];
	}
	else{
		selSID = -1;
	}
	CDialogEx::OnOK();
}

void CDlgExpList::OnBnClickedExplistBtnNew()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp *)AfxGetApp();
	CDlgExpCondition dlg;
	dlg.expCondition.Clear();
	dlg.DLG_MODE = 1;
	if (dlg.DoModal() == IDOK){
		updateDLG();
	}
}

void CDlgExpList::OnBnClickedExplistBtnModify()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp *)AfxGetApp();
	CDlgExpCondition dlg;
	CListCtrl *lstCtrl = (CListCtrl*)GetDlgItem(IDC_EXPLIST_LIST);
	selCursor = lstCtrl->GetNextItem(-1, LVNI_SELECTED);
	if (selCursor > -1){
		selSID = sidList[selCursor];
		dlg.expCondition = pApp->expList[selCursor];
		dlg.DLG_MODE = 2;
		if (dlg.DoModal() == IDOK){
			updateDLG();
		}
	}
}

void CDlgExpList::updateDLG()
{
	CString outTXT, tmpTXT;
	int numList;
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp *)AfxGetApp();
	numList = pApp->dbLoadExpList(0);
	if (numList > 0) {
		delete[] sidList;
		sidList = new int[numList];
		CListCtrl* lstCtrl = (CListCtrl*)GetDlgItem(IDC_EXPLIST_LIST);
		lstCtrl->DeleteAllItems();
		for (int i = 0; i < numList; i++) {
			sidList[i] = pApp->expList[i].SID;
			tmpTXT.Format(_T("%d"), sidList[i]);
			lstCtrl->InsertItem(i, tmpTXT);
			tmpTXT.Format(_T("%d"), pApp->expList[i].UNIT_ID);
			lstCtrl->SetItemText(i, 1, tmpTXT);
			lstCtrl->SetItemText(i, 2, pApp->expList[i].DATE);
			lstCtrl->SetItemText(i, 3, pApp->expList[i].NAME);
			lstCtrl->SetItemText(i, 4, pApp->expList[i].PERSON);
			lstCtrl->SetItemText(i, 5, pApp->expList[i].MEMO);
			tmpTXT.Format(_T("%d"), pApp->expList[i].enable);
			lstCtrl->SetItemText(i, 6, tmpTXT);
		}
	}
	return;
}
