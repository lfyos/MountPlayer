
// Shader_player.h : main header file for the Shader_player application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


#include "Movement\Movement_processor.h"

// CShader_playerApp:
// See Shader_player.cpp for the implementation of this class
//

class CShader_playerApp : public CWinAppEx
{
	void set_font_size(CFont *my_font,int my_size, LPCTSTR font_name);
public:
	CShader_playerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:	

	int mouse_move_id,mouse_wheel_id;
	bool mouse_left_key_down_flag,redraw_flag;
	CPoint mouse_left_key_down_point,last_point,mouse_move_point;
	double time_length;
	DWORD touch_time;
	
	
	Whole_system *ws;
	Movement_processor_with_virtual_mount  *mounter;

	virtual int ExitInstance();

	virtual BOOL OnIdle(LONG lCount);
};

extern CShader_playerApp theApp;
