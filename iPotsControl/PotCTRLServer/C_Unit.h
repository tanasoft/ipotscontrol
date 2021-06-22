#pragma once

#include <stdio.h>
#include <locale.h>
#include "C_PotData.h"

class unitStatus{
public:
	int setTime;
	int ERR_file;
	int ERR_node;
	int CT_mode;
	int writeProgram;
	int condition;
	int wtTankIN[4];
	int wtTankOUT[4];
	unitStatus::unitStatus(){};
	unitStatus::~unitStatus(){};
	void unitStatus::Clear(){
		setTime = 0;
		ERR_file = 0;
		ERR_node = 0;
		CT_mode = 0;
		writeProgram = 0;
		condition = 0;
		for (int i = 0; i < 4; i++){
			wtTankIN[i] = 0;
			wtTankOUT[i] = 0;
		}
	};
	void unitStatus::importTXT(CString _TXT1, CString _TXT2){
		int ret[3];
		setTime = _tstoi(_TXT1.Mid(0, 1));
		ERR_file = _tstoi(_TXT1.Mid(1, 1));
		ERR_node = _tstoi(_TXT1.Mid(3, 1));
		if (_tstoi(_TXT1.Mid(8, 1)) == 1){
			CT_mode = 1;
		}
		else if (_tstoi(_TXT1.Mid(9, 1)) == 1){
			CT_mode = 2;
		}
		else if (_tstoi(_TXT1.Mid(10, 1)) == 1){
			CT_mode = 3;
		}
		else if(_tstoi(_TXT1.Mid(11, 1)) == 1){
			CT_mode = 0;
		}
		writeProgram = _tstoi(_TXT1.Mid(12, 1));
		if (_tstoi(_TXT1.Mid(15, 1)) == 1){
			condition = 2;
		}
		else if (_tstoi(_TXT1.Mid(14, 1)) == 1){
			condition = 1;
		}
		else if (_tstoi(_TXT1.Mid(13, 1)) == 1){
			condition = 0;
		}
		wtTankIN[0] = _tstoi(_TXT2.Mid(0, 1));
		wtTankIN[1] = _tstoi(_TXT2.Mid(1, 1));
		if (_tstoi(_TXT2.Mid(2, 1)) == 1){
			wtTankIN[2] = 2;
		}
		else if (_tstoi(_TXT2.Mid(3, 1)) == 1){
			wtTankIN[2] = 1;
		}
		else{
			wtTankIN[2] = 0;
		}
		if (_tstoi(_TXT2.Mid(5, 1)) == 1){
			wtTankIN[3] = 1;
		}
		else if (_tstoi(_TXT2.Mid(6, 1)) == 1){
			wtTankIN[3] = 2;
		}
		wtTankOUT[0] = _tstoi(_TXT2.Mid(8, 1));
		wtTankOUT[1] = _tstoi(_TXT2.Mid(9, 1));
		if (_tstoi(_TXT2.Mid(10, 1)) == 1){
			wtTankOUT[2] = 2;
		}
		else if (_tstoi(_TXT2.Mid(11, 1)) == 1){
			wtTankOUT[2] = 1;
		}
		else{
			wtTankOUT[2] = 0;
		}
		if (_tstoi(_TXT2.Mid(13, 1)) == 1){
			wtTankOUT[3] = 1;
		}
		else if (_tstoi(_TXT2.Mid(14, 1)) == 1){
			wtTankOUT[3] = 2;
		}
	};
};

// ’ÊMÝ’è
#include <WinSock2.h>
#define TIMEOUT 60
#define TCPPORT 54321
#define UDPPORT 54320

#define WAITTIME 2000

#define BUFSIZE_TIME 38
#define BUFSIZE_PARAM 115
#define BUFSIZE_REQUEST_DATA_SEND 23
#define BUFSIZE_REQUEST_DATA_REC 2106
#define BUFSIZE_SETWATER 61
#define BUFSIZE_EMG 22
#define BUFSIZE_CTMODE 22
#define BUFSIZE_NODE 23
#define BUFSIZE_ALV 58
#define BUFSIZE_REBOOT 23

class C_Unit
{
public:
	C_Unit();
	~C_Unit();

	char cmdBuf[256];
	char recvBuf[2560];
	char retidchBuf[3];
	int UNIT_ID;
	CString IPAddress;
	CString statusHOST[5];
	unitStatus status;
	C_PotData statusPot[6];
	int SET_Time(void);
	int SET_Water(float _value[6], CString _ctrl[6]);
	int UNSET_Water(void);
	int SET_CTMode(int _mode);
	int SET_Emg(int _mode);
	int SET_Reboot(int _wait);
	int SET_Param(float _Kei[8], float _Clevel[8]);
	int GET_Data(void);
	int SET_ProgramUpdate(void);
private:
	SOCKET TCPSocket;
	BOOL TCPConnect(void);
	BOOL TCPDisConnect(void);
};

