#pragma once


// Mount_cache_information_dialog dialog

class Mount_cache_information_dialog : public CDialog
{
	DECLARE_DYNAMIC(Mount_cache_information_dialog)

public:
	Mount_cache_information_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~Mount_cache_information_dialog();

// Dialog Data
	enum { IDD = IDD_MOUNT_CACHE_INFORMATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString title;
	int start_main_step;
	int start_sub_step;
	int end_main_step;
	int end_sub_step;

protected:
	virtual void OnOK();
};
