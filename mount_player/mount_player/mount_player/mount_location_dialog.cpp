// mount_location_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "mount_player.h"
#include "mount_location_dialog.h"


// mount_location_dialog dialog

IMPLEMENT_DYNAMIC(mount_location_dialog, CDialog)

static double dx=0,dy=0,dz=0,rx=0,ry=0,rz=0,time_length=1000;
static int main_step=1,sub_step=1;

mount_location_dialog::mount_location_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(mount_location_dialog::IDD, pParent)
//	, dx(0)
//	, dy(0)
//	, dz(0)
//	, rx(0)
//	, ry(0)
//	, rz(0)
//	, time_length(1000)
//	, main_step(1)
//	, sub_step(1)
//, save_to_cache_flag(FALSE)
//, cache_label(_T(""))
, save_to_cache_flag(FALSE)
{
	
}

mount_location_dialog::~mount_location_dialog()
{
}

void mount_location_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DX_EDIT, dx);
	DDX_Text(pDX, IDC_DY_EDIT, dy);
	DDX_Text(pDX, IDC_DZ_EDIT, dz);
	DDX_Text(pDX, IDC_RX_EDIT, rx);
	DDX_Text(pDX, IDC_RY_EDIT, ry);
	DDX_Text(pDX, IDC_RZ_EDIT, rz);
	DDX_Text(pDX, IDC_TIME_EDIT, time_length);
	DDX_Text(pDX, IDC_MAIN_STEP_EDIT, main_step);
	DDX_Text(pDX, IDC_SUB_STEP_EDIT, sub_step);
	DDX_Check(pDX, IDC_SAVE_TO_CACHE_CHECK, save_to_cache_flag);
}


BEGIN_MESSAGE_MAP(mount_location_dialog, CDialog)
	ON_BN_CLICKED(IDC_LAST_POINT, &mount_location_dialog::OnBnClickedLastPoint)
END_MESSAGE_MAP()


// mount_location_dialog message handlers


BOOL mount_location_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	this->dx=::dx;
	this->dy=::dy;
	this->dz=::dz;
	this->rx=::rx;
	this->ry=::ry;
	this->rz=::rz;
	this->time_length=::time_length;
	this->main_step=::main_step;
	this->sub_step=::sub_step;
	
	save_to_cache_flag=FALSE;
	last_point_flag=FALSE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void mount_location_dialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	UpdateData(TRUE);
	::dx=this->dx;
	::dy=this->dy;
	::dz=this->dz;
	::rx=this->rx;
	::ry=this->ry;
	::rz=this->rz;
	::time_length=this->time_length;
	::main_step=this->main_step;
	::sub_step=this->sub_step;

	last_point_flag=FALSE;

	CDialog::OnOK();
}
void mount_location_dialog::OnBnClickedLastPoint()
{
	// TODO: Add your control notification handler code here

	
	UpdateData(TRUE);
	::dx=this->dx;
	::dy=this->dy;
	::dz=this->dz;
	::rx=this->rx;
	::ry=this->ry;
	::rz=this->rz;
	::time_length=this->time_length;
	::main_step=this->main_step;
	::sub_step=this->sub_step;

	last_point_flag=TRUE;

	CDialog::OnOK();
}
