#pragma once
#include "afxwin.h"


// Mount_information_dialog dialog

class Mount_information_dialog : public CDialog
{
	DECLARE_DYNAMIC(Mount_information_dialog)

public:
	Mount_information_dialog(int *my_main_step,int *my_sub_step,CString *my_sound_file,CString *my_title,CWnd* pParent);   // standard constructor
	virtual ~Mount_information_dialog();

// Dialog Data
	enum { IDD = IDD_MOUNT_INFORMATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int *main_step;
	int *sub_step;
	CString *sound_file_name;
	CString *title;
	CEdit sound_edit;
	CEdit title_edit;
	BOOL start_state_flag,end_state_flag;
	afx_msg void OnBnClickedStartCheck();
	afx_msg void OnBnClickedEndCheck();
	CButton start_state_button;
	CButton end_state_button;
	virtual BOOL OnInitDialog();
};

