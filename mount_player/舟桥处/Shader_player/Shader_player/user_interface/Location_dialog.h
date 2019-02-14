#pragma once
#include "afxcmn.h"


// Location_dialog dialog
class Component ;
class Part_set;

class Location_dialog : public CDialog
{
	DECLARE_DYNAMIC(Location_dialog)

	void set_min_and_max_value();
public:
	Component *comp;
	Part_set  *parts;
	Location component_location;

	Location_dialog(Component *my_comp,Part_set *my_parts,CWnd* pParent);   // standard constructor
	virtual ~Location_dialog();

// Dialog Data
	enum { IDD = IDD_LOCATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_x,m_y,m_z,r_x,r_y,r_z;
	double min_value,max_value,*pointer;

	afx_msg void OnEnChangeXEdit();
	afx_msg void OnEnChangeYEdit();
	afx_msg void OnEnChangeZEdit();
	afx_msg void OnEnChangeRxEdit();
	afx_msg void OnEnChangeRyEdit();
	afx_msg void OnEnChangeRzEdit();
private:
	void process(void);
public:
	afx_msg void OnEnSetfocusXEdit();
	afx_msg void OnEnSetfocusYEdit();
	afx_msg void OnEnSetfocusZEdit();
	afx_msg void OnEnSetfocusRxEdit();
	afx_msg void OnEnSetfocusRyEdit();
	afx_msg void OnEnSetfocusRzEdit();
	int slider;
	afx_msg void OnNMCustomdrawDataSlider(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl slider_controller;
	virtual BOOL OnInitDialog();
	void set_slider(void);

	static void reset();
};
