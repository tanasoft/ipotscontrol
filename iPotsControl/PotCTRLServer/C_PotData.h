#pragma once
class C_PotData
{
public:
	C_PotData();
	~C_PotData();

public:
	int nPot;
	double soilTemp[6];
	double soilTempWT[3];
	double tankTemp;
	double wtLevel;
	double tank_pF[3];
	double soilWTRef;
	double soilWT[3];
	int totalWT;
	int light;
	double temp;
	double moist;
	double setWT;
	CString statusTXT[2];
	CString verTXT;
	void Clear();
};

