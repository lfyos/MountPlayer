#pragma once


// movement_label_dialog dialog

class movement_label_dialog : public CDialog
{
	DECLARE_DYNAMIC(movement_label_dialog)

public:
	movement_label_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~movement_label_dialog();

// Dialog Data
	enum { IDD = IDD_MOVEMENT_LABEL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString label_name;
protected:
	virtual void OnOK();
};
