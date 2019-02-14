// mount_light_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "mount_player.h"
#include "mount_light_dialog.h"
#include "mount_view.h"
#include "MainFrm.h"
#include "mount_playerDoc.h"
#include "mount_playerView.h"

// mount_light_dialog dialog

IMPLEMENT_DYNAMIC(mount_light_dialog, CDialog)

mount_light_dialog::mount_light_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(mount_light_dialog::IDD, pParent)
	, m_x(0)
	, m_y(0)
	, m_z(0)
	, m_red(0)
	, m_green(0)
	, m_blue(0)
	, m_ambient(0)
	, m_diffuse(0)
	, m_emission(0)
	, m_shininess(0)
	, m_specular(0)
	, m_enable(FALSE)
	, m_light_id(0)
	, edit_pointer(NULL)
	, m_absolute_or_relative_flag(FALSE)
{
	
}

mount_light_dialog::~mount_light_dialog()
{
}

void mount_light_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_X_EDIT, m_x);
	DDX_Text(pDX, IDC_Y_EDIT, m_y);
	DDX_Text(pDX, IDC_Z_EDIT, m_z);
	DDX_Text(pDX, IDC_RED_EDIT, m_red);
	DDV_MinMaxDouble(pDX, m_red, 0, 1);
	DDX_Text(pDX, IDC_GREEN_EDIT, m_green);
	DDV_MinMaxDouble(pDX, m_green, 0, 1);
	DDX_Text(pDX, IDC_BLUE_EDIT, m_blue);
	DDV_MinMaxDouble(pDX, m_blue, 0, 1);
	DDX_Text(pDX, IDC_AMBIENT_EDIT, m_ambient);
	DDV_MinMaxDouble(pDX, m_ambient, 0, 1);
	DDX_Text(pDX, IDC_DIFFUSE_EDIT, m_diffuse);
	DDV_MinMaxDouble(pDX, m_diffuse, 0, 1);
	DDX_Text(pDX, IDC_EMISSION_EDIT, m_emission);
	DDV_MinMaxDouble(pDX, m_emission, 0, 1);
	DDX_Text(pDX, IDC_SHININESS_EDIT, m_shininess);
	DDV_MinMaxDouble(pDX, m_shininess, 0, 1);
	DDX_Text(pDX, IDC_SPECULAR_EDIT, m_specular);
	DDV_MinMaxDouble(pDX, m_specular, 0, 1);

	DDX_Check(pDX, IDC_ENABLE_CHECK, m_enable);
	DDX_Control(pDX, IDC_LIGHT_SLIDER, m_slider);
	DDX_Check(pDX, IDC_ABSOLUTE_RELATIVE_CHECK, m_absolute_or_relative_flag);
	DDX_Control(pDX, IDC_SEPARATE_SPECULAR_CHECK, separate_speclar_check);
}


