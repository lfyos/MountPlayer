#pragma once
#include "afxwin.h"
#include "const.h"

// virtual_mount_dialog dialog

class virtual_mount_dialog : public CDialog
{
	DECLARE_DYNAMIC(virtual_mount_dialog)

public:
	virtual_mount_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~virtual_mount_dialog();

// Dialog Data
	enum { IDD = IDD_VIRTUAL_MOUNT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_list;
private:
	int select_id[MAX_MOVEMENT_STEP];
public:
	afx_msg void OnLbnSelchangePartList();
	afx_msg void OnLbnDblclkPartList();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	int manual_mount_state_flag;//0:NOT manual mount,1:selecting part, 2:doing manual mount
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
