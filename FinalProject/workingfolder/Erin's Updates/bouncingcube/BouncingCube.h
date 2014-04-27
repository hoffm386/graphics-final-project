
// BouncingCube.h : main header file for the BouncingCube application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols



// CBouncingCubeApp:
// See BouncingCube.cpp for the implementation of this class
//

class CBouncingCubeApp : public CWinAppEx
{
public:
	CBouncingCubeApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBouncingCubeApp theApp;
