#pragma once


// input_main_sub_step_dialog dialog

class input_main_sub_step_dialog : public CDialog
{
	DECLARE_DYNAMIC(input_main_sub_step_dialog)

public:
	input_main_sub_step_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~input_main_sub_step_dialog();

// Dialog Data
	enum { IDD = IDD_MAIN_SUB_STEP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int main_step_id,sub_step_id;
};
