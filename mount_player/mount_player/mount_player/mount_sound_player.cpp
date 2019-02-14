// mount_sound_player.cpp : implementation file
//
#include "stdafx.h"
#include "mount_player.h"
#include "mount_sound_player.h"
#include "vfw.h"
#include "resource.h"

#pragma comment( lib,"Vfw32.lib")


// mount_sound_player dialog

IMPLEMENT_DYNAMIC(mount_sound_player, CDialog)

mount_sound_player::mount_sound_player(CWnd* pParent /*=NULL*/)
	: CDialog(mount_sound_player::IDD, pParent)
{
	m_audio = NULL;
	last_file_name="";
}

mount_sound_player::~mount_sound_player()
{
	if (m_audio){
		MCIWndStop(m_audio);
		MCIWndDestroy(m_audio);
	}
}

void mount_sound_player::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(mount_sound_player, CDialog)
END_MESSAGE_MAP()


// mount_sound_player message handlers

BOOL mount_sound_player::play(CString file_name)
{
	if(m_audio){
		if(MCIWndGetPosition(m_audio)!=MCIWndGetLength(m_audio))
			return (last_file_name==file_name)?FALSE:TRUE;
		if(last_file_name==file_name)
			return FALSE;
		MCIWndStop(m_audio);
		MCIWndDestroy(m_audio);
	}
	if(m_audio = MCIWndCreate(m_hWnd,AfxGetInstanceHandle(),WS_CHILD | WS_VISIBLE|MCIWNDF_NOMENU,file_name)){
		MCIWndPlay(m_audio);
		MCIWndSetVolume(m_audio,1000);

		last_file_name=file_name;
	}
	return FALSE;
}
void mount_sound_player::sound_close(void)
{
	MCIWndStop(m_audio);
	if (m_audio)
			MCIWndDestroy(m_audio);
	
	m_audio = NULL;
	last_file_name="";
	return;
}
