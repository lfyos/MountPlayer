#pragma once


// Mount_step_dialog dialog

class Mount_step_dialog : public CDialog
{
	DECLARE_DYNAMIC(Mount_step_dialog)

public:
	CString window_title;
	Mount_step_dialog(CWnd* pParent,CString my_title);   // standard constructor
	virtual ~Mount_step_dialog();

// Dialog Data
	enum { IDD = IDD_MOUNT_STEP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int step;
	virtual BOOL OnInitDialog();
};
