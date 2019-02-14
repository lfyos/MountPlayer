// Location_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "../Shader_player.h"
#include "../kernel/User_interface.h"
#include "Location_dialog.h"



// Location_dialog dialog

IMPLEMENT_DYNAMIC(Location_dialog, CDialog)


static double last_mx=0,last_my=0,last_mz=0,last_rx=0,last_ry=0,last_rz=0;

void Location_dialog::reset()
{
		last_mx=0;
		last_my=0;
		last_mz=0;
		last_rx=0;
		last_ry=0;
		last_rz=0;
}
Location_dialog::Location_dialog(Component *my_comp,Part_set *my_parts,CWnd* pParent)
	: CDialog(Location_dialog::IDD, pParent), slider(500)
{
	m_x=last_mx;
	m_y=last_my;
	m_z=last_mz;
	r_x=last_rx;
	r_y=last_ry;
	r_z=last_rz;

	pointer=&m_x;

	comp=my_comp;
	parts=my_parts;

	set_min_and_max_value();
}

Location_dialog::~Location_dialog()
{
	last_mx=m_x;
	last_my=m_y;
	last_mz=m_z;
	last_rx=r_x;
	last_ry=r_y;
	last_rz=r_z;
}

void Location_dialog::set_min_and_max_value()
{
	max_value=comp->component_box.distance()*10;
	min_value=(-max_value);
}

void Location_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X_EDIT, m_x);
	DDX_Text(pDX, IDC_Y_EDIT, m_y);
	DDX_Text(pDX, IDC_Z_EDIT, m_z);
	DDX_Text(pDX, IDC_RX_EDIT, r_x);
	DDV_MinMaxDouble(pDX, r_x, 0, 360);
	DDX_Text(pDX, IDC_RY_EDIT, r_y);
	DDV_MinMaxDouble(pDX, r_y, 0, 360);
	DDX_Text(pDX, IDC_RZ_EDIT, r_z);
	DDV_MinMaxDouble(pDX, r_z, 0, 360);
	DDX_Slider(pDX, IDC_DATA_SLIDER, slider);
	DDV_MinMaxInt(pDX, slider, 0, 1000);
	DDX_Control(pDX, IDC_DATA_SLIDER, slider_controller);
}


BEGIN_MESSAGE_MAP(Location_dialog, CDialog)
	

	ON_EN_CHANGE(IDC_X_EDIT, &Location_dialog::OnEnChangeXEdit)
	ON_EN_CHANGE(IDC_Y_EDIT, &Location_dialog::OnEnChangeYEdit)
	ON_EN_CHANGE(IDC_Z_EDIT, &Location_dialog::OnEnChangeZEdit)
	ON_EN_CHANGE(IDC_RX_EDIT, &Location_dialog::OnEnChangeRxEdit)
	ON_EN_CHANGE(IDC_RY_EDIT, &Location_dialog::OnEnChangeRyEdit)
	ON_EN_CHANGE(IDC_RZ_EDIT, &Location_dialog::OnEnChangeRzEdit)
	ON_EN_SETFOCUS(IDC_X_EDIT, &Location_dialog::OnEnSetfocusXEdit)
	ON_EN_SETFOCUS(IDC_Y_EDIT, &Location_dialog::OnEnSetfocusYEdit)
	ON_EN_SETFOCUS(IDC_Z_EDIT, &Location_dialog::OnEnSetfocusZEdit)
	ON_EN_SETFOCUS(IDC_RX_EDIT, &Location_dialog::OnEnSetfocusRxEdit)
	ON_EN_SETFOCUS(IDC_RY_EDIT, &Location_dialog::OnEnSetfocusRyEdit)
	ON_EN_SETFOCUS(IDC_RZ_EDIT, &Location_dialog::OnEnSetfocusRzEdit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DATA_SLIDER, &Location_dialog::OnNMCustomdrawDataSlider)
END_MESSAGE_MAP()


// Location_dialog message handlers

void Location_dialog::OnEnChangeXEdit()
{
	process();
}

void Location_dialog::OnEnChangeYEdit()
{
	process();
}

void Location_dialog::OnEnChangeZEdit()
{
	process();
}

void Location_dialog::OnEnChangeRxEdit()
{
	process();
}

void Location_dialog::OnEnChangeRyEdit()
{
	process();
}

void Location_dialog::OnEnChangeRzEdit()
{
	process();
}
void Location_dialog::OnEnSetfocusXEdit()
{
	pointer=&m_x;
	set_min_and_max_value();
	set_slider();
}

void Location_dialog::OnEnSetfocusYEdit()
{
	pointer=&m_y;
	set_min_and_max_value();
	set_slider();
}

void Location_dialog::OnEnSetfocusZEdit()
{
	pointer=&m_z;
	set_min_and_max_value();
	set_slider();
}

void Location_dialog::OnEnSetfocusRxEdit()
{
	pointer=&r_x;
	min_value=0;
	max_value=360;
	set_slider();
}

void Location_dialog::OnEnSetfocusRyEdit()
{
	pointer=&r_y;
	min_value=0;
	max_value=360;
	set_slider();
}

void Location_dialog::OnEnSetfocusRzEdit()
{
	pointer=&r_z;
	min_value=0;
	max_value=360;
	set_slider();
}
void Location_dialog::process(void)
{
	if(comp==NULL)
		return;
	UpdateData();

	component_location=
		 (Location::move(m_x,m_y,m_z))
		*(Location::z_rotate(r_z))
		*(Location::y_rotate(r_y))
		*(Location::x_rotate(r_x));

	comp->modify_move_absulate_location(component_location,*parts);

	theApp.redraw_flag=true;
}


void Location_dialog::OnNMCustomdrawDataSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateData();
	
	*pointer=min_value+(max_value-min_value)*((double)slider)/1000;

	UpdateData(FALSE);
	process();
}

BOOL Location_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	slider_controller.SetRange(0,1000);
	slider_controller.SetPos(500);

	return TRUE;
}

void  Location_dialog::set_slider(void)
{
	double p=(*pointer-min_value)/(max_value-min_value);
	slider_controller.SetPos((int)(1000*p));
}