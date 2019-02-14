// input_main_sub_step_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "mount_player.h"
#include "input_main_sub_step_dialog.h"


// input_main_sub_step_dialog dialog

IMPLEMENT_DYNAMIC(input_main_sub_step_dialog, CDialog)

static int last_main_step=1,last_sub_step=1;

input_main_sub_step_dialog::input_main_sub_step_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(input_main_sub_step_dialog::IDD, pParent)
{
	main_step_id=::last_main_step;
	sub_step_id=::last_sub_step;
}

input_main_sub_step_dialog::~input_main_sub_step_dialog()
{
	::last_main_step=main_step_id;
	::last_sub_step=sub_step_id;
}

void input_main_sub_step_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAIN_STEP_EDIT, main_step_id);
	DDX_Text(pDX, IDC_SUB_STEP_EDIT, sub_step_id);
}


BEGIN_MESSAGE_MAP(input_main_sub_step_dialog, CDialog)

END_MESSAGE_MAP()

