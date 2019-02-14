// Userface\Mount_step_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\Shader_player.h"
#include "Mount_step_dialog.h"


// Mount_step_dialog dialog

IMPLEMENT_DYNAMIC(Mount_step_dialog, CDialog)

Mount_step_dialog::Mount_step_dialog(CWnd* pParent,CString my_title)
	: CDialog(Mount_step_dialog::IDD, pParent)
	, step(1)
{
	window_title=my_title;
}

Mount_step_dialog::~Mount_step_dialog()
{
}

void Mount_step_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STEP_EDIT, step);
}
BEGIN_MESSAGE_MAP(Mount_step_dialog, CDialog)
END_MESSAGE_MAP()


// Mount_step_dialog message handlers

BOOL Mount_step_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	SetWindowText(window_title);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
