
#include "stdafx.h"
#include "PotCTRLServer.h"
#include "PotCTRLServerDlg.h"
#include "afxdialogex.h"

#include "DlgExpList.h"
#include "DlgExpCondition.h"
#include "DlgUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT ThreadMonitorUnitStatus(LPVOID pParam)
{
	CPotCTRLServerDlg *p = (CPotCTRLServerDlg*)pParam;
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	p->flgMonitorUnitStatus = true;
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(UDPPORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	char buf[BUFSIZE_ALV];
	memset(buf, 0, sizeof(buf));
	struct sockaddr_in from_address;
	int address_size = sizeof(struct sockaddr_in);
	while (1){
		if (p->flgMonitorUnitStatus == false)
			break;
		memset(buf, 0, sizeof(buf));
		recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&from_address, &address_size);
		CString tmpTXT;
#ifdef _DEBUG
		tmpTXT = (CString)buf;
		OutputDebugString(tmpTXT);
#endif
		int n = 0;
		char s[] = ",";
		char *value;
		CString _headTXT, _comTXT, _sysTimeTXT, _statusHOST[3], _verTXT;
		int _ID, _size;
		value = strtok(buf, s);
		tmpTXT = (CString)value;
		if (tmpTXT == _T("TIS")){
			n++;
			while (value != NULL){
				switch (n){
				case 1:
					_headTXT.Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					break;
				case 2:
					_ID = atoi(value);
					break;
				case 3:
					_size = atoi(value);
					break;
				case 4:
					_comTXT.Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					break;
				case 5:
					_sysTimeTXT.Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					break;
				case 6:
					_statusHOST[0].Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					break;
				case 7:
					_statusHOST[1].Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					break;
				case 8:
					_statusHOST[2].Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					break;
				case 9:
					tmpTXT.Format(_T("%s"), static_cast<LPCTSTR>(CString(value)));
					_verTXT = tmpTXT.Left(4);
					break;
				}
				value = strtok(NULL, s);
				n++;
			}
			pApp->unitDevice[_ID - 1].UNIT_ID = _ID;
			pApp->unitDevice[_ID - 1].statusHOST[0] = _sysTimeTXT;
			pApp->unitDevice[_ID - 1].statusHOST[1] = _statusHOST[0];
			pApp->unitDevice[_ID - 1].statusHOST[2] = _statusHOST[1];
			pApp->unitDevice[_ID - 1].statusHOST[3] = _statusHOST[2];
			pApp->unitDevice[_ID - 1].statusHOST[4] = _verTXT;
			pApp->unitDevice[_ID - 1].IPAddress = inet_ntoa(from_address.sin_addr);
			pApp->unitDevice[_ID - 1].status.importTXT(_statusHOST[0], _statusHOST[1]);
		}
		p->updateDiglog();
		if (p->flgMonitorUnitStatus == false)
			break;
		Sleep(100);
	}
	closesocket(sock);
	WSACleanup();

	return 0;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CPotCTRLServerDlg::CPotCTRLServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPotCTRLServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPotCTRLServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPotCTRLServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_MONITOR, &CPotCTRLServerDlg::OnBnClickedBtnStartMonitor)
	ON_BN_CLICKED(IDC_BTN_EXPLIST, &CPotCTRLServerDlg::OnBnClickedBtnExplist)
	ON_COMMAND_RANGE(IDC_BTN_UNIT_1, IDC_BTN_UNIT_30, &CPotCTRLServerDlg::OnBTN_UNIT)
	ON_COMMAND_RANGE(IDC_BTN_UNIT_CTMODE_01, IDC_BTN_UNIT_CTMODE_30, &CPotCTRLServerDlg::OnBTN_CTMODE)
	ON_COMMAND_RANGE(IDC_BTN_UNIT_REMOVE_POT_1, IDC_BTN_UNIT_REMOVE_POT_30, &CPotCTRLServerDlg::OnBTN_POTMOVE)
END_MESSAGE_MAP()

