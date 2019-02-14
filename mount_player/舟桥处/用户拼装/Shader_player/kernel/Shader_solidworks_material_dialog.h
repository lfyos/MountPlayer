#pragma once


// Shader_solidworks_material_dialog dialog
#include "Shader_solidworks.h"

#include "afxwin.h"
#include "afxcmn.h"

class Shader_solidworks_material_dialog : public CDialog
{
	DECLARE_DYNAMIC(Shader_solidworks_material_dialog)
	Shader_solidworks *sw;
	int material_id,edit_id;
	
	float red,green,blue,ambient,diffuse,specular,shininess,emission;

	void get_parameter();
	void put_parameter();
	
public:
	Shader_solidworks_material_dialog(Shader_solidworks *my_sw);   // standard constructor
	virtual ~Shader_solidworks_material_dialog();

// Dialog Data
	enum { IDD = IDD_SHADER_SOLIDWORKS_MATERIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CComboBox material_control;
	CSliderCtrl slider_control;

	afx_msg void OnChange();
	afx_msg void OnCbnSelchangeMaterialIdCombo();

	afx_msg void OnEnSetfocusRedEdit();	
	afx_msg void OnEnSetfocusGreenEdit();
	afx_msg void OnEnSetfocusBlueEdit();	
	afx_msg void OnEnSetfocusAmbientEdit();	
	afx_msg void OnEnSetfocusDiffuseEdit();
	afx_msg void OnEnSetfocusSpecularEdit();
	afx_msg void OnEnSetfocusSpininessEdit();
	afx_msg void OnEnSetfocusEmissionEdit();

	afx_msg void OnNMCustomdrawSolidworksMaterialSlider(NMHDR *pNMHDR, LRESULT *pResult);
};
