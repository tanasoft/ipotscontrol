
#pragma once

class CCButton : public CButton{
private:
	COLORREF cTextColor;
	COLORREF cBkColor;
	COLORREF cDisabledColor;
public:
	CCButton(){
		cTextColor = RGB(0, 0, 0);
		cBkColor = RGB(200, 200, 200);
		cDisabledColor = RGB(255, 255, 255);
	}
	void SetFourColor(const COLORREF color){ cTextColor = color; }
	void SetBackGroundColor(const COLORREF color){ cBkColor = color; }
	void SetDisabledColor(const COLORREF color){ cDisabledColor = color; }
	COLORREF GetTextColor(){ return cTextColor; }
	COLORREF GetBackGroundColor(){ return cBkColor; }
	COLORREF GetDisabledColor(){ return cDisabledColor; }
protected:
	void DrawItem(LPDRAWITEMSTRUCT lpDIS){
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CString strCaption;

		UINT nState = lpDIS->itemState;
		CRect focusRect, btnRect;
		focusRect.CopyRect(&lpDIS->rcItem);
		btnRect.CopyRect(&lpDIS->rcItem);

		focusRect.left += 4;
		focusRect.right -= 4;
		focusRect.top += 4;
		focusRect.bottom -= 4;

		GetWindowText(strCaption);

		DrawBkGround(pDC, btnRect, GetBackGroundColor());
		DrawCaption(pDC, btnRect, GetTextColor(), strCaption);
		pDC->DrawEdge(btnRect, EDGE_RAISED, BF_RECT);

		if (nState & ODS_FOCUS){
			if (nState & ODS_SELECTED){
				pDC->DrawEdge(btnRect, EDGE_SUNKEN, BF_RECT);
			}
			pDC->DrawFocusRect(focusRect);
		}
		else if (nState & ODS_DISABLED){
			DrawCaption(pDC, btnRect, GetDisabledColor(), strCaption);
		}
	}
	void DrawBkGround(CDC *pDC, CRect rect, const COLORREF color){
		CBrush brush(color);
		pDC->FillRect(rect, &brush);
	}
	void DrawCaption(CDC *pDC, CRect rect, const COLORREF color, const CString& str){
		COLORREF oldcolor = pDC->SetTextColor(color);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->SetTextColor(oldcolor);
	}
};
#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"
#include "C_Unit.h"

#define NUM_UNITS 30
#define NUM_POTS 6

class C_ExpCondition{
public:
	int SID;
	int enable;
	CString DATE;
	CString NAME;
	CString PERSON;
	CString MEMO;
	int UNIT_ID;
	double WATER[NUM_POTS];
	C_ExpCondition::C_ExpCondition(){};
	C_ExpCondition::~C_ExpCondition(){};
	void C_ExpCondition::Clear(){
		SID = 0;
		enable = 1;
		DATE = _T("2000/01/01 12:00:00");
		NAME = _T("no name");
		PERSON = _T("Rootomics");
		MEMO = _T("no text");
		UNIT_ID = -1;
		for (int i = 0; i < NUM_POTS; i++){
			WATER[i] = -1.0;
		}
	};
};

class C_ExpData{
public:
	int SID;
	int UNIT_ID;
	int TARGET;
	CString VALUE;

	C_ExpData::C_ExpData(){};
	C_ExpData::~C_ExpData(){};
};

class CPotCTRLServerApp : public CWinApp
{
public:
	CPotCTRLServerApp();
	~CPotCTRLServerApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

private:
	CMutex* m_mutexMultiExec;
	CSingleLock* m_lockMultiExec;
	CString dbFilename;
	char* dbFn;

public:
	int dbExecSQL(CString _sqlTXT);
	int _dbAddExp(C_ExpCondition _data);
	int _dbGetMaxSID(void);
	int dbExpListNew(C_ExpCondition _data);
	int dbExpListModify(C_ExpCondition _data);
	int dbGetPersonList(void);
	int dbGetExpCondition(int _UNIT_ID, C_ExpCondition *_COND);
	int dbGetExpConditionSID(int _SID, C_ExpCondition *_COND);
	int dbSaveExp(C_ExpCondition _data);
	int dbSaveEXPDATA(C_ExpData _data);
	int dbAddLog(int _unit, CString _command, int _type, CString _logTXT);
	C_Unit unitDevice[NUM_UNITS];
	C_ExpCondition *expList;
	int numExpList;
	int dbLoadExpList(int _not_selected_unit_only);
	CString *personTXT;
	int nPersonTXT;
};

extern CPotCTRLServerApp theApp;