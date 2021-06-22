#include "stdafx.h"
#include "C_Unit.h"
#include "PotCTRLServer.h"

#include <iostream>
#include <fstream>

C_Unit::C_Unit()
{
	IPAddress = _T("0.0.0.0");
	UNIT_ID = 0;
	status.Clear();
	for (int i = 0; i < 6; i++)
		statusPot[i].Clear();
}

C_Unit::~C_Unit()
{
}

int C_Unit::SET_Time(void)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT;
	CTime now = CTime::GetCurrentTime();
	cmdTXT.Format(_T("TIS,000,0021,TIM,%s"), now.Format(_T("%Y/%m/%d %H:%M:%S")));
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[59] = 0x0d;
	cmdBuf[60] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_TIME, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_TIME, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT = _T("TCP Connection error");
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("SET_Time"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

int C_Unit::SET_Water(float _value[NUM_POTS], CString _ctrl[NUM_POTS])
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT, tmpTXT;
	cmdTXT.Format(_T("TIS,000,0044,WLC"));
	for (int i = 0; i < NUM_POTS; i++){
		tmpTXT.Format(_T(",%05.1f%s"), _value[i], _ctrl[i]);
		cmdTXT += tmpTXT;
	}
	cmdTXT += _T("=");
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[59] = 0x0d;
	cmdBuf[60] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_SETWATER, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_SETWATER, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT.Format(_T("TCP Connection error"));
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("SET_Water"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}
int C_Unit::UNSET_Water(void)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT, tmpTXT;
	cmdTXT.Format(_T("TIS,000,0044,WLC"));
	for (int i = 0; i < NUM_POTS; i++){
		tmpTXT.Format(_T(",000.0 "));
		cmdTXT += tmpTXT;
	}
	cmdTXT += _T("=");
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[59] = 0x0d;
	cmdBuf[60] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_SETWATER, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_SETWATER, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT.Format(_T("TCP Connection error"));
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("UNSET_Water"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

int C_Unit::SET_CTMode(int _mode)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT;
	cmdTXT.Format(_T("TIS,000,0005,CTM,0%d="), _mode);
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[20] = 0x0d;
	cmdBuf[21] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_CTMODE, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_CTMODE, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT = _T("TCP Connection error");
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("SET_CTMode"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

int C_Unit::SET_Emg(int _mode)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT;
	cmdTXT.Format(_T("TIS,000,0005,EMG,0%d="), _mode);
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[20] = 0x0d;
	cmdBuf[21] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_EMG, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_EMG, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT = _T("TCP Connection error");
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("SET_Emg"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

int C_Unit::SET_Reboot(int _wait)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT;
	cmdTXT.Format(_T("TIS,000,0006,RST,%04d"), _wait);
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[21] = 0x0d;
	cmdBuf[22] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_REBOOT, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_REBOOT, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT = _T("TCP Connection error");
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("SET_Reboot"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

int C_Unit::SET_Param(float _Kei[8], float _Clevel[8])
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT, tmpTXT;
	cmdTXT.Format(_T("TIS,000,0098,CNF"));
	for (int i = 0; i < 8; i++){
		tmpTXT.Format(_T(",%02.2f,%03.1f"), _Kei[i], _Clevel[i]);
		cmdTXT += tmpTXT;
	}
	cmdTXT += _T("=");
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[113] = 0x0d;
	cmdBuf[114] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_PARAM, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_PARAM, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT = _T("TCP Connection error");
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("SET_Param"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

int C_Unit::GET_Data(void)
{
	CPotCTRLServerApp *pApp = (CPotCTRLServerApp*)AfxGetApp();
	int ret = 0;
	for (int i = 0; i < 256; i++)
		cmdBuf[i] = 0;
	for (int i = 0; i < 2560; i++)
		recvBuf[i] = 0;
	for (int i = 0; i < 3; i++)
		retidchBuf[i] = 0;

	CString cmdTXT, retTXT;
	cmdTXT.Format(_T("TIS,000,0006,DAT,READ"));
	_tcscpy_s(cmdBuf, cmdTXT.GetLength() + 1, cmdTXT);
	cmdBuf[21] = 0x0d;
	cmdBuf[22] = 0x0a;
	retTXT = _T("");
	if (TCPConnect()){
		if (send(TCPSocket, cmdBuf, BUFSIZE_REQUEST_DATA_SEND, 0) < 0){
			ret = -1;
			retTXT.Format(_T("TCP Send Request error - %d"), WSAGetLastError());
		}
		else{
			recv(TCPSocket, recvBuf, BUFSIZE_REQUEST_DATA_REC, 0);
			retidchBuf[0] = recvBuf[4];
			retidchBuf[1] = recvBuf[5];
			retidchBuf[2] = recvBuf[6];
			UNIT_ID = atoi(retidchBuf);
			retTXT = recvBuf;
			ret = 0;
		}
		TCPDisConnect();
	}
	else{
		retTXT = _T("TCP Connection error");
		ret = -1;
	}
	pApp->dbAddLog(UNIT_ID, _T("GET_Data"), ret, retTXT);
	Sleep(WAITTIME);

	return ret;
}

BOOL C_Unit::TCPConnect(void)
{
	struct sockaddr_in dest;
	char adr[20];
	_tcscpy_s(adr, IPAddress.GetLength() + 1, IPAddress);
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);
	memset(&dest, 0, sizeof(dest));
	dest.sin_port = htons(TCPPORT);
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(adr);
	TCPSocket = socket(AF_INET, SOCK_STREAM, 0);
	int ret;
	if (connect(TCPSocket, (struct sockaddr *) &dest, sizeof(dest))){
		ret = WSAGetLastError();
		return false;
	}

	return true;
}

BOOL C_Unit::TCPDisConnect(void)
{
	closesocket(TCPSocket);
	WSACleanup();

	return true;
}

