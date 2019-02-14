#pragma once


// mount_sound_player dialog

class mount_sound_player : public CDialog
{
	DECLARE_DYNAMIC(mount_sound_player)

public:
	mount_sound_player(CWnd* pParent = NULL);   // standard constructor
	virtual ~mount_sound_player();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	HWND m_audio;
	CString last_file_name;
public:
	BOOL play(CString file_name);
	void sound_close(void);
};
