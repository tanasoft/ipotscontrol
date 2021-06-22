
#include "stdafx.h"
#include "PotCTRLServer.h"
#include "PotCTRLServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPotCTRLServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

#include "sqlite3.h"
typedef int(__cdecl * pOpen)(char *, sqlite3 **);
typedef int(__cdecl * pExec)(sqlite3 *, const char *, sqlite3_callback, void *, char **);
typedef int(__cdecl * pGetTable)(sqlite3 *, const char *, char ***, int *, int *, char **);
typedef int(__cdecl * pFreeTable)(char **);
typedef int(__cdecl * pClose)(sqlite3 *);

CPotCTRLServerApp::CPotCTRLServerApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_mutexMultiExec = new CMutex(FALSE, _T("EnvPotControlMutex"));
	m_lockMultiExec = new CSingleLock(m_mutexMultiExec);
	expList = NULL;
	numExpList = 0;
	personTXT = NULL;
	nPersonTXT = 0;

	dbFilename = _T("");
	dbFn = NULL;
}
CPotCTRLServerApp::~CPotCTRLServerApp()
{
	if (m_lockMultiExec->IsLocked())
		m_lockMultiExec->Unlock();
	delete m_lockMultiExec;
	delete m_mutexMultiExec;
}
CPotCTRLServerApp theApp;
BOOL CPotCTRLServerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	if (!m_lockMultiExec->Lock(IGNORE)){
		MessageBox(NULL, _T("This program is running background."), _T("WARNING"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	SetRegistryKey(_T("TanaSoft2019"));
	TCHAR szPath[_MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	dbFilename.Format("%s\\rootomics.db", szPath);
	CPotCTRLServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "WARNING: Failed making dialog.\n");
		TRACE(traceAppMsg, 0, "WARNING: #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS\n");
	}
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}

int CPotCTRLServerApp::dbExecSQL(CString _sqlTXT)
{
	HINSTANCE hDll = LoadLibrary(_T("sqlite3"));
	if (hDll == NULL) {
		return -1;
	}
	pOpen sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	pExec sqlite3_exec = (pExec)GetProcAddress(hDll, "sqlite3_exec");
	pClose sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	sqlite3 *db;
	char *err;
	char **retSQL;
	int row, col;
	char *fn = new char[dbFilename.GetLength() + 1];
	_tcscpy_s(fn, dbFilename.GetLength() + 1, dbFilename);
	sqlite3_open(fn, &db);
	char *cmd = new char[_sqlTXT.GetLength() + 1];
	_tcscpy_s(cmd, _sqlTXT.GetLength() + 1, _sqlTXT);
	sqlite3_exec(db, cmd, NULL, NULL, &err);
	delete[] cmd;
	sqlite3_close(db);
	FreeLibrary(hDll);
	return 0;
}

int CPotCTRLServerApp::dbLoadExpList(int _not_selected_unit_only)
{
	HINSTANCE hDll = LoadLibrary(_T("sqlite3"));
	if (hDll == NULL) {
		return -1;
	}
	pOpen sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	pGetTable sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	pFreeTable sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	pClose sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	sqlite3 *db;
	char *err;
	char **retSQL;
	int row, col;
	CString tmpTXT;
	int m, n;
	char *fn = new char[dbFilename.GetLength() + 1];
	_tcscpy_s(fn, dbFilename.GetLength() + 1, dbFilename);
	sqlite3_open(fn, &db);
	if (_not_selected_unit_only == 1){
		sqlite3_get_table(db, "select * from EXPLIST where enable > 0 and UNIT < 0 order by SID;", &retSQL, &row, &col, &err);
	}
	else{
		sqlite3_get_table(db, "select * from EXPLIST where enable > 0 order by SID;", &retSQL, &row, &col, &err);
	}
	if (row > 0){
		delete[] expList;
		expList = new C_ExpCondition[row];
		for (int i = 1; i <= row; i++){
			expList[i - 1].SID = atoi(retSQL[(i*col)]);
			expList[i - 1].enable = atoi(retSQL[(i*col + 1)]);
			expList[i - 1].DATE = (CString)retSQL[(i*col + 2)];
			expList[i - 1].NAME = (CString)retSQL[(i*col + 3)];
			expList[i - 1].PERSON = (CString)retSQL[i*col + 4];
			expList[i - 1].MEMO = (CString)retSQL[i*col + 5];
			expList[i - 1].UNIT_ID = atoi(retSQL[i*col + 6]);
			tmpTXT = (CString)retSQL[i*col + 7];
			m = 0;
			n = tmpTXT.Find(_T(";"), m);
			for (int j = 0; j < NUM_POTS; j++){
				if (n > 0)
					expList[i - 1].WATER[j] = _tstof(tmpTXT.Mid(m, n));
				else
					expList[i - 1].WATER[j] = -1.0;
				m = n+1;
				n = tmpTXT.Find(_T(";"), m);
			}
		}
		numExpList = row;
	}
	else{
		numExpList = 0;
	}
	sqlite3_free_table(retSQL);
	sqlite3_close(db);
	FreeLibrary(hDll);
	return numExpList;
}

int CPotCTRLServerApp::_dbAddExp(C_ExpCondition _data)
{
	CString sqlTXT, waterTXT, tmpTXT;
	waterTXT = _T("");
	for (int i = 0; i < NUM_POTS; i++){
		tmpTXT.Format(_T("%.1lf;"), _data.WATER[i]);
		waterTXT += tmpTXT;
	}
	// SQL•¶
	sqlTXT.Format(_T("insert into EXPLIST values(%d,%d,'%s','%s','%s','%s',%d,'%s');"), _data.SID, _data.enable, _data.DATE, _data.NAME, _data.PERSON, _data.MEMO, _data.UNIT_ID, waterTXT);
	dbExecSQL(sqlTXT);

	return 0;
}
int CPotCTRLServerApp::_dbGetMaxSID(void)
{
	HINSTANCE hDll = LoadLibrary(_T("sqlite3"));
	if (hDll == NULL) {
		return -1;
	}
	pOpen sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	pGetTable sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	pFreeTable sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	pClose sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	int retSID = 0;
	sqlite3 *db;
	char *err;
	char **retSQL;
	int row, col;
	char *fn = new char[dbFilename.GetLength() + 1];
	_tcscpy_s(fn, dbFilename.GetLength() + 1, dbFilename);
	sqlite3_open(fn, &db);
	sqlite3_get_table(db, "select count(*) from EXPLIST;", &retSQL, &row, &col, &err);
	int n;
	if (retSQL == NULL) {
		retSID = -1;
	}
	else {
		n = atoi(retSQL[1]);
		if (n < 1) {
			retSID = -1;
		}
		else {
			sqlite3_get_table(db, "select max(SID) from EXPLIST;", &retSQL, &row, &col, &err);
			retSID = atoi(retSQL[1]);
		}
	}
	sqlite3_free_table(retSQL);	
	sqlite3_close(db);
	FreeLibrary(hDll);

	return retSID;
}

int CPotCTRLServerApp::dbExpListNew(C_ExpCondition _data)
{
	_data.SID = _dbGetMaxSID() + 1;
	_data.enable = 1;
	CTime now = CTime::GetCurrentTime();
	_data.DATE = now.Format(_T("%Y/%m/%d %H:%M:%S"));
	_data.UNIT_ID = -1;
	_dbAddExp(_data);
	return 0;
}
int CPotCTRLServerApp::dbExpListModify(C_ExpCondition _data)
{
	CString sqlTXT;
	sqlTXT.Format(_T("update EXPLIST set enable = -9 where SID=%d;"), _data.SID);
	dbExecSQL(sqlTXT);
	CTime now = CTime::GetCurrentTime();
	_data.DATE = now.Format(_T("%Y/%m/%d %H:%M:%S"));
	_dbAddExp(_data);
	return 0;
}
int CPotCTRLServerApp::dbSaveExp(C_ExpCondition _data)
{
	CString sqlTXT;
	if (_data.SID < 1){
		_data.SID = _dbGetMaxSID() + 1;
		_data.enable = 1;
		CTime now = CTime::GetCurrentTime();
		_data.DATE = now.Format(_T("%Y/%m/%d %H:%M:%S"));
		_dbAddExp(_data);
	}
	else{
		sqlTXT.Format(_T("update EXPLIST set enable = -9 where SID=%d;"), _data.SID);
		dbExecSQL(sqlTXT);
		CTime now = CTime::GetCurrentTime();
		_data.DATE = now.Format(_T("%Y/%m/%d %H:%M:%S"));
		_dbAddExp(_data);
	}
	return 0;
}
int CPotCTRLServerApp::dbSaveEXPDATA(C_ExpData _data)
{
	CString sqlTXT;
	CTime now = CTime::GetCurrentTime();
	sqlTXT.Format(_T("insert into EXPDATA values(%d,'%s',%d,%d,'%s');"), _data.SID, now.Format(_T("%Y/%m/%d %H:%M:%S")), _data.UNIT_ID, _data.TARGET, _data.VALUE);
	dbExecSQL(sqlTXT);
	return 0;
}

int CPotCTRLServerApp::dbAddLog(int _unit, CString _command, int _type, CString _logTXT)
{
	CString sqlTXT;
	CTime now = CTime::GetCurrentTime();
	sqlTXT.Format(_T("insert into log values(%d,'%s', '%s', %d,'%s');"), _unit, now.Format(_T("%Y/%m/%d %H:%M:%S")), _command, _type, _logTXT);
	dbExecSQL(sqlTXT);
	return 0;
}

int CPotCTRLServerApp::dbGetPersonList(void)
{
	HINSTANCE hDll = LoadLibrary(_T("sqlite3"));
	if (hDll == NULL) {
		return -1;
	}
	pOpen sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	pGetTable sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	pFreeTable sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	pClose sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	sqlite3 *db;
	char *err;
	char **retSQL;
	int row, col;
	CString tmpTXT;
	int m, n;
	char *fn = new char[dbFilename.GetLength() + 1];
	_tcscpy_s(fn, dbFilename.GetLength() + 1, dbFilename);
	sqlite3_open(fn, &db);
	sqlite3_get_table(db, "select distinct PERSON from EXPLIST;", &retSQL, &row, &col, &err);
	if (row > 0){
		delete[] personTXT;
		personTXT = new CString[row];
		for (int i = 1; i <= row; i++){
			personTXT[i - 1] = (CString)retSQL[(i*col)];
		}
		nPersonTXT = row;
	}
	else{
		nPersonTXT = 0;
	}
	sqlite3_free_table(retSQL);
	sqlite3_close(db);
	FreeLibrary(hDll);
	return 0;
}
int CPotCTRLServerApp::dbGetExpCondition(int _UNIT_ID, C_ExpCondition *_COND)
{
	if (_UNIT_ID < 0)
		return 0;
	int ret = 0;
	HINSTANCE hDll = LoadLibrary(_T("sqlite3"));
	if (hDll == NULL) {
		return -1;
	}
	pOpen sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	pGetTable sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	pFreeTable sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	pClose sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	sqlite3 *db;
	char *err;
	char **retSQL;
	int row, col;
	CString sqlTXT, tmpTXT;
	int m, n;
	char *fn = new char[dbFilename.GetLength() + 1];
	_tcscpy_s(fn, dbFilename.GetLength() + 1, dbFilename);
	sqlite3_open(fn, &db);
	sqlTXT.Format(_T("select * from EXPLIST where enable > 0 and UNIT = %d order by SID;"), _UNIT_ID);
	CStringA sqlChar(sqlTXT);
	sqlite3_get_table(db, sqlChar, &retSQL, &row, &col, &err);
	if (row > 0){
		_COND->SID = atoi(retSQL[col]);
		_COND->enable = atoi(retSQL[col + 1]);
		_COND->DATE = (CString)retSQL[col + 2];
		_COND->NAME = (CString)retSQL[col + 3];
		_COND->PERSON = (CString)retSQL[col + 4];
		_COND->MEMO = (CString)retSQL[col + 5];
		_COND->UNIT_ID = atoi(retSQL[col + 6]);
		tmpTXT = (CString)retSQL[col + 8];
		m = 0;
		n = tmpTXT.Find(_T(";"), m);
		for (int j = 0; j < NUM_POTS; j++){
			if (n > 0)
				_COND->WATER[j] = _tstof(tmpTXT.Mid(m, n));
			else
				_COND->WATER[j] = -1.0;
			m = n + 1;
			n = tmpTXT.Find(_T(";"), m);
		}
		ret = 1;
	}
	else{
		ret = 0;
	}
	sqlite3_free_table(retSQL);
	sqlite3_close(db);
	FreeLibrary(hDll);
	return ret;
}
int CPotCTRLServerApp::dbGetExpConditionSID(int _SID, C_ExpCondition *_COND)
{
	int ret = 0;
	HINSTANCE hDll = LoadLibrary(_T("sqlite3"));
	if (hDll == NULL) {
		return -1;
	}
	pOpen sqlite3_open = (pOpen)GetProcAddress(hDll, "sqlite3_open");
	pGetTable sqlite3_get_table = (pGetTable)GetProcAddress(hDll, "sqlite3_get_table");
	pFreeTable sqlite3_free_table = (pFreeTable)GetProcAddress(hDll, "sqlite3_free_table");
	pClose sqlite3_close = (pClose)GetProcAddress(hDll, "sqlite3_close");
	sqlite3 *db;
	char *err;
	char **retSQL;
	int row, col;
	CString sqlTXT, tmpTXT;
	int m, n;
	char *fn = new char[dbFilename.GetLength() + 1];
	_tcscpy_s(fn, dbFilename.GetLength() + 1, dbFilename);
	sqlite3_open(fn, &db);
	sqlTXT.Format(_T("select * from EXPLIST where enable > 0 and SID = %d order by DATE desc;"), _SID);
	CString sqlChar(sqlTXT);
	sqlite3_get_table(db, sqlChar, &retSQL, &row, &col, &err);
	if (row > 0){
		_COND->SID = atoi(retSQL[col]);
		_COND->enable = atoi(retSQL[col + 1]);
		_COND->DATE = (CString)retSQL[col + 2];
		_COND->NAME = (CString)retSQL[col + 3];
		_COND->PERSON = (CString)retSQL[col + 4];
		_COND->MEMO = (CString)retSQL[col + 5];
		_COND->UNIT_ID = atoi(retSQL[col + 6]);
		tmpTXT = (CString)retSQL[col + 7];
		m = 0;
		n = tmpTXT.Find(_T(";"), m);
		for (int j = 0; j < NUM_POTS; j++){
			if (n > 0)
				_COND->WATER[j] = _tstof(tmpTXT.Mid(m, n));
			else
				_COND->WATER[j] = -1.0;
			m = n + 1;
			n = tmpTXT.Find(_T(";"), m);
		}
		ret = 1;
	}
	else{
		ret = 0;
	}
	sqlite3_free_table(retSQL);
	sqlite3_close(db);
	FreeLibrary(hDll);
	return ret;
}