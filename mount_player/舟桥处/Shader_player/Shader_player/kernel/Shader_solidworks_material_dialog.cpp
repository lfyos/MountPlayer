// kernel\Shader_solidworks_material_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\Shader_player.h"
#include "Shader_solidworks_material_dialog.h"


// Shader_solidworks_material_dialog dialog

IMPLEMENT_DYNAMIC(Shader_solidworks_material_dialog, CDialog)

Shader_solidworks_material_dialog::Shader_solidworks_material_dialog(Shader_solidworks *my_sw)
	: CDialog(Shader_solidworks_material_dialog::IDD, ::AfxGetMainWnd())
{
	sw=my_sw;
	
	material_id=0;
	edit_id=0;

	get_parameter();
}

Shader_solidworks_material_dialog::~Shader_solidworks_material_dialog()
{
	
}

void Shader_solidworks_material_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RED_EDIT, red);
	DDV_MinMaxFloat(pDX, red, 0.0, 1.0);
	DDX_Text(pDX, IDC_GREEN_EDIT, green);
	DDV_MinMaxFloat(pDX, green, 0.0, 1.0);
	DDX_Text(pDX, IDC_BLUE_EDIT, blue);
	DDV_MinMaxFloat(pDX, blue, 0.0, 1.0);
	DDX_Text(pDX, IDC_AMBIENT_EDIT, ambient);
	DDV_MinMaxFloat(pDX, ambient, 0.0, 1.0);
	DDX_Text(pDX, IDC_DIFFUSE_EDIT, diffuse);
	DDV_MinMaxFloat(pDX, diffuse, 0.0, 1.0);
	DDX_Text(pDX, IDC_SPECULAR_EDIT, specular);
	DDV_MinMaxFloat(pDX, specular, 0.0, 1.0);
	DDX_Text(pDX, IDC_SPININESS_EDIT, shininess);
	DDX_Text(pDX, IDC_EMISSION_EDIT, emission);
	DDV_MinMaxFloat(pDX, emission, 0.0, 1.0);
	DDX_Control(pDX, IDC_MATERIAL_ID_COMBO, material_control);
	DDX_Control(pDX, IDC_SOLIDWORKS_MATERIAL_SLIDER, slider_control);
}


