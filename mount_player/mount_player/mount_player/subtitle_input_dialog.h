#pragma once


// subtitle_input_dialog dialog

class subtitle_input_dialog : public CDialog
{
	DECLARE_DYNAMIC(subtitle_input_dialog)

public:
	subtitle_input_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~subtitle_input_dialog();

// Dialog Data
	enum { IDD = IDD_SUBTITLE_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString subtitle;	
protected:
	virtual void OnOK();

public:

};