BOOL CPotCTRLServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	ShowWindow(SW_MAXIMIZE);
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	C_ExpCondition cond;
	for (int i = 0; i < 30; i++){
		unitBTN[i] = new CCButton();
		unitBTN[i]->SubclassDlgItem(IDC_BTN_UNIT_1 + i, this);
		if (pApp->dbGetExpCondition(i + 1, &cond) < 1){
			unitBTN[i]->SetBackGroundColor(RGB(255, 0, 0));
			unitBTN[i]->SetFourColor(RGB(255, 255, 255));
		}
		else{
			unitBTN[i]->SetBackGroundColor(RGB(0, 255, 0));
			unitBTN[i]->SetFourColor(RGB(255, 255, 255));
		}
		unitBTN[i]->RedrawWindow();
		ctBTN[i] = new CCButton();
		ctBTN[i]->SubclassDlgItem(IDC_BTN_UNIT_CTMODE_01 + i, this);
		ctBTN[i]->SetWindowText(_T("CT"));
		ctBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
		ctBTN[i]->SetFourColor(RGB(0, 0, 0));
		ctBTN[i]->RedrawWindow();
		potBTN[i] = new CCButton();
		potBTN[i]->SubclassDlgItem(IDC_BTN_UNIT_REMOVE_POT_1 + i, this);
		potBTN[i]->SetWindowText(_T("UNSET"));
		potBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
		potBTN[i]->SetFourColor(RGB( 0, 0, 0));
		potBTN[i]->RedrawWindow();
	}
	flgMonitorUnitStatus = false;

	return TRUE;
}

void CPotCTRLServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CPotCTRLServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR CPotCTRLServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPotCTRLServerDlg::OnBnClickedBtnStartMonitor()
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CButton *btn = (CButton*)GetDlgItem(IDC_BTN_START_MONITOR);
	if (flgMonitorUnitStatus){
		flgMonitorUnitStatus = false;
		btn->SetWindowText(_T("Stopped"));
	}
	else{
		AfxBeginThread(ThreadMonitorUnitStatus, this);
		btn->SetWindowText(_T("Running..."));
	}
	updateDiglog();
}

void CPotCTRLServerDlg::OnBTN_UNIT(UINT nID)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	C_ExpCondition cond;

	CDlgUnit dlg;
	dlg.selUnitNumber = nID - IDC_BTN_UNIT_1;
	dlg.unitID = dlg.selUnitNumber + 1;
	if (dlg.DoModal() == IDOK){
		if (pApp->dbGetExpCondition(dlg.selUnitNumber + 1, &cond) < 1){
			unitBTN[dlg.selUnitNumber]->SetBackGroundColor(RGB(255, 0, 0));
			unitBTN[dlg.selUnitNumber]->SetFourColor(RGB(255, 255, 255));
		}
		else{
			unitBTN[dlg.selUnitNumber]->SetBackGroundColor(RGB(0, 255, 0));
			unitBTN[dlg.selUnitNumber]->SetFourColor(RGB(255, 255, 255));
		}
		unitBTN[dlg.selUnitNumber]->RedrawWindow();
	}
}
void CPotCTRLServerDlg::OnBTN_CTMODE(UINT nID)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	C_ExpCondition expCondition;
	C_ExpData data;
	int n = nID - IDC_BTN_UNIT_CTMODE_01;
	int ret = pApp->dbGetExpCondition(n + 1, &expCondition);
	//
	switch (pApp->unitDevice[n].status.CT_mode){
	case 0:
		pApp->unitDevice[n].SET_CTMode(1);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("01");
		pApp->dbSaveEXPDATA(data);
		break;
	case 1:
		pApp->unitDevice[n].SET_CTMode(2);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("02");
		pApp->dbSaveEXPDATA(data);
		break;
	case 2:
		break;
	case 3:
		pApp->unitDevice[n].SET_CTMode(0);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("00");
		pApp->dbSaveEXPDATA(data);
		break;
	}
}
void CPotCTRLServerDlg::OnBTN_POTMOVE(UINT nID)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	C_ExpCondition expCondition;
	C_ExpData data;
	int n = nID - IDC_BTN_UNIT_REMOVE_POT_1;
	int ret = pApp->dbGetExpCondition(n + 1, &expCondition);
	switch (pApp->unitDevice[n].status.CT_mode){
	case 0:
		pApp->unitDevice[n].SET_CTMode(3);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("03");
		pApp->dbSaveEXPDATA(data);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		pApp->unitDevice[n].SET_CTMode(0);
		data.SID = expCondition.SID;
		data.UNIT_ID = expCondition.UNIT_ID;
		data.TARGET = 2;
		data.VALUE = _T("00");
		pApp->dbSaveEXPDATA(data);
		break;
	}
}

