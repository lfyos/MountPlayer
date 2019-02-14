// Mount_information_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "../Shader_player.h"
#include "Mount_information_dialog.h"


// Mount_information_dialog dialog

IMPLEMENT_DYNAMIC(Mount_information_dialog, CDialog)

Mount_information_dialog::Mount_information_dialog(int *my_main_step,int *my_sub_step,CString *my_sound_file,CString *my_title,CWnd* pParent)
	: CDialog(Mount_information_dialog::IDD, pParent)
{
	main_step=my_main_step;
	sub_step=my_sub_step;
	sound_file_name=my_sound_file;
	title=my_title;

 	start_state_flag=FALSE;
	end_state_flag=TRUE;
}

Mount_information_dialog::~Mount_information_dialog()
{

}

void Mount_information_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAIN_STEP_EDIT, (*main_step));
	DDX_Text(pDX, IDC_SUB_STEP_EDIT, (*sub_step));
	DDX_Text(pDX, IDC_SOUND_EDIT, (*sound_file_name));
	DDX_Text(pDX, IDC_TITLE_EDIT, (*title));
	DDX_Control(pDX, IDC_SOUND_EDIT, sound_edit);
	DDX_Control(pDX, IDC_TITLE_EDIT, title_edit);
	DDX_Check(pDX, IDC_START_CHECK, start_state_flag);
	DDX_Check(pDX, IDC_END_CHECK, end_state_flag);
	DDX_Control(pDX, IDC_START_CHECK, start_state_button);
	DDX_Control(pDX, IDC_END_CHECK, end_state_button);
}

BEGIN_MESSAGE_MAP(Mount_information_dialog, CDialog)
	ON_BN_CLICKED(IDC_START_CHECK, &Mount_information_dialog::OnBnClickedStartCheck)
	ON_BN_CLICKED(IDC_END_CHECK, &Mount_information_dialog::OnBnClickedEndCheck)
END_MESSAGE_MAP()


// Mount_information_dialog message handlers

void Mount_information_dialog::OnBnClickedStartCheck()
{
	UpdateData(TRUE);
	start_state_button.SetWindowText(start_state_flag?_T("开始显示"):_T("开始隐藏"));
}

void Mount_information_dialog::OnBnClickedEndCheck()
{
	UpdateData(TRUE);
	end_state_button.SetWindowText(end_state_flag?_T("结束显示"):_T("结束隐藏"));
}

BOOL Mount_information_dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBnClickedStartCheck();
	OnBnClickedEndCheck();


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
