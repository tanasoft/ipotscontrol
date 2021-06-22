#include "stdafx.h"
#include "C_PotData.h"


C_PotData::C_PotData()
{
	Clear();
}


C_PotData::~C_PotData()
{
}


void C_PotData::Clear()
{
	nPot = -1;
	for (int i = 0; i < 6; i++)
		soilTemp[i] = -999.9f;
	for (int i = 0; i < 3; i++)
		soilTempWT[i] = -999.9f;
	tankTemp = -999.9f;
	wtLevel = -9999.9f;
	for (int i = 0; i < 3; i++)
		tank_pF[i] = -1.0f;
	soilWTRef = -9999.9f;
	for (int i = 0; i < 3; i++)
		soilWT[i] = -9999.9f;
	totalWT = -1;
	light = -1;
	temp = -999.9f;
	moist = -1.0f;
	setWT = -9999.9f;
	statusTXT[0] = _T("-");
	statusTXT[1] = _T("-");
	verTXT = _T("-");
}
