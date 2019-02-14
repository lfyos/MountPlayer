
// mount_player.h : mount_player 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// Cmount_playerApp:
// 有关此类的实现，请参阅 mount_player.cpp
//

class Cmount_playerApp : public CWinAppEx
{
public:
	Cmount_playerApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnManualView();
private:
	void set_font_size(CFont *my_font,int my_size,char *font_name);
};

extern Cmount_playerApp theApp;