BEGIN_MESSAGE_MAP(Shader_solidworks_material_dialog, CDialog)
	ON_EN_SETFOCUS(IDC_RED_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusRedEdit)
	ON_EN_CHANGE(IDC_RED_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_CBN_SELCHANGE(IDC_MATERIAL_ID_COMBO, &Shader_solidworks_material_dialog::OnCbnSelchangeMaterialIdCombo)
	ON_EN_SETFOCUS(IDC_GREEN_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusGreenEdit)
	ON_EN_CHANGE(IDC_GREEN_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_EN_SETFOCUS(IDC_BLUE_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusBlueEdit)
	ON_EN_CHANGE(IDC_BLUE_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_EN_SETFOCUS(IDC_AMBIENT_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusAmbientEdit)
	ON_EN_CHANGE(IDC_AMBIENT_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_EN_SETFOCUS(IDC_DIFFUSE_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusDiffuseEdit)
	ON_EN_CHANGE(IDC_DIFFUSE_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_EN_SETFOCUS(IDC_SPECULAR_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusSpecularEdit)
	ON_EN_CHANGE(IDC_SPECULAR_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_EN_SETFOCUS(IDC_SPININESS_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusSpininessEdit)
	ON_EN_CHANGE(IDC_SPININESS_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_EN_SETFOCUS(IDC_EMISSION_EDIT, &Shader_solidworks_material_dialog::OnEnSetfocusEmissionEdit)
	ON_EN_CHANGE(IDC_EMISSION_EDIT, &Shader_solidworks_material_dialog::OnChange)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SOLIDWORKS_MATERIAL_SLIDER, &Shader_solidworks_material_dialog::OnNMCustomdrawSolidworksMaterialSlider)
END_MESSAGE_MAP()


// Shader_solidworks_material_dialog message handlers
void Shader_solidworks_material_dialog::get_parameter()
{
		red=sw->material[material_id].red;
		green=sw->material[material_id].green;
		blue=sw->material[material_id].blue;
		ambient=sw->material[material_id].ambient;
		diffuse=sw->material[material_id].diffuse;
		specular=sw->material[material_id].specular;
		shininess=sw->material[material_id].shininess;
		emission=sw->material[material_id].emission;
}
void Shader_solidworks_material_dialog::put_parameter()
{
		sw->material[material_id].red=red;
		sw->material[material_id].green=green;
		sw->material[material_id].blue=blue;
		sw->material[material_id].ambient=ambient;
		sw->material[material_id].diffuse=diffuse;
		sw->material[material_id].specular=specular;
		sw->material[material_id].shininess=shininess;
		sw->material[material_id].emission=emission;

		sw->save_material();
		theApp.redraw_flag=true;
}
void Shader_solidworks_material_dialog::OnChange()
{
	UpdateData(TRUE);
	put_parameter();
}
BOOL Shader_solidworks_material_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	get_parameter();
	for(int i=0;i<(sw->material_number);i++){
		CString str;
		str.Format(_T("²ÄÖÊ£º%d"),i+1);
		material_control.InsertString(i,str);
	}
	material_control.SetCurSel(0);

	slider_control.SetRange(0,10000);
	slider_control.SetPos((int)(red*10000));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void Shader_solidworks_material_dialog::OnCbnSelchangeMaterialIdCombo()
{
	if((material_id=material_control.GetCurSel())<0)
		material_id=0;
	else if(material_id>=(sw->material_number))
		material_id=sw->material_number-1;
}
void Shader_solidworks_material_dialog::OnEnSetfocusRedEdit()
{
	edit_id=0;
	slider_control.SetPos((int)(red*10000));
}
void Shader_solidworks_material_dialog::OnEnSetfocusGreenEdit()
{
	edit_id=1;
	slider_control.SetPos((int)(green*10000));
}
void Shader_solidworks_material_dialog::OnEnSetfocusBlueEdit()
{
	edit_id=2;
	slider_control.SetPos((int)(blue*10000));
}

void Shader_solidworks_material_dialog::OnEnSetfocusAmbientEdit()
{
	edit_id=3;
	slider_control.SetPos((int)(ambient*10000));
}
void Shader_solidworks_material_dialog::OnEnSetfocusDiffuseEdit()
{
	edit_id=4;
	slider_control.SetPos((int)(diffuse*10000));
}
void Shader_solidworks_material_dialog::OnEnSetfocusSpecularEdit()
{
	edit_id=5;
	slider_control.SetPos((int)(specular*10000));
}
void Shader_solidworks_material_dialog::OnEnSetfocusSpininessEdit()
{
	edit_id=6;
	slider_control.SetPos((int)(shininess*500));
}
void Shader_solidworks_material_dialog::OnEnSetfocusEmissionEdit()
{
	edit_id=7;
	slider_control.SetPos((int)(emission*10000));
}

void Shader_solidworks_material_dialog::OnNMCustomdrawSolidworksMaterialSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	switch(edit_id){
	case 0:
		red=(float)(slider_control.GetPos())/10000;
		break;
	case 1:
		green=(float)(slider_control.GetPos())/10000;
		break;
	case 2:
		blue=(float)(slider_control.GetPos())/10000;
		break;
	case 3:
		ambient=(float)(slider_control.GetPos())/10000;
		break;
	case 4:
		diffuse=(float)(slider_control.GetPos())/10000;
		break;
	case 5:
		specular=(float)(slider_control.GetPos())/10000;
		break;
	case 6:
		shininess=(float)(slider_control.GetPos())/500;
		break;
	case 7:
		emission=(float)(slider_control.GetPos())/10000;
		break;
	default:
		break;
	};
	UpdateData(FALSE);
	put_parameter();
}
