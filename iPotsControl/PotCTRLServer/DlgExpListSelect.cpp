#include "stdafx.h"
#include "PotCTRLServer.h"
#include "DlgExpListSelect.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDlgExpListSelect, CDialogEx)

CDlgExpListSelect::CDlgExpListSelect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgExpListSelect::IDD, pParent)
{
	selCursor = -1;
	selSID = -1;
	sidList = NULL;
}

CDlgExpListSelect::~CDlgExpListSelect()
{
}

void CDlgExpListSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgExpListSelect, CDialogEx)
END_MESSAGE_MAP()

void CDlgExpListSelect::OnCancel()
{
	selSID = -1;
	CDialogEx::OnCancel();
}

BOOL CDlgExpListSelect::OnInitDialog()
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
	CString outTXT, tmpTXT;
	int numList;
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp *)AfxGetApp();
	numList = pApp->dbLoadExpList(1);
	delete[] sidList;
	sidList = new int[numList];
	for (int i = 0; i < numList; i++){
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
	return TRUE;
}

void CDlgExpListSelect::OnOK()
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