BEGIN_MESSAGE_MAP(mount_light_dialog, CDialog)
	ON_BN_CLICKED(IDC_SELECT_1_RADIO, &mount_light_dialog::OnBnClickedSelect1Radio)
	ON_BN_CLICKED(IDC_SELECT_2_RADIO, &mount_light_dialog::OnBnClickedSelect2Radio)
	ON_BN_CLICKED(IDC_SELECT_3_RADIO, &mount_light_dialog::OnBnClickedSelect3Radio)
	ON_BN_CLICKED(IDC_SELECT_4_RADIO, &mount_light_dialog::OnBnClickedSelect4Radio)
	ON_BN_CLICKED(IDC_SELECT_5_RADIO, &mount_light_dialog::OnBnClickedSelect5Radio)
	ON_BN_CLICKED(IDC_SELECT_6_RADIO, &mount_light_dialog::OnBnClickedSelect6Radio)
	ON_BN_CLICKED(IDC_SELECT_7_RADIO, &mount_light_dialog::OnBnClickedSelect7Radio)
	ON_BN_CLICKED(IDC_SELECT_8_RADIO, &mount_light_dialog::OnBnClickedSelect8Radio)
	ON_BN_CLICKED(IDC_SELECT_SELECTED_RADIO, &mount_light_dialog::OnBnClickedSelectSelectedRadio)
	ON_BN_CLICKED(IDC_SELECT_BACKGROUND_RADIO, &mount_light_dialog::OnBnClickedSelectBackgroundRadio)
	ON_EN_SETFOCUS(IDC_RED_EDIT, &mount_light_dialog::OnEnSetfocusRedEdit)
	ON_EN_CHANGE(IDC_RED_EDIT, &mount_light_dialog::OnEnChangeRedEdit)
	ON_EN_SETFOCUS(IDC_GREEN_EDIT, &mount_light_dialog::OnEnSetfocusGreenEdit)
	ON_EN_CHANGE(IDC_GREEN_EDIT, &mount_light_dialog::OnEnChangeGreenEdit)
	ON_EN_SETFOCUS(IDC_BLUE_EDIT, &mount_light_dialog::OnEnSetfocusBlueEdit)
	ON_EN_CHANGE(IDC_BLUE_EDIT, &mount_light_dialog::OnEnChangeBlueEdit)
	ON_EN_SETFOCUS(IDC_AMBIENT_EDIT, &mount_light_dialog::OnEnSetfocusAmbientEdit)
	ON_EN_CHANGE(IDC_AMBIENT_EDIT, &mount_light_dialog::OnEnChangeAmbientEdit)
	ON_EN_SETFOCUS(IDC_DIFFUSE_EDIT, &mount_light_dialog::OnEnSetfocusDiffuseEdit)
	ON_EN_CHANGE(IDC_DIFFUSE_EDIT, &mount_light_dialog::OnEnChangeDiffuseEdit)
	ON_EN_SETFOCUS(IDC_EMISSION_EDIT, &mount_light_dialog::OnEnSetfocusEmissionEdit)
	ON_EN_CHANGE(IDC_EMISSION_EDIT, &mount_light_dialog::OnEnChangeEmissionEdit)
	ON_EN_SETFOCUS(IDC_SHININESS_EDIT, &mount_light_dialog::OnEnSetfocusShininessEdit)
	ON_EN_CHANGE(IDC_SHININESS_EDIT, &mount_light_dialog::OnEnChangeShininessEdit)
	ON_EN_SETFOCUS(IDC_SPECULAR_EDIT, &mount_light_dialog::OnEnSetfocusSpecularEdit)
	ON_EN_CHANGE(IDC_SPECULAR_EDIT, &mount_light_dialog::OnEnChangeSpecularEdit)
	ON_EN_CHANGE(IDC_X_EDIT, &mount_light_dialog::OnEnChangeXEdit)
	ON_EN_CHANGE(IDC_Y_EDIT, &mount_light_dialog::OnEnChangeYEdit)
	ON_EN_CHANGE(IDC_Z_EDIT, &mount_light_dialog::OnEnChangeZEdit)

	ON_BN_CLICKED      (IDC_ENABLE_CHECK, &mount_light_dialog::OnBnClickedEnableCheck)
	
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIGHT_SLIDER, &mount_light_dialog::OnNMCustomdrawLightSlider)
	ON_WM_CLOSE()
//	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ABSOLUTE_RELATIVE_CHECK, &mount_light_dialog::OnBnClickedAbsoluteRelativeCheck)
	
	ON_BN_CLICKED(IDC_COLOR_INPUT_BUTTON, &mount_light_dialog::OnBnClickedColorInputButton)
	ON_BN_CLICKED(IDC_SELECT_ENVIRONMENT_RADIO, &mount_light_dialog::OnBnClickedSelectEnvironmentRadio)
	ON_BN_CLICKED(IDC_SEPARATE_SPECULAR_CHECK, &mount_light_dialog::OnBnClickedSeparateSpecularCheck)
END_MESSAGE_MAP()


// mount_light_dialog message handlers

