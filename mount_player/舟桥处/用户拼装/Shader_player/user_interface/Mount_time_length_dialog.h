#pragma once
#include "afxcmn.h"


// Mount_time_length_dialog dialog
class Component;
class Part_set;

class Mount_time_length_dialog : public CDialog
{
	DECLARE_DYNAMIC(Mount_time_length_dialog)

public:
	Mount_time_length_dialog();   // standard constructor
	virtual ~Mount_time_length_dialog();

// Dialog Data
	enum { IDD = IDD_MOUNT_TIME_LENGTH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void set_title();
	int time_length_edit;
	int time_length_slider;

	afx_msg void OnNMCustomdrawTimeLengthSlider(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CSliderCtrl slider_controller;
protected:

public:
	afx_msg void OnBnClickedMiddlePoint();
	afx_msg void OnBnClickedTerminatation();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnComponentLocation();
	afx_msg void OnBnClickedComponentResetForMovement();
	afx_msg void OnBnClickedMovementResetForMovement();
};
