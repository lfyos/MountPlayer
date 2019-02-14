#pragma once
#include "afxwin.h"


// mount_location_dialog dialog

class mount_location_dialog : public CDialog
{
	DECLARE_DYNAMIC(mount_location_dialog)
public:
	mount_location_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~mount_location_dialog();

// Dialog Data
	enum { IDD = IDD_LOCATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	double dx;
	double dy;
	double dz;
	double rx;
	double ry;
	double rz;
	double time_length;
	int main_step;
	int sub_step;
	BOOL save_to_cache_flag,last_point_flag;
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedLastPoint();		
};
