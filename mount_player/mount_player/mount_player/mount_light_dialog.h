#pragma once
#include "afxwin.h"


// mount_light_dialog dialog

class mount_light_dialog : public CDialog
{
	DECLARE_DYNAMIC(mount_light_dialog)
private:
	double m_x;
	double m_y;
	double m_z;
	double m_red;
	double m_green;
	double m_blue;
	double m_ambient;
	double m_diffuse;
	double m_emission;
	double m_shininess;
	double m_specular;
	BOOL m_enable;
	BOOL m_absolute_or_relative_flag;
	CSliderCtrl m_slider;

//	bool modify_flag;
	int m_light_id;
	double *edit_pointer;

public:
	mount_light_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~mount_light_dialog();

// Dialog Data
	enum { IDD = IDD_MOUNT_LIGHT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	

	DECLARE_MESSAGE_MAP()

private:
	private:
	void set_display(int  light_id);
	void get_value(void);
	void set_edit_pointer(double * p);
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedSelect1Radio();
	afx_msg void OnBnClickedSelect2Radio();
	afx_msg void OnBnClickedSelect3Radio();
	afx_msg void OnBnClickedSelect4Radio();
	afx_msg void OnBnClickedSelect5Radio();
	afx_msg void OnBnClickedSelect6Radio();
	afx_msg void OnBnClickedSelect7Radio();
	afx_msg void OnBnClickedSelect8Radio();
	afx_msg void OnBnClickedSelectSelectedRadio();
	afx_msg void OnBnClickedSelectBackgroundRadio();
	afx_msg void OnEnSetfocusRedEdit();
	afx_msg void OnEnChangeRedEdit();
	afx_msg void OnEnSetfocusGreenEdit();
	afx_msg void OnEnChangeGreenEdit();
	afx_msg void OnEnSetfocusBlueEdit();
	afx_msg void OnEnChangeBlueEdit();
	afx_msg void OnEnSetfocusAmbientEdit();
	afx_msg void OnEnChangeAmbientEdit();
	afx_msg void OnEnSetfocusDiffuseEdit();
	afx_msg void OnEnChangeDiffuseEdit();
	afx_msg void OnEnSetfocusEmissionEdit();
	afx_msg void OnEnChangeEmissionEdit();
	afx_msg void OnEnSetfocusShininessEdit();
	afx_msg void OnEnChangeShininessEdit();
	afx_msg void OnEnSetfocusSpecularEdit();
	afx_msg void OnEnChangeSpecularEdit();
	afx_msg void OnEnChangeXEdit();
	afx_msg void OnEnChangeYEdit();
	afx_msg void OnEnChangeZEdit();
	afx_msg void OnBnClickedEnableCheck();
	afx_msg void OnNMCustomdrawLightSlider(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnClose();
	afx_msg void OnBnClickedAbsoluteRelativeCheck();
	afx_msg void OnBnClickedColorInputButton();
	afx_msg void OnBnClickedSelectEnvironmentRadio();
	afx_msg void OnBnClickedSeparateSpecularCheck();
	CButton separate_speclar_check;
};