void mount_light_dialog::get_value(void)
{
	struct graphics_system_light *light;
	double *p;

	UpdateData(TRUE);

	if((m_light_id>=0)&&(m_light_id<MAX_LIGHT)){
		light=&(bridge_view->light[m_light_id]);
	
		light->effective_flag	=m_enable;
		light->absolute_or_relative_flag=m_absolute_or_relative_flag;

		light->x			=m_x;
		light->y			=m_y;
		light->z			=m_z;

		light->R			=m_red;
		light->G			=m_green;
		light->B			=m_blue;
	
		light->Ambient		=m_ambient;
		light->Diffuse		=m_diffuse;
		light->Specular		=m_specular;

		light->Shininess	=m_shininess;
		light->Emission		=m_emission;
	
	}else if(m_light_id==(1+MAX_LIGHT)){
		p=&(bridge_view->selected_material_property.material_value[0]);

		p[0]=m_red;
		p[1]=m_green;
		p[2]=m_blue;
	
		p[3]=m_ambient;
		p[4]=m_diffuse;
		p[5]=m_specular;

		p[6]=m_shininess;
		p[8]=m_emission;
	
	}else if(m_light_id==MAX_LIGHT){
		bridge_view->background_red_color	=m_red;
		bridge_view->background_green_color	=m_green;
		bridge_view->background_blue_color	=m_blue;
	}else if(m_light_id==(2+MAX_LIGHT)){
		bridge_view->environment_red_color	=m_red;
		bridge_view->environment_green_color=m_green;
		bridge_view->environment_blue_color	=m_blue;
	};

	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;
	
	Cmount_playerView* pView=(Cmount_playerView*)(pFrame->GetActiveView());
	if(pView==NULL)
		return;

	pView->InvalidateRect(NULL,FALSE);

	bridge_view->save_light();

	return;
}

void mount_light_dialog::set_display(int  light_id)
{
	struct graphics_system_light *light;
	double *p;

	if((light_id>=0)&&(light_id<MAX_LIGHT)){
		m_light_id=light_id;
		light=&(bridge_view->light[m_light_id]);

		m_enable					=light->effective_flag;
		m_absolute_or_relative_flag	=light->absolute_or_relative_flag;

		m_x			=light->x;
		m_y			=light->y;
		m_z			=light->z;

		m_red		=light->R;
		m_green		=light->G;
		m_blue		=light->B;
	
		m_ambient	=light->Ambient;
		m_diffuse	=light->Diffuse;
		m_specular	=light->Specular;

		m_shininess	=light->Shininess;
		m_emission	=light->Emission;

	}else if((m_light_id=light_id)==(1+MAX_LIGHT)){
		p=&(bridge_view->selected_material_property.material_value[0]);

		m_enable	=TRUE;

		m_red		=p[0];
		m_green		=p[1];
		m_blue		=p[2];
	
		m_ambient	=p[3];
		m_diffuse	=p[4];
		m_specular	=p[5];

		m_shininess	=p[6];
		m_emission	=p[8];
		
	}else if((m_light_id=light_id)==MAX_LIGHT){
		m_enable	=TRUE;

		m_red		=bridge_view->background_red_color;
		m_green		=bridge_view->background_green_color;
		m_blue		=bridge_view->background_blue_color;

		m_ambient	=0;
		m_diffuse	=0;
		m_specular	=0;

		m_shininess	=0;
		m_emission	=0;

	}else if((m_light_id=light_id)==(2+MAX_LIGHT)){
		m_enable	=TRUE;

		m_red		=bridge_view->environment_red_color;
		m_green		=bridge_view->environment_green_color;
		m_blue		=bridge_view->environment_blue_color;

		m_ambient	=0;
		m_diffuse	=0;
		m_specular	=0;

		m_shininess	=0;
		m_emission	=0;
	}
	UpdateData(FALSE);
}

void mount_light_dialog::set_edit_pointer(double * p)
{
	if((*p)<0)
		(*p)=0;
	if((*p)>1.0)
		(*p)=1.0;
	edit_pointer=p;
	m_slider.SetPos((int)(10000*(*p)));
	return;
}

void mount_light_dialog::OnBnClickedSelect1Radio()
{
	// TODO: Add your control notification handler code here
	set_display(0);
}

void mount_light_dialog::OnBnClickedSelect2Radio()
{
	// TODO: Add your control notification handler code here
	set_display(1);
}

void mount_light_dialog::OnBnClickedSelect3Radio()
{
	// TODO: Add your control notification handler code here
	set_display(2);
}

void mount_light_dialog::OnBnClickedSelect4Radio()
{
	// TODO: Add your control notification handler code here
	set_display(3);
}

void mount_light_dialog::OnBnClickedSelect5Radio()
{
	// TODO: Add your control notification handler code here
	set_display(4);
}

void mount_light_dialog::OnBnClickedSelect6Radio()
{
	// TODO: Add your control notification handler code here
	set_display(5);
}

void mount_light_dialog::OnBnClickedSelect7Radio()
{
	// TODO: Add your control notification handler code here
	set_display(6);
}

void mount_light_dialog::OnBnClickedSelect8Radio()
{
	// TODO: Add your control notification handler code here
	set_display(7);
}