void CPotCTRLServerDlg::updateDiglog()
{
	CString tmpTXT, outTXT;
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	CEdit *edit;
	for (int i = 0; i < NUM_UNITS; i++){
		if (pApp->unitDevice[i].UNIT_ID < 0)
			continue;
		outTXT.Format(_T("Unit %d\r\n"), pApp->unitDevice[i].UNIT_ID);
		tmpTXT.Format(_T("Time:%s\r\n"), pApp->unitDevice[i].statusHOST[0]);
		outTXT += tmpTXT;
		tmpTXT.Format(_T("Status 1:%s\r\n"), pApp->unitDevice[i].statusHOST[1]);
		outTXT += tmpTXT;
		tmpTXT.Format(_T("Status 2:%s\r\n"), pApp->unitDevice[i].statusHOST[2]);
		outTXT += tmpTXT;
		tmpTXT.Format(_T("Status 3:%s\r\n"), pApp->unitDevice[i].statusHOST[3]);
		outTXT += tmpTXT;
		tmpTXT.Format(_T("Version:%s"), pApp->unitDevice[i].statusHOST[4]);
		outTXT += tmpTXT;
		edit = (CEdit*)GetDlgItem(IDC_EDIT_UNIT_STATUS_1 + i);
		edit->SetWindowText(outTXT);
	}
	for (int i = 0; i < NUM_UNITS; i++){
		unitBTN[i]->EnableWindow(TRUE);
		switch (pApp->unitDevice[i].status.CT_mode){
		case 0:
			ctBTN[i]->SetWindowText(_T("Draining"));
			ctBTN[i]->SetFourColor(RGB(0, 0, 0));
			ctBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
			ctBTN[i]->EnableWindow(TRUE);
			potBTN[i]->SetWindowText(_T("UNSET Pots"));
			potBTN[i]->SetFourColor(RGB(0, 0, 0));
			potBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
			potBTN[i]->EnableWindow(TRUE);
			break;
		case 1:
			ctBTN[i]->SetWindowText(_T("Cancel draining"));
			ctBTN[i]->SetFourColor(RGB(255, 255, 255));
			ctBTN[i]->SetBackGroundColor(RGB(255, 0, 0));
			ctBTN[i]->EnableWindow(TRUE);
			potBTN[i]->SetWindowText(_T("-"));
			potBTN[i]->SetFourColor(RGB(0, 0, 0));
			potBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
			potBTN[i]->EnableWindow(FALSE);
			break;
		case 2:
			ctBTN[i]->SetWindowText(_T("-"));
			ctBTN[i]->SetFourColor(RGB(0, 0, 0));
			ctBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
			ctBTN[i]->EnableWindow(FALSE);
			potBTN[i]->SetWindowText(_T("UNSET Pots"));
			potBTN[i]->SetFourColor(RGB(255, 255, 255));
			potBTN[i]->SetBackGroundColor(RGB(0, 0, 255));
			potBTN[i]->EnableWindow(TRUE);
			break;
		case 3:
			ctBTN[i]->SetWindowText(_T("-"));
			ctBTN[i]->SetFourColor(RGB(0, 0, 0));
			ctBTN[i]->SetBackGroundColor(RGB(255, 255, 255));
			ctBTN[i]->EnableWindow(FALSE);
			potBTN[i]->SetWindowText(_T("UNSET Pots"));
			potBTN[i]->SetFourColor(RGB(255, 255, 255));
			ctBTN[i]->SetBackGroundColor(RGB(0, 255, 0));
			potBTN[i]->EnableWindow(TRUE);
			break;
		}
		ctBTN[i]->RedrawWindow();
		potBTN[i]->RedrawWindow();
	}

	Invalidate(false);
}

BOOL CPotCTRLServerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CPotCTRLServerDlg::OnBnClickedBtnExplist()
{
	CDlgExpList dlg;
	dlg.DoModal();
}
