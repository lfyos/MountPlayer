// user_interface\Mount_step_tag_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\Shader_player.h"
#include "Mount_step_tag_dialog.h"


// Mount_step_tag_dialog dialog

IMPLEMENT_DYNAMIC(Mount_step_tag_dialog, CDialog)

Mount_step_tag_dialog::Mount_step_tag_dialog(CString my_tag_string,int my_x0,int my_y0,CWnd* pParent /*=NULL*/)
	: CDialog(Mount_step_tag_dialog::IDD, pParent)
{
	tag_string=my_tag_string;
	x0=my_x0;
	y0=my_y0;
}

Mount_step_tag_dialog::~Mount_step_tag_dialog()
{
}

void Mount_step_tag_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TAG_EDIT, tag_string);
}


BEGIN_MESSAGE_MAP(Mount_step_tag_dialog, CDialog)
END_MESSAGE_MAP()


// Mount_step_tag_dialog message handlers

BOOL Mount_step_tag_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rt;
	GetWindowRect(rt);
	rt.MoveToXY(x0,y0);


	MoveWindow(rt);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