void mount_light_dialog::OnBnClickedSelectSelectedRadio()
{
	// TODO: Add your control notification handler code here
	set_display(1+MAX_LIGHT);
}

void mount_light_dialog::OnBnClickedSelectBackgroundRadio()
{
	// TODO: Add your control notification handler code here
	set_display(MAX_LIGHT);
}

void mount_light_dialog::OnBnClickedSelectEnvironmentRadio()
{
	// TODO: Add your control notification handler code here
	set_display(2+MAX_LIGHT);
}

void mount_light_dialog::OnEnSetfocusRedEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_red);
}

void mount_light_dialog::OnEnChangeRedEdit()
{

	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnEnSetfocusGreenEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_green);
}

void mount_light_dialog::OnEnChangeGreenEdit()
{

	// TODO:  Add your control notification handler code here

	get_value();
}

void mount_light_dialog::OnEnSetfocusBlueEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_blue);
}

void mount_light_dialog::OnEnChangeBlueEdit()
{
	// TODO:  Add your control notification handler code here
	get_value();
}
void mount_light_dialog::OnEnSetfocusAmbientEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_ambient);
}
void mount_light_dialog::OnEnChangeAmbientEdit()
{
	// TODO:  Add your control notification handler code here
	get_value();
}
void mount_light_dialog::OnEnSetfocusDiffuseEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_diffuse);
}

void mount_light_dialog::OnEnChangeDiffuseEdit()
{

	// TODO:  Add your control notification handler code here
	get_value();
}
void mount_light_dialog::OnEnSetfocusEmissionEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_emission);
}

void mount_light_dialog::OnEnChangeEmissionEdit()
{
	
	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnEnSetfocusShininessEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_shininess);
}

void mount_light_dialog::OnEnChangeShininessEdit()
{
	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnEnSetfocusSpecularEdit()
{
	// TODO: Add your control notification handler code here
	set_edit_pointer(&m_specular);
}

void mount_light_dialog::OnEnChangeSpecularEdit()
{

	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnEnChangeXEdit()
{
	
	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnEnChangeYEdit()
{

	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnEnChangeZEdit()
{

	// TODO:  Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnBnClickedEnableCheck()
{
	// TODO: Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnNMCustomdrawLightSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if(edit_pointer!=NULL)
		*edit_pointer=(double)(m_slider.GetPos())/(double)10000;
	UpdateData(FALSE);
	get_value();
}
BOOL mount_light_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_slider.SetRange(0,10000);
	m_slider.SetPos(0);
	set_display(m_light_id);

	CheckRadioButton(IDC_SELECT_1_RADIO,IDC_SELECT_8_RADIO,IDC_SELECT_1_RADIO);

	separate_speclar_check.SetWindowText(bridge_view->separate_specular_color_flag?"镜面辅助颜色":"单色");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void mount_light_dialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default

//	CDialog::OnClose();

	ShowWindow(SW_HIDE);
}

void mount_light_dialog::OnBnClickedAbsoluteRelativeCheck()
{
	// TODO: Add your control notification handler code here
	get_value();
}

void mount_light_dialog::OnBnClickedColorInputButton()
{
	// TODO: Add your control notification handler code here
	get_value();
	CColorDialog dlg(RGB((int)(m_red*255),(int)(m_green*255),(int)(m_blue*255)),NULL,this); 

	if(dlg.DoModal()==IDOK){
		get_value();
		m_red   =(double)(GetRValue(dlg.GetColor()))/255.0;
		m_green =(double)(GetGValue(dlg.GetColor()))/255.0;
		m_blue  =(double)(GetBValue(dlg.GetColor()))/255.0;
		UpdateData(FALSE);
		get_value();
	}
	CMainFrame*  pFrame=(CMainFrame*)(theApp.m_pMainWnd);
	if(pFrame==NULL)
		return;
	
	Cmount_playerView* pView=(Cmount_playerView*)(pFrame->GetActiveView());
	if(pView==NULL)
		return;
	pView->InvalidateRect(NULL,FALSE);
}


void mount_light_dialog::OnBnClickedSeparateSpecularCheck()
{
	// TODO: Add your control notification handler code here
	bridge_view->separate_specular_color_flag=bridge_view->separate_specular_color_flag?FALSE:TRUE;
	separate_speclar_check.SetWindowText(bridge_view->separate_specular_color_flag?"镜面辅助颜色":"单色");
	get_value();
}
