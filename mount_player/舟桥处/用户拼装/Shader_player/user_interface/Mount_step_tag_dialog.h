#pragma once


// Mount_step_tag_dialog dialog

class Mount_step_tag_dialog : public CDialog
{
	DECLARE_DYNAMIC(Mount_step_tag_dialog)

public:
	Mount_step_tag_dialog(CString my_tag_string,int my_x0,int my_y0,CWnd* pParent = NULL);   // standard constructor
	virtual ~Mount_step_tag_dialog();

// Dialog Data
	enum { IDD = IDD_STEP_TAG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString tag_string;
	int x0,y0;

	virtual BOOL OnInitDialog();
};
