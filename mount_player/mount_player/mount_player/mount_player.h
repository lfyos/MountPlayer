
// mount_player.h : mount_player Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// Cmount_playerApp:
// �йش����ʵ�֣������ mount_player.cpp
//

class Cmount_playerApp : public CWinAppEx
{
public:
	Cmount_playerApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
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
