// Sound_player.cpp : implementation file
//
#include "stdafx.h"
#include "Sound_player.h"
#include "vfw.h"

#include <fstream>
using namespace std;

#pragma comment( lib,"Vfw32.lib")


static HWND m_audio=NULL;
static CString last_file_name=_T("");
static bool on_off_flag=true;

static Sound_player a;

bool Sound_player::sound_state()
{
	return on_off_flag;
}
void Sound_player::turn_sound_on_off()
{
	on_off_flag=on_off_flag?false:true;
	if (m_audio){
		MCIWndStop(m_audio);
		MCIWndDestroy(m_audio);
	}
	m_audio = NULL;
	last_file_name=_T("");
	return ;
}
bool Sound_player::play(CString file_name)
{
	if(on_off_flag){
		if(m_audio){
			if(last_file_name==file_name)
					return false;
			int cur_time=MCIWndGetPosition(m_audio),max_time=MCIWndGetLength(m_audio);
			if(cur_time<max_time)
				return true;
			MCIWndStop(m_audio);
			MCIWndDestroy(m_audio);
			m_audio = NULL;
		}
		last_file_name=file_name;
		
		CFileStatus  status;

		if(CFile::GetStatus(file_name,status))
			if(status.m_size>0)
				if(m_audio =MCIWndCreate(::AfxGetMainWnd()->m_hWnd,::AfxGetInstanceHandle(),WS_CHILD | WS_VISIBLE|MCIWNDF_NOMENU,file_name)){
					::ShowWindow(m_audio,SW_HIDE);
					MCIWndPlay(m_audio);
					MCIWndSetVolume(m_audio,1000);
				}
	}
	return false;
}

